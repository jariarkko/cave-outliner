  
///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <cassert>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerhighprecision.hh"
#include "outlinermath.hh"
#include "outlinerdebug.hh"
#include "outlinerprocessor.hh"
#include "outlinerprocessorcrosssection.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Model processing ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

ProcessorCrossSection::ProcessorCrossSection(const char* fileNameIn,
                                             enum outlinerdirection sliceDirectionIn,
                                             const HighPrecisionVector2D& lineStartIn,
                                             const HighPrecisionVector2D& lineEndIn,
                                             Processor& procIn) :
  fileName(fileNameIn),
  sliceDirection(sliceDirectionIn),
  lineStart(lineStartIn),
  lineEnd(lineEndIn),
  proc(procIn),
  svg(0) {
  calculateLineEquation();
}

ProcessorCrossSection::~ProcessorCrossSection() {
  deleteSvg();
}

void
ProcessorCrossSection::deleteSvg(void) {
  if (svg != 0) {
    
    // Check that file I/O was ok
    if (!svg->ok()) {
      errf("File output to %s failed", fileName);
      exit(1);
    }

    // Delete the object
    delete svg;
    svg = 0;
  }
}

void
ProcessorCrossSection::calculateLineEquation(void) {
  debugf("calculate cross section line equation for (%.2f,%.2f)-(%.2f,%.2f)",
         lineStart.x, lineStart.y, lineEnd.x, lineEnd.y);
  xDifference = lineEnd.x - lineStart.x;
  yDifference = lineEnd.y - lineStart.y;
  if (xDifference == 0 && yDifference == 0) {
    errf("Cross-section starting and ending points cannot be same");
    exit(1);
  }
  lineLength = sqrt(xDifference*xDifference + yDifference*yDifference);
  lineStep = (proc.stepx + proc.stepy)/2.0;
  lineSteps = lineLength/lineStep;
  lineStepX = xDifference / lineSteps;
  lineStepY = yDifference / lineSteps;
  debugf("cross section line equation length %.2f steps %.2f stepX %.2f stepY %.2f",
         lineLength,
         lineSteps,
         lineStepX,
         lineStepY);
}

void
ProcessorCrossSection::lineIteratorInit(struct ProcessorCrossSectionLineIterator& iter) {
  iter.x = lineStart.x;
  iter.y = lineStart.y;
}

bool
ProcessorCrossSection::lineIteratorDone(struct ProcessorCrossSectionLineIterator& iter) {
  if (lineEnd.x > lineStart.x && iter.x > lineEnd.x) return(1);
  if (lineEnd.x < lineStart.x && iter.x < lineEnd.x) return(1);
  if (lineEnd.y > lineStart.y && iter.y > lineEnd.y) return(1);
  if (lineEnd.y < lineStart.y && iter.y < lineEnd.y) return(1);
  return(0);
}

void
ProcessorCrossSection::lineIteratorNext(struct ProcessorCrossSectionLineIterator& iter) {
  iter.x += lineStepX;
  iter.y += lineStepY;
}

bool
ProcessorCrossSection::processSceneCrossSection(const aiScene* scene) {
  deleteSvg();
  debugf("process scene cross section");
  sliceVerticalBoundingBox(scene,sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
  svg = proc.createSvg(fileName,sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd,sliceDirection);
  return(1);
}

void
ProcessorCrossSection::sliceVerticalBoundingBox(const aiScene* scene,
                                                HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                                HighPrecisionVector2D& sliceVerticalBoundingBoxEnd) {
  assert(scene != 0);
  deepdebugf("process cross section bounding box");
  bool set = 0;
  sliceVerticalBoundingBoxNode(scene,scene->mRootNode,
                               set,
                               sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
}

void
ProcessorCrossSection::sliceVerticalBoundingBoxNode(const aiScene* scene,
                                                    const aiNode* node,
                                                    bool& set,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxEnd) {
  assert(scene != 0);
  assert(node != 0);
  debugf("process node cross section");
  if (!node->mTransformation.IsIdentity()) {
    errf("Cannot handle transformations yet");
    exit(1);
  }
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    sliceVerticalBoundingBoxMesh(scene,scene->mMeshes[node->mMeshes[j]],
                                 set,
                                 sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    sliceVerticalBoundingBoxNode(scene,node->mChildren[i],
                                 set,
                                 sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
  }
}

void
ProcessorCrossSection::sliceVerticalBoundingBoxMesh(const aiScene* scene,
                                                    const aiMesh* mesh,
                                                    bool& set,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxEnd) {
  assert(scene != 0);
  assert(mesh != 0);
  debugf("process mesh cross section");
  struct ProcessorCrossSectionLineIterator iter;
  lineIteratorInit(iter);
  for (; !lineIteratorDone(iter); lineIteratorNext(iter)) {
    unsigned int nFaces = 0;
    const aiFace** faces = 0;
    proc.indexed.getFaces(mesh,iter.x,iter.y,&nFaces,&faces);
    if (nFaces > 0) {
      deepdebugf("got %u cross section faces from (%.2f,%.2f)", nFaces, iter.x, iter.y);
      for (unsigned int f = 0; f < nFaces; f++) {
        sliceVerticalBoundingBoxFace(scene,mesh,faces[f],
                                     iter.x, iter.y,
                                     set,
                                     sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
      }
    }
  }
}

void
ProcessorCrossSection::sliceVerticalBoundingBoxFace(const aiScene* scene,
                                                    const aiMesh* mesh,
                                                    const aiFace* face,
                                                    outlinerhighprecisionreal x,
                                                    outlinerhighprecisionreal y,
                                                    bool& set,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxEnd) {
  aiVector2D a;
  aiVector2D b;
  aiVector2D c;
  proc.faceGetVertices(mesh,face,sliceDirection,a,b,c);
  HighPrecisionVector2D point(x,y);
  HighPrecisionVector2D stepboundingbox(x+lineStepX,y+lineStepY);
  if (OutlinerMath::boundingBoxIntersectsTriangle2D(a,b,c,point,stepboundingbox)) {
    deepdeepdebugf("cross section face (%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f) hits step bounding box (%.2f,%.2f)-(%.2f,%.2f)",
                   mesh->mVertices[face->mIndices[0]].x,mesh->mVertices[face->mIndices[0]].y,mesh->mVertices[face->mIndices[0]].z,
                   mesh->mVertices[face->mIndices[1]].x,mesh->mVertices[face->mIndices[1]].y,mesh->mVertices[face->mIndices[1]].z,
                   mesh->mVertices[face->mIndices[2]].x,mesh->mVertices[face->mIndices[2]].y,mesh->mVertices[face->mIndices[2]].z,
                   point.x, point.y,
                   stepboundingbox.x,
                   stepboundingbox.y);
    HighPrecisionVector2D faceBoundingBoxStart;
    HighPrecisionVector2D faceBoundingBoxEnd;
    OutlinerMath::triangleBoundingBox2D(a,b,c,
                                        faceBoundingBoxStart,
                                        faceBoundingBoxEnd);
    HighPrecisionVector2D resultBoundingBoxStart;
    HighPrecisionVector2D resultBoundingBoxEnd;
    OutlinerMath::boundingBoxIntersection(point,
                                          stepboundingbox,
                                          faceBoundingBoxStart,
                                          faceBoundingBoxEnd,
                                          resultBoundingBoxStart,
                                          resultBoundingBoxEnd);
    if (!set) {
      sliceVerticalBoundingBoxStart = resultBoundingBoxStart;
      sliceVerticalBoundingBoxEnd = resultBoundingBoxEnd;
      deepdebugf("setting initial cross section bounding box at (%.2f,%.2f) to (%.2f,%.2f)-(%.2f,%.2f)",
                 x, y,
                 sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
                 sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y);
      set = 1;
    } else {
      HighPrecisionVector2D newBoundingBoxStart;
      HighPrecisionVector2D newBoundingBoxEnd;
      OutlinerMath::boundingBoxUnion(resultBoundingBoxStart,
                                     resultBoundingBoxEnd,
                                     sliceVerticalBoundingBoxStart,
                                     sliceVerticalBoundingBoxEnd,
                                     newBoundingBoxStart,
                                     newBoundingBoxEnd);
      deepdebugf("testing new cross section bounding box for face (%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f) at (%.2f,%.2f) as (%.2f,%.2f)-(%.2f,%.2f)",
                 mesh->mVertices[face->mIndices[0]].x,mesh->mVertices[face->mIndices[0]].y,mesh->mVertices[face->mIndices[0]].z,
                 mesh->mVertices[face->mIndices[1]].x,mesh->mVertices[face->mIndices[1]].y,mesh->mVertices[face->mIndices[1]].z,
                 mesh->mVertices[face->mIndices[2]].x,mesh->mVertices[face->mIndices[2]].y,mesh->mVertices[face->mIndices[2]].z,
                 x, y,
                 newBoundingBoxStart.x, newBoundingBoxStart.y,
                 newBoundingBoxEnd.x, newBoundingBoxEnd.y);
      if (!OutlinerMath::boundingBoxEqual(sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd,
                                          newBoundingBoxStart,newBoundingBoxEnd)) {
        sliceVerticalBoundingBoxStart = newBoundingBoxStart;
        sliceVerticalBoundingBoxEnd = newBoundingBoxEnd;
        deepdebugf("setting new cross section bounding box to (%.2f,%.2f)-(%.2f,%.2f)",
                   sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
                   sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y);
      }
    } 
  } else {
    deepdeepdebugf("cross section face (%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f) "
                   "for direction %s (%.2f,%.2f)-(%.2f,%.2f)-(%.2f,%.2f) does NOT hit "
                   "step bounding box (%.2f,%.2f)-(%.2f,%.2f)",
                   mesh->mVertices[face->mIndices[0]].x,mesh->mVertices[face->mIndices[0]].y,mesh->mVertices[face->mIndices[0]].z,
                   mesh->mVertices[face->mIndices[1]].x,mesh->mVertices[face->mIndices[1]].y,mesh->mVertices[face->mIndices[1]].z,
                   mesh->mVertices[face->mIndices[2]].x,mesh->mVertices[face->mIndices[2]].y,mesh->mVertices[face->mIndices[2]].z,
                   DirectionOperations::toString(sliceDirection),
                   a.x, a.y, b.x, b.y, c.x, c.y,
                   point.x, point.y,
                   stepboundingbox.x,
                   stepboundingbox.y);
  }
}

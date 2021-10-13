  
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
// Object creation and destruction ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

ProcessorCrossSection::ProcessorCrossSection(const char* fileNameIn,
                                             enum outlinerdirection sliceDirectionIn,
                                             const HighPrecisionVector2D& lineStartIn,
                                             const HighPrecisionVector2D& lineEndIn,
                                             outlinerhighprecisionreal stepzIn,
                                             Processor& procIn) :
  fileName(fileNameIn),
  sliceDirection(sliceDirectionIn),
  lineStart(lineStartIn),
  lineEnd(lineEndIn),
  stepz(stepzIn),
  matrix(0),
  proc(procIn),
  svg(0) {
  calculateLineEquation();
}

ProcessorCrossSection::~ProcessorCrossSection() {
  deleteSvg();
  deleteMatrix();
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
ProcessorCrossSection::deleteMatrix(void) {
  if (matrix != 0) {
    
    // Delete the object
    delete matrix;
    matrix = 0;
    
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Main program for generating a cross section ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
ProcessorCrossSection::processSceneCrossSection(const aiScene* scene) {

  // Get rid of previous image and matrix, if any
  deleteSvg();
  deleteMatrix();
  
  // Debugs
  debugf("process scene cross section");
  
  // Determine the bounding box of what we see in the cross section
  sliceVerticalBoundingBox(scene,sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
  infof("  cross section bounding box (%.2f,%.2f) to (%.2f,%.2f)",
        sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
        sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y);
  infof("  steps %.2f and %.2f", proc.stepy, stepz);
  
  // Increase bounding box to each side
  sliceVerticalBoundingBoxStart.x -= 2*proc.stepy;
  sliceVerticalBoundingBoxStart.y -= 2*stepz;
  infof("  cross section bounding box after adjustment (%.2f,%.2f) to (%.2f,%.2f)",
         sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
         sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y);
  
  // Create a material matrix
  matrix = new MaterialMatrix(sliceVerticalBoundingBoxStart,
                              sliceVerticalBoundingBoxEnd,
                              proc.stepy,
                              stepz);
  debugf("  slice bounding box after matrix creation (%.2f,%.2f) to (%.2f,%.2f) and steps %.2f and %.2f",
         sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
         sliceVerticalBoundingBoxStart.x + proc.stepy * matrix->xIndexSize,
         sliceVerticalBoundingBoxStart.y + stepz * matrix->yIndexSize,
         proc.stepy, stepz);
  
  // Create an image base of the bounding size
  svg = proc.createSvg(fileName,sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd,sliceDirection);

  debugf("  slice bounding box after svg creation (%.2f,%.2f) to (%.2f,%.2f) and steps %.2f and %.2f",
         sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
         sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y,
         proc.stepy, stepz);
  
  // Process the actual cross section
  drawCrossSection(scene);  
  debugf("  slice bounding box after drawing (%.2f,%.2f) to (%.2f,%.2f) and steps %.2f and %.2f",
         sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
         sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y,
         proc.stepy, stepz);

  // Now there's a matrix filled with a flag for each coordinate,
  // whether there was material or not. And small holes have been filled per above.
  // Draw the output based on all this.
  proc.matrixToSvg(matrix,svg,
                   sliceVerticalBoundingBoxStart.x,
                   sliceVerticalBoundingBoxStart.y,
                   proc.stepx,
                   stepz);

  // Main result (plan view) is done, flush the image output
  deleteSvg();

  // Done
  return(1);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Line equations and iterators ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////////
// Drawing a cross section ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
ProcessorCrossSection::drawCrossSection(const aiScene* scene) {

  // Sanity checks
  assert(scene != 0);

  // Actual matrix calculations
  drawCrossSectionNode(scene,scene->mRootNode);
}

void
ProcessorCrossSection::drawCrossSectionNode(const aiScene* scene,
                                            const aiNode* node) {
  assert(scene != 0);
  assert(node != 0);
  if (!node->mTransformation.IsIdentity()) {
    errf("Cannot handle transformations yet");
    exit(1);
  }
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    drawCrossSectionMesh(scene,scene->mMeshes[node->mMeshes[j]]);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    drawCrossSectionNode(scene,node->mChildren[i]);
  }
}

void
ProcessorCrossSection::drawCrossSectionMesh(const aiScene* scene,
                                            const aiMesh* mesh) {
  assert(scene != 0);
  assert(mesh != 0);
  struct ProcessorCrossSectionLineIterator iter;
  lineIteratorInit(iter);
  for (; !lineIteratorDone(iter); lineIteratorNext(iter)) {
    unsigned int nFaces = 0;
    const aiFace** faces = 0;
    proc.indexed.getFaces(mesh,iter.x,iter.y,&nFaces,&faces);
    if (nFaces > 0) {
      deepdebugf("drawCrossSectionMesh: got %u cross section faces from (%.2f,%.2f)", nFaces, iter.x, iter.y);
      for (unsigned int f = 0; f < nFaces; f++) {
        drawCrossSectionFace(scene,mesh,faces[f],
                             iter.x, iter.y);
      }
    }
  }
}

void
ProcessorCrossSection::drawCrossSectionFace(const aiScene* scene,
                                            const aiMesh* mesh,
                                            const aiFace* face,
                                            outlinerhighprecisionreal x,
                                            outlinerhighprecisionreal y) {
  assert(scene != 0);
  assert(mesh != 0);
  assert(face != 0);
  assert(matrix != 0);
  aiVector3D a3;
  aiVector3D b3;
  aiVector3D c3;
  proc.faceGetVertices3D(mesh,face,a3,b3,c3);
  for  (outlinerhighprecisionreal z = sliceVerticalBoundingBoxStart.y;
        z <= sliceVerticalBoundingBoxEnd.y;
        z += stepz) {
    HighPrecisionVector3D point(x,y,z);
    HighPrecisionVector3D stepboundingbox(x+lineStepX,y+lineStepY,z+stepz);
    if (OutlinerMath::boundingBoxIntersectsTriangle3D(a3,b3,c3,point,stepboundingbox)) {
      deepdebugf("cross section draw xyz match at (%.2f,%.2f,%.2f)",x,y,z);
      unsigned int xIndex = coordinateXYToImageXIndex(x,y);
      unsigned int yIndex = coordinateZToImageYIndex(z);
      infof("      set matrix %u,%u", xIndex, yIndex);
      matrix->setMaterialMatrix(xIndex,yIndex);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Cross section coordinates calculation //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

unsigned int
ProcessorCrossSection::coordinateXYToImageXIndex(outlinerhighprecisionreal x,
                                                 outlinerhighprecisionreal y) {
  assert(matrix != 0);
  outlinerhighprecisionreal thisXDifference = x - lineStart.x;
  outlinerhighprecisionreal thisYDifference = y - lineStart.y;
  outlinerhighprecisionreal thisLineLength = sqrt(thisXDifference*thisXDifference + thisYDifference*thisYDifference);
  unsigned int result = (unsigned int)((thisLineLength * (matrix->xIndexSize + 1)) / lineLength);
  assert(result < matrix->xIndexSize);
  return(result);
}
 
unsigned int
ProcessorCrossSection::coordinateZToImageYIndex(outlinerhighprecisionreal z) {
  assert(matrix != 0);
  assert(z >= sliceVerticalBoundingBoxStart.y);
  assert(z <= sliceVerticalBoundingBoxEnd.y);
  outlinerhighprecisionreal diff =
    (z - sliceVerticalBoundingBoxStart.y) /
    (sliceVerticalBoundingBoxEnd.y - sliceVerticalBoundingBoxStart.y);
  unsigned int result = (unsigned int)(diff * (matrix->yIndexSize-1));
  infof("    cross section coordinate z %.2f (diff %.4f) to %u (yIndexSize %u)",
        z, diff, result, matrix->yIndexSize);
  assert(result < matrix->yIndexSize);
  return(result);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Cross section bounding box calculation /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

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
  aiVector3D a3;
  aiVector3D b3;
  aiVector3D c3;
  proc.faceGetVertices3D(mesh,face,a3,b3,c3);
  aiVector2D a(DirectionOperations::outputx(proc.direction,a3),DirectionOperations::outputy(proc.direction,a3));
  aiVector2D b(DirectionOperations::outputx(proc.direction,b3),DirectionOperations::outputy(proc.direction,b3));
  aiVector2D c(DirectionOperations::outputx(proc.direction,c3),DirectionOperations::outputy(proc.direction,c3));
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
    deepdeepdebugf("cross section direction %s and %s",
                   DirectionOperations::toString(proc.direction),
                   DirectionOperations::toString(sliceDirection));
    aiVector2D aSlice(DirectionOperations::outputy(proc.direction,a3),DirectionOperations::outputz(proc.direction,a3));
    aiVector2D bSlice(DirectionOperations::outputy(proc.direction,b3),DirectionOperations::outputz(proc.direction,b3));
    aiVector2D cSlice(DirectionOperations::outputy(proc.direction,c3),DirectionOperations::outputz(proc.direction,c3));
    HighPrecisionVector2D faceBoundingBoxStart;
    HighPrecisionVector2D faceBoundingBoxEnd;
    OutlinerMath::triangleBoundingBox2D(aSlice,bSlice,cSlice,
                                        faceBoundingBoxStart,
                                        faceBoundingBoxEnd);
    deepdebugf("    adding a face to cross section bounding box, triangle box (%.2f,%.2f) - (%.2f,%.2f)",
               faceBoundingBoxStart.x, faceBoundingBoxStart.y,
               faceBoundingBoxEnd.x, faceBoundingBoxEnd.y);
    //HighPrecisionVector2D resultBoundingBoxStart;
    //HighPrecisionVector2D resultBoundingBoxEnd;
    //OutlinerMath::boundingBoxIntersection(point,
    //                                      stepboundingbox,
    //                                      faceBoundingBoxStart,
    //                                      faceBoundingBoxEnd,
    //                                      resultBoundingBoxStart,
    //                                      resultBoundingBoxEnd);
    if (!set) {
      sliceVerticalBoundingBoxStart = faceBoundingBoxStart;
      sliceVerticalBoundingBoxEnd = faceBoundingBoxEnd;
      deepdebugf("setting initial cross section bounding box at (%.2f,%.2f) "
                 "to (%.2f,%.2f)-(%.2f,%.2f)",
                 x, y,
                 sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
                 sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y);
      set = 1;
    } else {
      HighPrecisionVector2D newBoundingBoxStart;
      HighPrecisionVector2D newBoundingBoxEnd;
      OutlinerMath::boundingBoxUnion(faceBoundingBoxStart,
                                     faceBoundingBoxEnd,
                                     sliceVerticalBoundingBoxStart,
                                     sliceVerticalBoundingBoxEnd,
                                     newBoundingBoxStart,
                                     newBoundingBoxEnd);
      deepdebugf("testing new cross section bounding box "
                 "for face (%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f) "
                 "at (%.2f,%.2f) as (%.2f,%.2f)-(%.2f,%.2f)",
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
                   DirectionOperations::toString(proc.direction),
                   a.x, a.y, b.x, b.y, c.x, c.y,
                   point.x, point.y,
                   stepboundingbox.x,
                   stepboundingbox.y);
  }
}

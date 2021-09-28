  
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
  debugf("calculate line equation for (%.2f,%.2f)-(%.2f,%.2f)",
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
  sliceVerticalBoundingBox(scene,sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
  svg = proc.createSvg(fileName,sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd,sliceDirection);
  return(1);
}

void
ProcessorCrossSection::sliceVerticalBoundingBox(const aiScene* scene,
                                                HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                                HighPrecisionVector2D& sliceVerticalBoundingBoxEnd) {
  assert(scene != 0);
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
  struct ProcessorCrossSectionLineIterator iter;
  lineIteratorInit(iter);
  for (; !lineIteratorDone(iter); lineIteratorNext(iter)) {
    unsigned int nFaces = 0;
    const aiFace** faces = 0;
    proc.indexed.getFaces(mesh,iter.x,iter.y,&nFaces,&faces);
    for (unsigned int f = 0; f < nFaces; f++) {
      sliceVerticalBoundingBoxFace(scene,mesh,faces[f],
                                   set,
                                   sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
    }
  }
}

void
ProcessorCrossSection::sliceVerticalBoundingBoxFace(const aiScene* scene,
                                                    const aiMesh* mesh,
                                                    const aiFace* face,
                                                    bool& set,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxEnd) {
}

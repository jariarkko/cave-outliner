///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//
//      CCC    AAA   V     V EEEEE       OOO   UU   UU TTTTTT LL    II NN   NN EEEEE RRRRR
//    CC   CC AA  AA  V   V  EE        OO   OO UU   UU   TT   LL    II NNN  NN EE    RR  RR
//    CC      AA  AA  V   V  EEE       OO   OO UU   UU   TT   LL    II NN N NN EEE   RRRRR
//    CC   CC AAAAAA   V V   EE        OO   OO UU   UU   TT   LL    II NN  NNN EE    RR R
//      CCc   AA  AA    V    EEEEE       OOO    UUUUU    TT   LLLLL II NN   NN EEEEE RR  R
//
//    CAVE OUTLINER -- Cave 3D model processing software
//
//    Copyright (C) 2021 by Jari Arkko -- See LICENSE.txt for license information.
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

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
#include "outlinerdirection.hh"
#include "outlinermath.hh"
#include "outlinerdebug.hh"
#include "outlinermaterialmatrix3d.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Material matrix maintenance ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

MaterialMatrix3D::MaterialMatrix3D(const OutlinerBox3D& boundingBoxIn,
                                   const outlinerreal stepxIn,
                                   const outlinerreal stepyIn,
                                   const outlinerreal stepzIn) :
  boundingBox(boundingBoxIn),
  xIndexSize(MaterialMatrix2D::calculateSize(boundingBox.start.x,boundingBox.end.x,stepxIn)),
  yIndexSize(MaterialMatrix2D::calculateSize(boundingBox.start.y,boundingBox.end.y,stepyIn)),
  zIndexSize(MaterialMatrix2D::calculateSize(boundingBox.start.z,boundingBox.end.z, stepzIn)),
  stepx(stepxIn),
  stepy(stepyIn),
  stepz(stepzIn),
  verticalMatrixes(new VerticalMatrix [xIndexSize]) {
  if (verticalMatrixes == 0) {
    errf("Cannot allocate %u vertical matrixes", xIndexSize);
    exit(1);
  }
  memset(verticalMatrixes,0,xIndexSize*sizeof(VerticalMatrix));
}

MaterialMatrix3D::~MaterialMatrix3D() {
}
  
void
MaterialMatrix3D::setMaterialMatrix(const unsigned int xIndex,
                                    const unsigned int yIndex,
                                    const unsigned int zIndex) {
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(zIndex < zIndexSize);
  if (verticalMatrixes[xIndex].matrix == 0) {
    OutlinerBox2D sliceBox(boundingBox.start.y,boundingBox.start.z,
                           boundingBox.end.y,boundingBox.end.z);
    verticalMatrixes[xIndex].matrixBoundingBox = sliceBox;
    verticalMatrixes[xIndex].yIndexOffset = 0;
    verticalMatrixes[xIndex].matrix = new MaterialMatrix2D(sliceBox,stepy,stepz);
    if (verticalMatrixes[xIndex].matrix == 0) {
      errf("Cannot allocate a vertical matrix");
      exit(1);
    }
  }
  assert(yIndex >= verticalMatrixes[xIndex].yIndexOffset);
  unsigned int sliceYIndex = yIndex - verticalMatrixes[xIndex].yIndexOffset;
  assert(sliceYIndex < verticalMatrixes[xIndex].matrix->xIndexSize);
  verticalMatrixes[xIndex].matrix->setMaterialMatrix(yIndex,zIndex);
}

void
MaterialMatrix3D::setMaterialMatrixSlice(const unsigned int xIndex,
                                         const OutlinerBox2D& sliceBoundingBox,
                                         MaterialMatrix2D* sliceMatrix) {
  assert(xIndex < xIndexSize);
  assert(verticalMatrixes[xIndex].matrix == 0);
  outlinerreal yDiff = sliceBoundingBox.start.y - boundingBox.start.y;
  assert(yDiff >= 0.0);
  verticalMatrixes[xIndex].yIndexOffset = yDiff / stepy;
  verticalMatrixes[xIndex].matrixBoundingBox = sliceBoundingBox;
  verticalMatrixes[xIndex].matrix = sliceMatrix;
}

bool
MaterialMatrix3D::getMaterialMatrix(const unsigned int xIndex,
                                    const unsigned int yIndex,
                                    const unsigned int zIndex) const {
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(zIndex < zIndexSize);
  MaterialMatrix2D* sliceMatrix = verticalMatrixes[xIndex].matrix;
  if (sliceMatrix == 0) {
    return(0);
  } else if (yIndex < verticalMatrixes[xIndex].yIndexOffset) {
    return(0);
  } else {
    unsigned int sliceYIndex = yIndex - verticalMatrixes[xIndex].yIndexOffset;
    if (sliceYIndex >= sliceMatrix->xIndexSize) {
      return(0);
    } else {
      return(sliceMatrix->getMaterialMatrix(yIndex,zIndex));
    }
  }
}

unsigned int
MaterialMatrix3D::count(void) const {
  unsigned int theCount = 0;
  for (unsigned int xIndex = 0; xIndex < xIndexSize; xIndex++) {
    const VerticalMatrix& vertical = verticalMatrixes[xIndex];
    if (vertical.matrix != 0) {
      theCount += vertical.matrix->count();
    }
  }
  return(theCount);
}

outlinerreal
MaterialMatrix3D::filledPercentage(unsigned int& memory,
                                   unsigned int& theoretical) const {
  unsigned int fullPixels = xIndexSize * yIndexSize * zIndexSize;
  infof("  filled percentage size %u x %u x %u", xIndexSize, yIndexSize, zIndexSize);
  theoretical = fullPixels / 8;
  unsigned int actualPixels = 0;
  for (unsigned int xIndex = 0; xIndex < xIndexSize; xIndex++) {
    const VerticalMatrix& vertical = verticalMatrixes[xIndex];
    if (vertical.matrix != 0) {
      actualPixels += vertical.matrix->xIndexSize * vertical.matrix->yIndexSize;
      debugf("    filled percentage at x %u size %u x %u",
             xIndex,
             vertical.matrix->xIndexSize, vertical.matrix->yIndexSize);
    }
  }
  memory = actualPixels / 8;
  debugf("  filled percentage pixels %u %u mems %u %u",
         actualPixels, fullPixels,
         memory, theoretical);
  return((100.0 * ((outlinerreal)actualPixels)) / ((outlinerreal)fullPixels));
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Unit tests for this module /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
MaterialMatrix3D::test(void) {

  infof("material matrix test running");
  infof("material matrix test ok");
}



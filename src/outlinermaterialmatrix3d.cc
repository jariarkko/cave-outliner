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
  zIndexSize(MaterialMatrix2D::calculateSize(boundingBox.start.z,boundingBox.end.z,stepzIn)),
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
    verticalMatrixes[xIndex].zIndexOffset = 0;
    verticalMatrixes[xIndex].matrix = new MaterialMatrix2D(sliceBox,stepy,stepz);
    if (verticalMatrixes[xIndex].matrix == 0) {
      errf("Cannot allocate a vertical matrix");
      exit(1);
    }
  }
  assert(yIndex >= verticalMatrixes[xIndex].yIndexOffset);
  assert(zIndex >= verticalMatrixes[xIndex].zIndexOffset);
  unsigned int sliceYIndex = yIndex - verticalMatrixes[xIndex].yIndexOffset;
  unsigned int sliceZIndex = zIndex - verticalMatrixes[xIndex].zIndexOffset;
  assert(sliceYIndex < verticalMatrixes[xIndex].matrix->xIndexSize);
  assert(sliceZIndex < verticalMatrixes[xIndex].matrix->yIndexSize);
  verticalMatrixes[xIndex].matrix->setMaterialMatrix(yIndex,zIndex);
}

void
MaterialMatrix3D::setMaterialMatrixSlice(const unsigned int xIndex,
                                         const OutlinerBox2D& sliceBoundingBox,
                                         MaterialMatrix2D* sliceMatrix) {
  assert(xIndex < xIndexSize);
  assert(verticalMatrixes[xIndex].matrix == 0);
  outlinerreal yDiff = sliceBoundingBox.start.x - boundingBox.start.y;
  outlinerreal zDiff = sliceBoundingBox.start.y - boundingBox.start.z;
  assert(yDiff >= 0.0);
  assert(zDiff >= 0.0);
  verticalMatrixes[xIndex].yIndexOffset = yDiff / stepy;
  verticalMatrixes[xIndex].zIndexOffset = zDiff / stepz;
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
  } else if (zIndex < verticalMatrixes[xIndex].zIndexOffset) {
    return(0);
  } else {
    unsigned int sliceYIndex = yIndex - verticalMatrixes[xIndex].yIndexOffset;
    unsigned int sliceZIndex = zIndex - verticalMatrixes[xIndex].zIndexOffset;
    infof("      3d getmm %u,%u,%u slice yz %u %u (offsets %u %u maxes %u %u)",
          xIndex,yIndex,zIndex,
          sliceYIndex, sliceZIndex,
          verticalMatrixes[xIndex].yIndexOffset,
          verticalMatrixes[xIndex].zIndexOffset,
          sliceMatrix->xIndexSize,
          sliceMatrix->yIndexSize);
    if (sliceYIndex >= sliceMatrix->xIndexSize) {
      return(0);
    } else if (sliceZIndex >= sliceMatrix->yIndexSize) {
      return(0);
    } else {
      return(sliceMatrix->getMaterialMatrix(sliceYIndex,sliceZIndex));
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
// Model processing -- coordinate conversions /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

unsigned int
MaterialMatrix3D::coordinateXToIndex(outlinerreal x) const {
  outlinerreal xStart = boundingBox.start.x;
  outlinerreal xEnd = boundingBox.end.x;
  assert(outlinergeepsilon(x,xStart));
  assert(outlinerleepsilon(x,xEnd));
  return((x - xStart)/stepx);
}

unsigned int
MaterialMatrix3D::coordinateYToIndex(outlinerreal y) const {
  outlinerreal yStart = boundingBox.start.y;
  outlinerreal yEnd = boundingBox.end.y;
  assert(outlinergeepsilon(y,yStart));
  assert(outlinerleepsilon(y,yEnd));
  return((y - yStart)/stepy);
}

unsigned int
MaterialMatrix3D::coordinateZToIndex(outlinerreal z) const {
  outlinerreal zStart = boundingBox.start.z;
  outlinerreal zEnd = boundingBox.end.z;
  assert(outlinergeepsilon(z,zStart));
  assert(outlinerleepsilon(z,zEnd));
  return((z - zStart)/stepz);
}

outlinerreal
MaterialMatrix3D::indexToCoordinateX(unsigned int xIndex) const {
  outlinerreal xStart = boundingBox.start.x;
  return(xStart + stepx * xIndex);
}

outlinerreal
MaterialMatrix3D::indexToCoordinateY(unsigned int yIndex) const {
  outlinerreal yStart = boundingBox.start.y;
  return(yStart + stepy * yIndex);
}

outlinerreal
MaterialMatrix3D::indexToCoordinateZ(unsigned int zIndex) const {
  outlinerreal zStart = boundingBox.start.z;
  return(zStart + stepz * zIndex);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Unit tests for this module /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
MaterialMatrix3D::test(void) {

  infof("material matrix test running");
  infof("material matrix test ok");
}



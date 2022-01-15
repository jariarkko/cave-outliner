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
                                   const outlinerreal stepzIn,
                                   const unsigned int multiplier) :
  boundingBox(boundingBoxIn),
  xIndexSize(MaterialMatrix2D::calculateSize(boundingBox.start.x,boundingBox.end.x,stepxIn,multiplier,"matrix3 x")),
  yIndexSize(MaterialMatrix2D::calculateSize(boundingBox.start.y,boundingBox.end.y,stepyIn,multiplier,"matrix3 y")),
  zIndexSize(MaterialMatrix2D::calculateSize(boundingBox.start.z,boundingBox.end.z,stepzIn,multiplier,"matrix3 z")),
  stepx(stepxIn*multiplier),
  stepy(stepyIn*multiplier),
  stepz(stepzIn*multiplier),
  verticalMatrixes(new VerticalMatrix [xIndexSize]) {
  if (verticalMatrixes == 0) {
    errf("Cannot allocate %u vertical matrixes", xIndexSize);
    return;
  }
}

MaterialMatrix3D::~MaterialMatrix3D() {
  infof("MaterialMatrix3D::~MaterialMatrix3D");
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
      return;
    }
  }
  assert(yIndex >= verticalMatrixes[xIndex].yIndexOffset);
  assert(zIndex >= verticalMatrixes[xIndex].zIndexOffset);
  unsigned int sliceYIndex = yIndex - verticalMatrixes[xIndex].yIndexOffset;
  unsigned int sliceZIndex = zIndex - verticalMatrixes[xIndex].zIndexOffset;
  assert(sliceYIndex < verticalMatrixes[xIndex].matrix->xIndexSize);
  assert(sliceZIndex < verticalMatrixes[xIndex].matrix->yIndexSize);
  verticalMatrixes[xIndex].matrix->setMaterialMatrix(sliceYIndex,sliceZIndex);
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
  debugf("set slice yoffset %u (%.2f..%.2f) zoffset %u (%2.f..%.2f)",
         verticalMatrixes[xIndex].yIndexOffset,
         verticalMatrixes[xIndex].matrixBoundingBox.start.x, verticalMatrixes[xIndex].matrixBoundingBox.end.x,
         verticalMatrixes[xIndex].zIndexOffset,
         verticalMatrixes[xIndex].matrixBoundingBox.start.y, verticalMatrixes[xIndex].matrixBoundingBox.end.y);
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
    deepdebugf("      3d getmm %u,%u,%u slice yz %u %u (offsets %u %u maxes %u %u)",
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

  // Simple matrix creation and deletion
  {
    OutlinerBox3D bb1(0,0,0,10,10,10);
    MaterialMatrix3D matrix1(bb1,1,1,1,1);
    for (unsigned int i = 0; i < 10; i++) matrix1.setMaterialMatrix(i,i,i);
    // Verify contents
    for (unsigned int x = 0; x < 10; x++) {
      for (unsigned int y = 0; y < 10; y++) {
        for (unsigned int z = 0; z < 10; z++) {
          bool ans = matrix1.getMaterialMatrix(x,y,z);
          assert((ans == 0 && !(x == y && y == z)) ||
                 (ans == 1 && x == y && y == z));
        }
      }
    }
  }

  // Make a matrix of 2D matrixes (full size)
  {
    OutlinerBox3D bb2(0,0,0,100,100,100);
    MaterialMatrix3D matrix2(bb2,1,1,1,1);
    for (unsigned int i = 0; i < 100; i++) {
      OutlinerBox2D sliceBoundingBox(0,0,100,100);
      MaterialMatrix2D* sliceMatrix = new MaterialMatrix2D(sliceBoundingBox,1,1);
      sliceMatrix->setMaterialMatrix(i,i);
      matrix2.setMaterialMatrixSlice(i,
                                     sliceBoundingBox,
                                     sliceMatrix);
    }
    // Verify contents
    for (unsigned int x = 0; x < 100; x++) {
      for (unsigned int y = 0; y < 100; y++) {
        for (unsigned int z = 0; z < 100; z++) {
          bool ans = matrix2.getMaterialMatrix(x,y,z);
          debugf("  verify %u,%u,%u = %u", x, y, z, ans);
          assert((ans == 0 && !(x == y && y == z)) ||
                 (ans == 1 && x == y && y == z));
        }
      }
    }
  }
  
  // Make a matrix of 2D matrixes (smaller size 2D matrixes)
  {
    OutlinerBox3D bb3(0,0,0,100,100,100);
    MaterialMatrix3D matrix3(bb3,1,1,1,1);
    for (unsigned int i = 0; i < 100; i++) {
      OutlinerBox2D sliceBoundingBoxA(0,0,50,50);
      OutlinerBox2D sliceBoundingBoxB(50,50,100,100);
      MaterialMatrix2D* sliceMatrix;
      if (i < 50) {
        sliceMatrix = new MaterialMatrix2D(sliceBoundingBoxA,1,1);
        sliceMatrix->setMaterialMatrix(i,i);
        matrix3.setMaterialMatrixSlice(i,
                                       sliceBoundingBoxA,
                                       sliceMatrix);
      } else {
        sliceMatrix = new MaterialMatrix2D(sliceBoundingBoxB,1,1);
        sliceMatrix->setMaterialMatrix(i-50,i-50);
        matrix3.setMaterialMatrixSlice(i,
                                       sliceBoundingBoxB,
                                       sliceMatrix);
      }
    }
    // Verify contents
    for (unsigned int x = 0; x < 100; x++) {
      for (unsigned int y = 0; y < 100; y++) {
        for (unsigned int z = 0; z < 100; z++) {
          bool ans = matrix3.getMaterialMatrix(x,y,z);
          debugf("  verify %u,%u,%u = %u", x, y, z, ans);
          assert((ans == 0 && !(x == y && y == z)) ||
                 (ans == 1 && x == y && y == z));
        }
      }
    }

  }
  
  // Make a matrix of 2D matrixes (smaller size 2D matrixes,
  // non-uniorm content)
  {
    OutlinerBox3D bb3(0,0,0,100,100,100);
    MaterialMatrix3D matrix3(bb3,1,1,1,1);
    for (unsigned int i = 0; i < 100; i++) {
      OutlinerBox2D sliceBoundingBoxA(0,0,50,50);
      OutlinerBox2D sliceBoundingBoxB(50,50,100,100);
      MaterialMatrix2D* sliceMatrix;
      if (i < 50) {
        sliceMatrix = new MaterialMatrix2D(sliceBoundingBoxA,1,1);
        matrix3.setMaterialMatrixSlice(i,
                                       sliceBoundingBoxA,
                                       sliceMatrix);
      } else {
        sliceMatrix = new MaterialMatrix2D(sliceBoundingBoxB,1,1);
        matrix3.setMaterialMatrixSlice(i,
                                       sliceBoundingBoxB,
                                       sliceMatrix);
      }
    }
    
    // Add some non-uniform content
    matrix3.setMaterialMatrix(1,2,3);
    matrix3.setMaterialMatrix(4,5,6);
    matrix3.setMaterialMatrix(90,55,51);
    matrix3.setMaterialMatrix(0,10,10);
    matrix3.setMaterialMatrix(60,61,59);
    
    // Verify contents
    infof("non-uniform verify");
    for (unsigned int x = 0; x < 100; x++) {
      for (unsigned int y = 0; y < 100; y++) {
        for (unsigned int z = 0; z < 100; z++) {
          bool ans = matrix3.getMaterialMatrix(x,y,z);
          deepdebugf("  verify %u,%u,%u = %u", x, y, z, ans);
          if (x == 1 && y == 2 && z == 3) assert(ans);
          else if (x == 4 && y == 5 && z == 6) assert(ans);
          else if (x == 90 && y == 55 && z == 51) assert(ans);
          else if (x == 0 && y == 10 && z == 10) assert(ans);
          else if (x == 60 && y == 61 && z == 59) assert(ans);
          else assert(!ans);
        }
      }
    }

  }
  
  infof("material matrix test ok");
}

///////////////////////////////////////////////////////////////////////////////////////////////
// VerticalMatrix objects /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
        
VerticalMatrix::VerticalMatrix() :
  yIndexOffset(0), zIndexOffset(0), matrix(0) {
}

VerticalMatrix::~VerticalMatrix() {
  infof("VerticalMatrix::~VerticalMatrix");
}


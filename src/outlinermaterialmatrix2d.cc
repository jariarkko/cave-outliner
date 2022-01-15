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
#include "outlinermaterialmatrix2d.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Material matrix maintenance ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

MaterialMatrix2D::MaterialMatrix2D(const OutlinerBox2D& boundingBoxIn,
                                   const outlinerreal stepxIn,
                                   const outlinerreal stepyIn) :
  boundingBox(boundingBoxIn),
  xIndexSize(calculateSize(boundingBox.start.x,boundingBox.end.x,stepxIn,1,"matrix2 x")),
  yIndexSize(calculateSize(boundingBox.start.y,boundingBox.end.y,stepyIn,1,"matrix2 y")),
  stepx(stepxIn),
  stepy(stepyIn),
  nBits(xIndexSize * yIndexSize),
  nChars((nBits / 8) + 1),
  bitMatrix(new unsigned char [nChars]) {
  debugf("yIndexSize %u from %.8f - %.8f / %.8f + 2",
        yIndexSize, boundingBox.start.y, boundingBox.end.y, stepy);
  debugf("sub %.8f div %.8f",
        (boundingBox.end.y - boundingBox.start.y),
        ((boundingBox.end.y - boundingBox.start.y) / stepy));
  debugf("material matrix %ux%u from %.2f..%.2f and %.2f..%.2f",
        xIndexSize, yIndexSize,
        boundingBox.start.x, boundingBox.end.x,
        boundingBox.start.y, boundingBox.end.y);
  if (bitMatrix == 0) {
    errf("Cannot allocate bit matrix for %u bytes", nChars);
    return;
  }
  memset(bitMatrix,0,nChars);
  debugf("created a matrix of %u x %u, nBits %u nChars %u", xIndexSize, yIndexSize, nBits, nChars);
}

MaterialMatrix2D::~MaterialMatrix2D() {
  if (bitMatrix != 0) {
    delete [] bitMatrix;
  }
}

unsigned int
MaterialMatrix2D::calculateSize(const outlinerreal from,
                                const outlinerreal to,
                                const outlinerreal step,
                                const unsigned int multiplier,
                                const char* what) {
  assert(from <= to);
  assert(step > 0.0);
  outlinerreal divResult = ((to - from) / step);
  outlinerreal ceilResult = ceil(divResult);
  outlinerreal mulResult = ceilResult / multiplier;
  outlinerreal ceil2Result = ceil(mulResult);
  outlinerreal realResult = ceil2Result + 2;
  unsigned int intResult = realResult;
  debugf("calculate %s size %5.2f..%5.2f in %5.2f x %u: %6.2f => %6.2f => %6.2f => %6.2f% => %6.2f=%u",
         what,
         from, to, step, multiplier,
         divResult, ceilResult, mulResult, ceil2Result, realResult,
         intResult);
  return(intResult);
}

void
MaterialMatrix2D::setMaterialMatrix(const unsigned int xIndex,
                                    const unsigned int yIndex) {
  if (xIndex >= xIndexSize || yIndex >= yIndexSize)
    debugf("  setMaterialMatrix(%u/%u,%u/%u)",
           xIndex, xIndexSize, yIndex, yIndexSize);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  unsigned int index = xIndex * yIndexSize + yIndex;
  unsigned int charpart = index / 8;
  unsigned int bitpart = index % 8;
  unsigned char bitMask = (1 << bitpart);
  //deepdebugf("      setting material matrix %u/%u (%u/%u,%u/%u) elem %u/%u with mask %x",
  //           index, nBits, xIndex, xIndexSize, yIndex, yIndexSize, charpart, nChars, bitMask);
  assert(index < nBits);
  assert(charpart < nChars);
  assert(bitpart < 8);
  bitMatrix[charpart] |= bitMask;
}

void
MaterialMatrix2D::unsetMaterialMatrix(const unsigned int xIndex,
                                      const unsigned int yIndex) {
  if (xIndex >= xIndexSize || yIndex >= yIndexSize)
    debugf("  setMaterialMatrix(%u/%u,%u/%u)",
           xIndex, xIndexSize, yIndex, yIndexSize);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  unsigned int index = xIndex * yIndexSize + yIndex;
  unsigned int charpart = index / 8;
  unsigned int bitpart = index % 8;
  unsigned char bitMask = (1 << bitpart);
  //deepdebugf("      setting material matrix %u/%u (%u/%u,%u/%u) elem %u/%u with mask %x",
  //           index, nBits, xIndex, xIndexSize, yIndex, yIndexSize, charpart, nChars, bitMask);
  assert(index < nBits);
  assert(charpart < nChars);
  assert(bitpart < 8);
  bitMatrix[charpart] &= ~bitMask;
}

bool
MaterialMatrix2D::getMaterialMatrix(const unsigned int xIndex,
                                    const unsigned int yIndex) const {
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(bitMatrix != 0);
  unsigned int index = xIndex * yIndexSize + yIndex;
  unsigned int charpart = index / 8;
  unsigned int bitpart = index % 8;
  unsigned char thechar = bitMatrix[charpart];
  unsigned char bitMask = (1 << bitpart);
  deepdeepdebugf("       getting material matrix %u (%u,%u) elem %u value %x with mask %x",
                 index, xIndex, yIndex, charpart, thechar, bitMask);
  assert(index < nBits);
  assert(charpart < nChars);
  assert(bitpart < 8);
  if ((thechar & bitMask) != 0) return(1);
  else return(0);
}

bool
MaterialMatrix2D::getMaterialMatrixRange(const unsigned int xIndexStart,
                                         const unsigned int yIndexStart,
                                         const unsigned int xIndexEnd,
                                         const unsigned int yIndexEnd) const {
  deepdebugf("      multi-get-mm %u..%u x %u..%u", xIndexStart, xIndexEnd, yIndexStart, yIndexEnd);
  assert(xIndexStart < xIndexSize);
  assert(xIndexEnd < xIndexSize);
  assert(yIndexStart < yIndexSize);
  assert(yIndexEnd < yIndexSize);
  for (unsigned int xIndex = xIndexStart;
       xIndex <= xIndexEnd && xIndex < xIndexSize;
       xIndex++) {
    for (unsigned int yIndex = yIndexStart;
         yIndex <= yIndexEnd && yIndex < yIndexSize;
         yIndex++) {
      assert(xIndex < xIndexSize);
      assert(yIndex < yIndexSize);
      if (getMaterialMatrix(xIndex,yIndex)) {
        deepdebugf("      multi-get-mm found");
        return(1);
      }
    }
  }
  
  // Not found
  deepdebugf("      multi-get-mm not found");
  return(0);
}

bool
MaterialMatrix2D::getMaterialYBounds(const unsigned int xIndex,
                                     unsigned int& yIndexFrom,
                                     unsigned int& yIndexTo) const {
  assert(xIndex < xIndexSize);
  bool found = 0;
  for (unsigned int yIndex = 0; yIndex < yIndexSize; yIndex++) {
    if (getMaterialMatrix(xIndex,yIndex)) {
      if (!found) {
        yIndexFrom = yIndex;
        yIndexTo = yIndex;
        found = 1;
      } else {
        yIndexTo = yIndex;
      }
    }
  }
  return(found);
}

unsigned int
MaterialMatrix2D::count(void) const {
  unsigned int theCount = 0;
  for (unsigned int i = 0; i < nChars; i++) {
    assert(i < nChars);
    unsigned char theChar = bitMatrix[i];
    if (theChar != 0) {
      deepdebugf("      found non-zero char %x in index %u",
                 theChar, i, nChars);
    }
    while (theChar != 0) {
      if ((theChar & 1) != 0) theCount++;
      theChar >>= 1;
    }
  }
  return(theCount);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Model processing -- coordinate conversions /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

unsigned int
MaterialMatrix2D::coordinateXToIndex(outlinerreal x) const {
  outlinerreal xStart = boundingBox.start.x;
  outlinerreal xEnd = boundingBox.end.x;
  assert(outlinergeepsilon(x,xStart));
  assert(outlinerleepsilon(x,xEnd));
  return((x - xStart)/stepx);
}

unsigned int
MaterialMatrix2D::coordinateYToIndex(outlinerreal y) const {
  outlinerreal yStart = boundingBox.start.y;
  outlinerreal yEnd = boundingBox.end.y;
  assert(outlinergeepsilon(y,yStart));
  assert(outlinerleepsilon(y,yEnd));
  return((y - yStart)/stepy);
}

outlinerreal
MaterialMatrix2D::indexToCoordinateX(unsigned int xIndex) const {
  outlinerreal xStart = boundingBox.start.x;
  return(xStart + stepx * xIndex);
}

outlinerreal
MaterialMatrix2D::indexToCoordinateY(unsigned int yIndex) const {
  outlinerreal yStart = boundingBox.start.y;
  return(yStart + stepy * yIndex);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Unit tests for this module /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
MaterialMatrix2D::test(void) {

  infof("material matrix test running");
  
  // Simple test
  {
    OutlinerVector2D boundingBoxStart(0,0);
    OutlinerVector2D boundingBoxEnd(10,10);
    OutlinerBox2D boundingBox(boundingBoxStart,boundingBoxEnd);
    float stepx = 1.0;
    float stepy = 1.0;
    MaterialMatrix2D test1(boundingBox,stepx,stepy);
    unsigned int xSize = test1.xIndexSize;
    unsigned int ySize = test1.yIndexSize;
    debugf("test1 sizes %u and %u", xSize, ySize);
    unsigned int n = test1.count();
    debugf("test1 initial count = %u", n);
    assert(n == 0);
    test1.setMaterialMatrix(5,5);
    n = test1.count();
    assert(n == 1);
    test1.unsetMaterialMatrix(5,5);
    n = test1.count();
    assert(n == 0);
    test1.setMaterialMatrix(5,5);
    n = test1.count();
    assert(n == 1);
    test1.setMaterialMatrix(6,6);
    n = test1.count();
    assert(n == 2);
    test1.setMaterialMatrix(9,9);
    n = test1.count();
    assert(n == 3);
    test1.setMaterialMatrix(7,10);
    n = test1.count();
    assert(n == 4);
    test1.setMaterialMatrix(7,2);
    n = test1.count();
    assert(n == 5);
    for (unsigned int x = boundingBox.start.x; x <= boundingBox.end.x; x++) {
      for (unsigned int y = boundingBox.start.y; y <= boundingBox.end.y; y++) {
        bool ans =  test1.getMaterialMatrix(x,y);
        if (ans) {
          debugf("found bit in %u,%u", x, y);
          assert((x == 5 && y == 5) ||
                 (x == 6 && y == 6) ||
                 (x == 9 && y == 9) ||
                 (x == 7 && y == 2) ||
                 (x == 7 && y == 10));
        }
      }
    }
    unsigned int yFrom;
    unsigned int yTo;
    bool bans = test1.getMaterialYBounds(3,yFrom,yTo);
    assert(!bans);
    bans = test1.getMaterialYBounds(6,yFrom,yTo);
    assert(bans);
    assert(yFrom == 6 && yTo == 6);
    bans = test1.getMaterialYBounds(7,yFrom,yTo);
    assert(bans);
    assert(yFrom == 2 && yTo == 10);
  }
  
  // Large test
  {
    OutlinerVector2D boundingBoxStart(0,0);
    OutlinerVector2D boundingBoxEnd(1000,1000);
    OutlinerBox2D boundingBox(boundingBoxStart,boundingBoxEnd);
    float stepx = 0.1;
    float stepy = 0.1;
    MaterialMatrix2D test2(boundingBox,stepx,stepy);
    unsigned int xSize = test2.xIndexSize;
    unsigned int ySize = test2.yIndexSize;
    debugf("test2 sizes %u and %u", xSize, ySize);
    unsigned int n = test2.count();
    debugf("test2 initial count = %u", n);
    assert(n == 0);
    test2.setMaterialMatrix(5000,4900);
    n = test2.count();
    assert(n == 1);
    test2.setMaterialMatrix(9999,9999);
    n = test2.count();
    assert(n == 2);
  }

  infof("material matrix test ok");
}



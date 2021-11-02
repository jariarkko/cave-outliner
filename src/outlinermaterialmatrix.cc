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
#include "outlinermaterialmatrix.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Material matrix maintenance ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

MaterialMatrix::MaterialMatrix(OutlinerBox2D boundingbox,
                               outlinerreal stepx,
                               outlinerreal stepy) {
  xIndexSize = ((unsigned int)ceil(((boundingbox.end.x - boundingbox.start.x) / stepx))) + 2;
  yIndexSize = ((unsigned int)ceil(((boundingbox.end.y - boundingbox.start.y) / stepy))) + 2;
  debugf("yIndexSize %u from %.8f - %.8f / %.8f + 2",
        yIndexSize, boundingbox.start.y, boundingbox.end.y, stepy);
  debugf("sub %.8f div %.8f",
        (boundingbox.end.y - boundingbox.start.y),
        ((boundingbox.end.y - boundingbox.start.y) / stepy));
  debugf("material matrix %ux%u from %.2f..%.2f and %.2f..%.2f",
        xIndexSize, yIndexSize,
        boundingbox.start.x, boundingbox.end.x,
        boundingbox.start.y, boundingbox.end.y);
  nBits = xIndexSize * yIndexSize;
  nChars = (nBits / 8) + 1;
  bitMatrix = new unsigned char [nChars];
  if (bitMatrix == 0) {
    errf("Cannot allocate bit matrix for %u bytes", nChars);
    exit(1);
  }
  memset(bitMatrix,0,nChars);
  debugf("created a matrix of %u x %u, nBits %u nChars %u", xIndexSize, yIndexSize, nBits, nChars);
}

MaterialMatrix::~MaterialMatrix() {
  if (bitMatrix != 0) {
    delete bitMatrix;
  }
}
  
void
MaterialMatrix::setMaterialMatrix(unsigned int xIndex,
                                  unsigned int yIndex) {
  if (xIndex >= xIndexSize || yIndex >= yIndexSize)
    infof("  setMaterialMatrix(%u/%u,%u/%u)",
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

bool
MaterialMatrix::getMaterialMatrix(unsigned int xIndex,
                                  unsigned int yIndex) {
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

unsigned int
MaterialMatrix::count(void) {
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
// Unit tests for this module /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
MaterialMatrix::test(void) {

  infof("material matrix test running");
  
  // Simple test
  {
    OutlinerVector2D boundingBoxStart(0,0);
    OutlinerVector2D boundingBoxEnd(10,10);
    OutlinerBox2D boundingBox(boundingBoxStart,boundingBoxEnd);
    float stepx = 1.0;
    float stepy = 1.0;
    MaterialMatrix test1(boundingBox,stepx,stepy);
    unsigned int xSize = test1.xIndexSize;
    unsigned int ySize = test1.yIndexSize;
    debugf("test1 sizes %u and %u", xSize, ySize);
    unsigned int n = test1.count();
    debugf("test1 initial count = %u", n);
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
    for (unsigned int x = boundingBox.start.x; x <= boundingBox.end.x; x++) {
      for (unsigned int y = boundingBox.start.y; y <= boundingBox.end.y; y++) {
        bool ans =  test1.getMaterialMatrix(x,y);
        if (ans) {
          debugf("found bit in %u,%u", x, y);
          assert((x == 5 && y == 5) ||
                 (x == 6 && y == 6) ||
                 (x == 9 && y == 9) ||
                 (x == 7 && y == 10));
        }
      }
    }
  }
  
  // Large test
  {
    OutlinerVector2D boundingBoxStart(0,0);
    OutlinerVector2D boundingBoxEnd(1000,1000);
    OutlinerBox2D boundingBox(boundingBoxStart,boundingBoxEnd);
    float stepx = 0.1;
    float stepy = 0.1;
    MaterialMatrix test2(boundingBox,stepx,stepy);
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



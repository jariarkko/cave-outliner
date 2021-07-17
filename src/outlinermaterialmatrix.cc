///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinermath.hh"
#include "outlinerdebug.hh"
#include "outlinermaterialmatrix.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Material matrix maintenance ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

MaterialMatrix::MaterialMatrix(aiVector3D boundingboxstart,
                               aiVector3D boundingboxend,
                               float stepx,
                               float stepy) {
  xIndexSize = ((boundingboxend.x - boundingboxstart.x) / stepx) + 2;
  yIndexSize = ((boundingboxend.y - boundingboxstart.y) / stepy) + 2;
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
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  unsigned int index = xIndex * xIndexSize + yIndex;
  unsigned int charpart = index / 8;
  unsigned int bitpart = index % 8;
  unsigned char bitMask = (1 << bitpart);
  debugf("setting material matrix %u (%u,%u) elem %u with mask %x", index, xIndex, yIndex, charpart, bitMask);
  assert(index < nBits);
  assert(charpart < nChars);
  bitMatrix[charpart] |= bitMask;
}

bool
MaterialMatrix::getMaterialMatrix(unsigned int xIndex,
                                  unsigned int yIndex) {
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(bitMatrix != 0);
  unsigned int index = xIndex * xIndexSize + yIndex;
  unsigned int charpart = index / 8;
  unsigned int bitpart = index % 8;
  unsigned char thechar = bitMatrix[charpart];
  unsigned char bitMask = (1 << bitpart);
  deepdebugf("getting material matrix %u (%u,%u) elem %u value %x with mask %x",
             index, xIndex, yIndex, charpart, thechar, bitMask);
  assert(index < nBits);
  assert(charpart < nChars);
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
      deepdebugf("found non-zero char %x in index %u",
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

  // Simple test
  {
    aiVector3D boundingboxstart(0,0,0);
    aiVector3D boundingboxend(10,10,10);
    float stepx = 1.0;
    float stepy = 1.0;
    MaterialMatrix test1(boundingboxstart,boundingboxend,stepx,stepy);
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
    for (unsigned int x = boundingboxstart.x; x <= boundingboxend.x; x++) {
      for (unsigned int y = boundingboxstart.y; y <= boundingboxend.y; y++) {
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
    aiVector3D boundingboxstart(0,0,0);
    aiVector3D boundingboxend(1000,1000,1000);
    float stepx = 0.1;
    float stepy = 0.1;
    MaterialMatrix test2(boundingboxstart,boundingboxend,stepx,stepy);
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
}



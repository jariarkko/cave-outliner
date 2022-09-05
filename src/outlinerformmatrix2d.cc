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
#include "outlinerformmatrix2d.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Material matrix maintenance ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

FormMatrix2D::FormMatrix2D(const unsigned int xIndexSizeIn,
                           const unsigned int yIndexSizeIn) :
  xIndexSize(xIndexSizeIn),
  yIndexSize(yIndexSizeIn),
  fullSizeNibbles(xIndexSize * yIndexSize),
  fullSizeChars((fullSizeNibbles & 1) != 0 ? (fullSizeNibbles/2)+1 : fullSizeNibbles/2),
  data(0) {
  assert(xIndexSize > 0);
  assert(yIndexSize > 0);
  data = new uint8_t[fullSizeChars];
  if (data == 0) {
    fatalf("Cannot allocate form matrix of %u bytes", fullSizeChars);
    return;
  }
  memset(data,0,fullSizeChars);
}

FormMatrix2D::~FormMatrix2D() {
  debugf("FormMatrix2D::~FormMatrix2D start");
  if (data != 0) {
    delete [] data;
    data = 0;
  }
  debugf("FormMatrix2D::~FormMatrix2D done");
}
  
void
FormMatrix2D::setForm(const unsigned int xIndex,
                      const unsigned int yIndex,
                      const outlinerform form) {
# define formMatrixNibbleIndex(x,y)       (((y)*xIndexSize)+((x)))
# define formMatrixCharIndex(ni)          ((ni)>>1)
# define formMatrixShift(ni)              (((ni)&0x1) != 0 ? 4 : 0)
  assert(form <= outlinerform_max);
  assert(data != 0);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  const unsigned int nibbleIndex = formMatrixNibbleIndex(xIndex,yIndex);
  const unsigned int charIndex = formMatrixCharIndex(nibbleIndex);
  const unsigned int shift = formMatrixShift(nibbleIndex);
  deepdeepdebugf("setForm(%u,%u)=%x index %u %u shift %u fs %u\n", xIndex, yIndex, form, nibbleIndex, charIndex, shift, fullSizeChars);
  deepdebugf("          setForm(%u,%u)=%x",
             xIndex, yIndex, form);
  assert(charIndex < fullSizeChars);
  assert(shift == 0 || shift == 4);
  uint8_t baseValue = data[charIndex];
  deepdebugf("            original base = %02x (shift %u)", baseValue, shift);
  baseValue &= ~(0x0F << shift);
  baseValue |= (form << shift);
  deepdebugf("            new base = %02x", baseValue);
  data[charIndex] = baseValue;
  if (form == outlinerform_mainform_dripline) infof("  setting %u,%u to entrance form", xIndex, yIndex);
}

void
FormMatrix2D::setForm(const unsigned int xIndexStart,
                      const unsigned int yIndexStart,
                      const unsigned int xIndexEnd,
                      const unsigned int yIndexEnd,
                      const outlinerform form) {
  assert(xIndexStart < xIndexSize);
  assert(yIndexStart < yIndexSize);
  deepdebugf("        setForm(%u,%u..%u,%u)",
             xIndexStart, yIndexStart,
             xIndexEnd, yIndexEnd);
  for (unsigned int xIndex = xIndexStart;
       xIndex <= xIndexEnd && xIndex < xIndexSize;
       xIndex++) {
    for (unsigned int yIndex = yIndexStart;
         yIndex <= yIndexEnd && yIndex < yIndexSize;
         yIndex++) {
      assert(xIndex < xIndexSize);
      assert(yIndex < yIndexSize);
      setForm(xIndex,yIndex,form);
    }
  }
}

outlinerform
FormMatrix2D::getForm(const unsigned int xIndex,
                      const unsigned int yIndex) const {
  assert(data != 0);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  const unsigned int nibbleIndex = formMatrixNibbleIndex(xIndex,yIndex);
  const unsigned int charIndex = formMatrixCharIndex(nibbleIndex);
  const unsigned int shift = formMatrixShift(nibbleIndex);
  assert(charIndex < fullSizeChars);
  const uint8_t baseValue = data[charIndex];
  outlinerform form = ((baseValue >> shift) & 0x0F);
  deepdebugf("getForm(%u,%u)=%x base %02x index %u %u shift %u fs %u\n",
             xIndex, yIndex, form,
             baseValue,
             nibbleIndex, charIndex, shift, fullSizeChars);
  assert(form <= outlinerform_max);
  assert(shift == 0 || shift == 4);
  return(form);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Debug outputs //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

char
FormMatrix2D::getFormChar(const outlinerform form) const {
  switch (form) {
  case outlinerform_mainform_empty: return(' ');
  case outlinerform_mainform_tunnel: return('T');
  case outlinerform_mainform_tunnel_stalac: return('S');
  case outlinerform_mainform_dripline: return('E');
  case outlinerform_mainform_degenerate: return('D');
  case outlinerform_mainform_complex: return('C');
  case outlinerform_mainform_cleared: return('Z');
  default:
    assert(0);
    return('U');
  }
}

void
FormMatrix2D::print(unsigned int step) const {
  assert(step < yIndexSize);
  unsigned int yIndex = yIndexSize - step;
  for (;;) {
    char buf[200];
    unsigned int bufCnt = 0;
    memset(buf,0,sizeof(buf));
    for (unsigned int xIndex = 0; xIndex < xIndexSize && bufCnt < sizeof(buf)-1; xIndex += step) {
      buf[bufCnt++] = getFormChar(getForm(xIndex,yIndex));
    }
    debugf("%3u: %s", yIndex, buf);
    if (yIndex < step) break;
    yIndex -= step;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Unit tests for this module /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
FormMatrix2D::test(void) {

  infof("form matrix test running");

  FormMatrix2D matrix(10,10);
  outlinerform form = matrix.getForm(5,5);
  assert(form == outlinerform_mainform_empty);
  form = matrix.getForm(9,9);
  assert(form == outlinerform_mainform_empty);
  matrix.setForm(5,6,outlinerform_mainform_complex);
  matrix.setForm(5,5,outlinerform_mainform_tunnel);
  matrix.setForm(5,4,outlinerform_mainform_tunnel_stalac);
  form = matrix.getForm(1,3);
  assert(form == outlinerform_mainform_empty);
  form = matrix.getForm(5,7);
  assert(form == outlinerform_mainform_empty);
  form = matrix.getForm(5,6);
  assert(form == outlinerform_mainform_complex);
  form = matrix.getForm(5,5);
  assert(form == outlinerform_mainform_tunnel);
  form = matrix.getForm(5,4);
  assert(form == (outlinerform_mainform_tunnel_stalac));
  infof("form matrix test ok");
}



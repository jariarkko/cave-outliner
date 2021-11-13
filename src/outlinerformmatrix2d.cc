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

FormMatrix2D::FormMatrix2D(unsigned int xIndexSizeIn,
                           unsigned int yIndexSizeIn) :
  xIndexSize(xIndexSizeIn),
  yIndexSize(yIndexSizeIn),
  data(0) {
  assert(xIndexSize > 0);
  assert(yIndexSize > 0);
  fullSizeNibbles = xIndexSize * yIndexSize;
  fullSizeChars = fullSizeNibbles/2;
  if ((fullSizeNibbles & 1) != 0) {
    fullSizeChars++;
  }
  data = new uint8_t[fullSizeChars];
  if (data == 0) {
    errf("Cannot allocaet form matrix of %u bytes", fullSizeChars);
    exit(1);
  }
  memset(data,0,fullSizeChars);
}

FormMatrix2D::~FormMatrix2D() {
  if (data != 0) {
    free((void*)data);
    data = 0;
  }
}
  
void
FormMatrix2D::setForm(unsigned int xIndex,
                      unsigned int yIndex,
                      outlinerform form) {
# define formMatrixNibbleIndex(x,y)       (((y)*yIndexSize)+((x)))
# define formMatrixCharIndex(ni)          ((ni)>>1)
# define formMatrixShift(ni)              (((ni)&0x1) != 0 ? 4 : 0)
  assert(form < outlinerform_max);
  assert(data != 0);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  const unsigned int nibbleIndex = formMatrixNibbleIndex(xIndex,yIndex);
  const unsigned int charIndex = formMatrixCharIndex(nibbleIndex);
  const unsigned int shift = formMatrixShift(nibbleIndex);
  deepdeepdebugf("setForm(%u,%u) index %u %u shift %u fs %u\n", xIndex, yIndex, nibbleIndex, charIndex, shift, fullSizeChars);
  assert(charIndex < fullSizeChars);
  uint8_t baseValue = data[charIndex];
  baseValue &= (0xF << shift);
  baseValue |= (form << shift);
  data[charIndex] = baseValue;
}

outlinerform
FormMatrix2D::getForm(unsigned int xIndex,
                      unsigned int yIndex) {
  assert(data != 0);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  const unsigned int nibbleIndex = formMatrixNibbleIndex(xIndex,yIndex);
  const unsigned int charIndex = formMatrixCharIndex(nibbleIndex);
  const unsigned int shift = formMatrixShift(nibbleIndex);
  deepdeepdebugf("setForm(%u,%u) index %u %u shift %u fs %u\n", xIndex, yIndex, nibbleIndex, charIndex, shift, fullSizeChars);
  assert(charIndex < fullSizeChars);
  const uint8_t baseValue = data[charIndex];
  outlinerform form = (baseValue >> shift);
  assert(form < outlinerform_max);
  return(form);
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
  matrix.setForm(5,4,outlinerform_mainform_tunnel|outlinerform_extraform_stalac);
  form = matrix.getForm(1,3);
  assert(form == outlinerform_mainform_empty);
  form = matrix.getForm(5,7);
  assert(form == outlinerform_mainform_empty);
  form = matrix.getForm(5,6);
  assert(form == outlinerform_mainform_complex);
  form = matrix.getForm(5,5);
  assert(form == outlinerform_mainform_tunnel);
  form = matrix.getForm(5,4);
  assert(form == (outlinerform_mainform_tunnel|outlinerform_extraform_stalac));
  infof("form matrix test ok");
}



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
                                   const outlinerreal stepx,
                                   const outlinerreal stepy,
                                   const outlinerreal stepz) :
  boundingBox(boundingBoxIn),
  xIndexSize(MaterialMatrix2D::calculateSize(boundingBox.start.x,boundingBox.end.x,stepx)),
  yIndexSize(MaterialMatrix2D::calculateSize(boundingBox.start.y,boundingBox.end.y,stepy)),
  zIndexSize(MaterialMatrix2D::calculateSize(boundingBox.start.z,boundingBox.end.z,stepz)),
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
}

bool
MaterialMatrix3D::getMaterialMatrix(const unsigned int xIndex,
                                    const unsigned int yIndex,
                                    const unsigned int zIndex) const {
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(zIndex < zIndexSize);
  return(0);
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

///////////////////////////////////////////////////////////////////////////////////////////////
// Unit tests for this module /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
MaterialMatrix3D::test(void) {

  infof("material matrix test running");
  infof("material matrix test ok");
}



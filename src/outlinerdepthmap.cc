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
#include "outlinerprocessor.hh"
#include "outlinermaterialmatrix2d.hh"
#include "outlinerdepthmap.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Material matrix maintenance ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

DepthMap::DepthMap(const unsigned int xIndexSizeIn,
                   const unsigned int yIndexSizeIn,
                   const MaterialMatrix2D& materialMatrixIn) :
  xIndexSize(xIndexSizeIn),
  yIndexSize(yIndexSizeIn),
  fullSize(xIndexSize * yIndexSize),
  data(0),
  materialMatrix(materialMatrixIn) {
  assert(xIndexSize > 0);
  assert(yIndexSize > 0);
  data = new outlinerdepth[fullSize];
  if (data == 0) {
    errf("Cannot allocate depth matrix of %u bytes", fullSize * sizeof(outlinerdepth));
    exit(1);
  }
  memset(data,0,fullSize * sizeof(outlinerdepth));
}

DepthMap::~DepthMap() {
  if (data != 0) {
    free((void*)data);
    data = 0;
  }
}
  
void
DepthMap::setDepth(const unsigned int xIndex,
                   const unsigned int yIndex,
                   const outlinerdepth depth) {
# define depthMatrixIndex(x,y)       (((y)*xIndexSize)+((x)))
  assert(data != 0);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(materialMatrix.getMaterialMatrix(xIndex,yIndex));
  const unsigned int index = depthMatrixIndex(xIndex,yIndex);
  assert(index < fullSize);
  data[index] = depth;
}

outlinerdepth
DepthMap::getDepth(const unsigned int xIndex,
                   const unsigned int yIndex) const {
  assert(data != 0);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(materialMatrix.getMaterialMatrix(xIndex,yIndex));
  const unsigned int index = depthMatrixIndex(xIndex,yIndex);
  assert(index < fullSize);
  return(data[index]);
}

OutlinerSvgStyle
DepthMap::depthToColor(const unsigned int xIndex,
                       const unsigned int yIndex) const {
  assert(data != 0);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(materialMatrix.getMaterialMatrix(xIndex,yIndex));
  outlinerdepth depth = getDepth(xIndex,yIndex);
  if (depth == 0) return(outlinersvgstyle_greyval(254));
  else if (depth == 255) return(outlinersvgstyle_greyval(1));
  else return(outlinersvgstyle_greyval(255 - depth));
}
  
OutlinerSvgStyle
DepthMap::depthDiffToColor(const unsigned int xIndex,
                           const unsigned int yIndex,
                           const Processor& proc) const {
  assert(data != 0);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(materialMatrix.getMaterialMatrix(xIndex,yIndex));
  outlinerdepth depth = getDepth(xIndex,yIndex);
  unsigned int n;
  const unsigned int tableSize = 20;
  unsigned int tableX[tableSize];
  unsigned int tableY[tableSize];
  proc.getNeighbours(xIndex,
                     yIndex,
                     n,
                     tableSize,
                     tableX,
                     tableY);
  outlinerreal present = 0.0;
  outlinerreal sum = 0.0;
  for (unsigned int i = 0; i < n; i++) {
    unsigned int neighXIndex = tableX[i];
    unsigned int neighYIndex = tableY[i];
    if (materialMatrix.getMaterialMatrix(neighXIndex,neighYIndex)) {
      present++;
      sum += getDepth(neighXIndex,neighYIndex);
    }
  }
  if (present == 0) {
    return(outlinersvgstyle_greyval(128));
  } else {
    outlinerreal avg = sum / present;
    outlinerdepth avgInt = (outlinerdepth)floor(avg);
    outlinerdepth diff = (avgInt < depth) ? (depth-avgInt) : (avgInt-depth);
    outlinerdepth normalizeddiff = diff/2;
    assert(normalizeddiff <= 128);
    if (avgInt == depth) {
      return(outlinersvgstyle_greyval(128));
    } else if (avgInt < depth) {
      return(outlinersvgstyle_greyval(128-normalizeddiff));
    } else {
      return(outlinersvgstyle_greyval(128+normalizeddiff));
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Unit tests for this module /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
DepthMap::test(void) {

  infof("depth map test running");

  OutlinerBox2D mmbb(0,0,10,10);
  MaterialMatrix2D mm(mmbb,1,1);
  mm.setMaterialMatrix(1,3);
  mm.setMaterialMatrix(5,7);
  mm.setMaterialMatrix(5,6);
  mm.setMaterialMatrix(5,5);
  mm.setMaterialMatrix(5,4);
  mm.setMaterialMatrix(9,9);
  DepthMap matrix(10,10,mm);
  outlinerform depth = matrix.getDepth(5,5);
  assert(depth == 0);
  depth = matrix.getDepth(9,9);
  assert(depth == 0);
  matrix.setDepth(5,6,10);
  matrix.setDepth(5,5,200);
  matrix.setDepth(5,4,250);
  depth = matrix.getDepth(1,3);
  assert(depth == 0);
  depth = matrix.getDepth(5,7);
  assert(depth == 0);
  depth = matrix.getDepth(5,6);
  assert(depth == 10);
  depth = matrix.getDepth(5,5);
  assert(depth == 200);
  depth = matrix.getDepth(5,4);
  assert(depth == 250);
  infof("depth matrix test ok");
}



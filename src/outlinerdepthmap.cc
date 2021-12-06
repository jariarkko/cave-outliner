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
#include "outlinersvg.hh"
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
  rangeSet(0),
  rangeMin(0),
  rangeMax(0),
  nEntries(0),
  materialMatrix(materialMatrixIn) {

  assert(xIndexSize > 0);
  assert(yIndexSize > 0);
  assert(xIndexSize == materialMatrix.xIndexSize);
  assert(yIndexSize == materialMatrix.yIndexSize);
  data = new outlinerdepth[fullSize];
  if (data == 0) {
    fatalf("Cannot allocate depth matrix of %u bytes", fullSize * sizeof(outlinerdepth));
    return;
  }
  memset(data,0,fullSize * sizeof(outlinerdepth));
}

DepthMap::~DepthMap() {
  infof("depth map %u/%u entries, range %u..%u",
        nEntries, fullSize,
        rangeMin, rangeMax);
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
  //assert(materialMatrix.getMaterialMatrix(xIndex,yIndex));
  const unsigned int index = depthMatrixIndex(xIndex,yIndex);
  assert(index < fullSize);
  outlinerdepth prev = data[index];
  data[index] = depth;
  if (!rangeSet) {
    rangeMin = rangeMax = depth;
    rangeSet = 1;
  } else {
    if (depth < rangeMin) rangeMin = depth;
    if (depth > rangeMax) rangeMax = depth;
  }
  if (prev == 0) nEntries++;
  infof("depth(%u,%u)=%u (%u..%u)", xIndex, yIndex, depth, rangeMin, rangeMax);
}

void
DepthMap::setDepthRange(const unsigned int xIndexStart,
                        const unsigned int yIndexStart,
                        const unsigned int xIndexEnd,
                        const unsigned int yIndexEnd,
                        const outlinerdepth depth) {
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
      setDepth(xIndex,yIndex,depth);
    }
  }
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
  outlinerdepth input = getDepth(xIndex,yIndex);
  infof("    depthToColor(%u,%u)=%u (%u..%u)", xIndex, yIndex, input, rangeMin, rangeMax);
  outlinerdepth depth = normalize(input);
  outlinerdepth rgbval = 255 - depth;
  outlinerdepth finalval;
  if (rgbval < outlinerunusablegreyscale) finalval = outlinerunusablegreyscale;
  else if (rgbval > 255-outlinerunusablegreyscale) finalval = 255-outlinerunusablegreyscale;
  else finalval = rgbval;
  debugf("    depth %u => %u => %u => %u", input, depth, rgbval,finalval);
  return(outlinersvgstyle_greyval(finalval));
}

OutlinerSvgStyle
DepthMap::depthDiffToColor(const unsigned int xIndex,
                           const unsigned int yIndex,
                           const Processor& proc) const {
  assert(data != 0);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(materialMatrix.getMaterialMatrix(xIndex,yIndex));
  outlinerdepth depth = normalize(getDepth(xIndex,yIndex));
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
  //outlinerreal present = 0.0;
  //outlinerreal sum = 0.0;
  unsigned int present = 0;
  outlinerdepth minval = 0;
  for (unsigned int i = 0; i < n; i++) {
    unsigned int neighXIndex = tableX[i];
    unsigned int neighYIndex = tableY[i];
    if (materialMatrix.getMaterialMatrix(neighXIndex,neighYIndex)) {
      outlinerdepth x = normalize(getDepth(neighXIndex,neighYIndex));
      if (present == 0) minval = x;
      else if (minval > x) minval = x;
      present++;
    }
  }
  debugf("  neighbor depths %u min %u, own %u", present, minval, depth);
  if (present == 0) {
    return(outlinersvgstyle_greyval(128));
  } else {
    //outlinerreal avg = sum / present;
    //outlinerdepth avgInt = (outlinerdepth)floor(avg);
    outlinerdepth avgInt = minval;
    if (depth <= avgInt) {
      return(outlinersvgstyle_greyval(255-outlinerunusablegreyscale));
    } else {
      outlinerdepth diff = depth-avgInt;
      outlinerdepth val  = 255-diff;
      outlinerdepth finalval;
      if (val < outlinerunusablegreyscale) finalval = outlinerunusablegreyscale;
      if (val > 255 - outlinerunusablegreyscale) finalval = 255 - outlinerunusablegreyscale;
      else finalval = val;
      debugf("  depth diff %u from %u = %u => %u => %u", depth, avgInt, diff, val, finalval);
      return(outlinersvgstyle_greyval(finalval));
    }
  }
}

outlinerdepth
DepthMap::normalize(outlinerdepth input) const {
  assert(rangeSet);
  if (input < rangeMin || input > rangeMax) {
    infof("    normalize depth %u (%u..%u)", input, rangeMin, rangeMax);
  }
  assert(input >= rangeMin);
  assert(input <= rangeMax);
  outlinerdepth range = rangeMax - rangeMin;
  outlinerdepth base = 0;
  outlinerdepth factor = 1;
  outlinerdepth halfFactor = 0;
  if      (range <   8) { base = rangeMin; factor = 32; halfFactor = 0; }
  else if (range <  16) { base = rangeMin; factor = 16; halfFactor = 0; }
  else if (range <  32) { base = rangeMin; factor =  8; halfFactor = 0; }
  else if (range <  64) { base = rangeMin; factor =  4; halfFactor = 0; }
  else if (range < 128) { base = rangeMin; factor =  2; halfFactor = 0; }
  else if (range < 196) { base = rangeMin; factor =  1; halfFactor = 1; }
  outlinerdepth preval = input - base;
  outlinerdepth result = preval * factor + (preval/2) * halfFactor;
  if (base != 0 || factor != 1 || halfFactor != 0) {
    debugf("  normalizing depth %u to %u via %ux 0.5*%ux sub %u",
           input, result,
           factor, halfFactor, base);
  }
  return(result);
}

outlinerdepth
DepthMap::calculateDepthWithinRange(outlinerreal depth,
                                    outlinerreal start,
                                    outlinerreal end) {
  outlinerreal normalizedDepth;
  if (depth < start) normalizedDepth = 0;
  else if (depth > end) normalizedDepth = 255;
  else if (start == end) normalizedDepth = 128;
  else normalizedDepth = (255 * (depth - start)) / (end - start);
  outlinerdepth normalizedDepthInt = (outlinerdepth)floor(normalizedDepth);
  return(normalizedDepthInt);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Image processing ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
DepthMap::toImage(const char* filename,
                  const unsigned int multiplier,
                  const bool svgYSwap,
                  const bool diff,
                  const Processor& proc) const {

  // Sanity checks
  
  assert(xIndexSize == materialMatrix.xIndexSize);
  assert(yIndexSize == materialMatrix.yIndexSize);
  // Allocate an image object
  SvgCreator image(filename,
                   xIndexSize,yIndexSize,
                   multiplier,
                   0,0,
                   1,1,
                   0,0,
                   1,
                   svgYSwap);

  // Construct the actual image
  infof("converting depth map to image in file %s", filename);
  toImageAux(image,diff,proc);
  
  // Check that we were able to open and write the file
  deepdebugf("svg initial check");
  if (!image.ok()) {
    errf("File open for writing to %s failed", filename);
  }
  infof("Done");
}

void
DepthMap::toImageAux(SvgCreator& image,
                     const bool diff,
                     const Processor& proc) const {
  if (!rangeSet) {
     errf("Not enough model to draw a depth map");
    return;
  }
  assert(xIndexSize == materialMatrix.xIndexSize);
  assert(yIndexSize == materialMatrix.yIndexSize);
  for (unsigned int xIndex = 0; xIndex < xIndexSize; xIndex++) {
    for (unsigned int yIndex = 0; yIndex < yIndexSize; yIndex++) {
      if (!proc.getMaterialMatrix(xIndex,yIndex)) continue;
      OutlinerSvgStyle style =
        (diff ?
         depthDiffToColor(xIndex,yIndex,proc) :
         depthToColor(xIndex,yIndex));
      image.pixel(xIndex,yIndex,style);
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
  DepthMap matrix(mm.xIndexSize,mm.yIndexSize,mm);
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



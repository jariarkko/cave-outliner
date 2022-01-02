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
  range(0),
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
  infof("depth map %u/%u entries, range %u..%u (%u)",
        nEntries, fullSize,
        rangeMin, rangeMax,
	range);
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
    range = 0;
    rangeSet = 1;
  } else {
    if (depth < rangeMin) { rangeMin = depth; range = rangeMax - rangeMin; }
    if (depth > rangeMax) { rangeMax = depth; range = rangeMax - rangeMin; }
  }
  if (prev == 0) nEntries++;
  if (yIndex == 10) infof("set depth(%u,%u index %u/%u into table %lx)=%u (%u..%u)", xIndex, yIndex, index, fullSize, data, depth, rangeMin, rangeMax);
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
  outlinerdepth result = data[index];
  if (yIndex == 10) infof("get depth(%u,%u index %u/%u into table %lx)=%u (%u..%u)", xIndex, yIndex, index, fullSize, data, result, rangeMin, rangeMax);
  return(result);
}


OutlinerSvgStyle
DepthMap::depthToColor(const unsigned int xIndex,
                       const unsigned int yIndex) const {
  assert(data != 0);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(materialMatrix.getMaterialMatrix(xIndex,yIndex));
  outlinerdepth input = getDepth(xIndex,yIndex);
  outlinerdepth depth = normalize(input);
  outlinerdepth reverseval = outlinerdepth_maxvalue - depth;
  OutlinerRgb finalval = rgbCompress(reverseval);
  debugf("    depthToColor(%u,%u)=%u (%u..%u) normalized %02x rgbval %02x finalval %02x",
	 xIndex, yIndex, input, rangeMin, rangeMax, depth, reverseval, finalval);
  if (yIndex == 10) infof(" depth matrix color x = %u, depth = %u, normalized = %u, rgbval = %u, rgb = %u",
			  xIndex, input, depth, reverseval, finalval);
  return(outlinersvgstyle_greyval(finalval));
}

OutlinerRgb
DepthMap::rgbCompress(const outlinerdepth input) {
  static const unsigned int scaleFactor = 16;
  static const uint64_t actualRgbRangeAvailable = (256 - 2*outlinerunusablegreyscale);
  static const uint64_t scale = ((actualRgbRangeAvailable<<scaleFactor)/256);
  const uint64_t scaledInput = (((uint64_t)input)*scale);
  const uint64_t compressedScaledInput = (scaledInput >> scaleFactor);
  const outlinerdepth compressedInput = (outlinerdepth)compressedScaledInput;
  const OutlinerRgb adjustedCompressedInput = outlinerunusablegreyscale + (OutlinerRgb)(compressedInput >> outlinerdepthrgbscale);
  infof("   rgb compress %u to %lu  (actual %lu, scale %lu, scaled input %lu)",
        input, adjustedCompressedInput,
        actualRgbRangeAvailable, scale, scaledInput);
  return(adjustedCompressedInput);
}

OutlinerSvgStyle
DepthMap::depthDiffToColor(const unsigned int xIndex,
                           const unsigned int yIndex,
                           unsigned int step) const {

  // Sanity checks
  assert(data != 0);
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(materialMatrix.getMaterialMatrix(xIndex,yIndex));
  assert(step >= 1);

  // Normalize this value
  outlinerdepth depth = getDepth(xIndex,yIndex);

  // Determine what neighbors there are 
  unsigned int n;
  const unsigned int tableSize = 20;
  unsigned int tableX[tableSize];
  unsigned int tableY[tableSize];
  Processor::getNeighbours(xIndex,
                           yIndex,
                           xIndexSize,
                           yIndexSize,
                           n,
                           tableSize,
                           tableX,
                           tableY,
                           step);
  // Handle special cases
  if (range == 0) return(128);
  
  // Calculate what depth values they have
# define sideavg(nside,sumside)		\
  (((outlinerreal)(sumside))/(nside))
# define sidegradient1(depthhere,nside,sumside)                                         \
  ((depthhere) - sideavg(nside,sumside))
# define sidegradient2(depthhere,nside,sumside)                                         \
  (sideavg(nside,sumside) - (depthhere))
# define sidegradient3(depthhere,nsmaller,sumsmaller,nlarger,sumlarger)                 \
  ((sidegradient1(depthhere,nsmaller,sumsmaller) +                                      \
    sidegradient2(depthhere,nlarger,sumlarger))/2.0)   
# define setdirectiongradient(gradient,depthhere,nsmaller,sumsmaller,nlarger,sumlarger) \
  if ((nsmaller) == 0 && (nlarger) == 0) (gradient) = 0.0;                              \
  else if ((nlarger) == 0) (gradient) = sidegradient1(depthhere,nsmaller,sumsmaller); \
  else if ((nsmaller) == 0) (gradient) = sidegradient2(depthhere,nlarger,sumlarger); \
  else (gradient) = sidegradient3(depthhere,nsmaller,sumsmaller,nlarger,sumlarger)
  unsigned int sSmallerX;
  unsigned int nSmallerX = countSmallerX(xIndex,yIndex,n,tableX,tableY,sSmallerX);
  unsigned int sLargerX;
  unsigned int nLargerX = countLargerX(xIndex,yIndex,n,tableX,tableY,sLargerX);
  unsigned int sSmallerY;
  unsigned int nSmallerY = countSmallerY(xIndex,yIndex,n,tableX,tableY,sSmallerY);
  unsigned int sLargerY;
  unsigned int nLargerY = countLargerY(xIndex,yIndex,n,tableX,tableY,sLargerY);
  outlinerreal depthreal = depth;
  outlinerreal xgradient;
  outlinerreal ygradient;
  setdirectiongradient(xgradient,depthreal,nSmallerX,sSmallerX,nLargerX,sLargerX);
  setdirectiongradient(ygradient,depthreal,nSmallerY,sSmallerY,nLargerY,sLargerY);
  outlinerreal result = sqrt((outlinersquared(xgradient) + outlinersquared(ygradient))/2.0);
  assert(result >= 0.0);
  assert(result <= outlinerdepth_maxvalue);
  outlinerdepth resultInt = (outlinerdepth)floor(result);
  outlinerdepth diffcompress;
  outlinerdepth diffexpand;
  if (range < 16)        { diffcompress =  2; diffexpand = (outlinerdepth_maxvalue+1) /    16; }
  else if (range < 64)   { diffcompress =  4; diffexpand = (outlinerdepth_maxvalue+1) /    64; }
  else if (range < 256)  { diffcompress =  8; diffexpand = (outlinerdepth_maxvalue+1) /   256; }
  else if (range < 1024) { diffcompress = 16; diffexpand = (outlinerdepth_maxvalue+1) /  1024; }
  else if (range < 4096) { diffcompress = 32; diffexpand = (outlinerdepth_maxvalue+1) /  4096; }
  else                   { diffcompress = 64; diffexpand = (outlinerdepth_maxvalue+1) / 65536; }
  const outlinerdepth maxdiff = (range / diffcompress) - 1;
  outlinerdepth resultFinal = resultInt;
  if (resultFinal > maxdiff) resultFinal = maxdiff;
  resultFinal *= diffcompress;
  resultFinal *= diffexpand;
  outlinerdepth reverseval = outlinerdepth_maxvalue - resultFinal;
  outlinerdepth resultCompressed = rgbCompress(reverseval);
  if (yIndex == 10) infof(" depth matrix diff x = %u, depth = %u, result = %u, resultFinal = %u, reverseval = %u (compress %u, expand %u), compressed = %u",
			  xIndex, depth, resultInt, resultFinal, reverseval,
			  diffcompress, diffexpand,
			  resultCompressed);
  return(outlinersvgstyle_greyval(resultCompressed));
  
}

unsigned int
DepthMap::countSmallerX(const unsigned int xIndex,
                        const unsigned int yIndex,
                        const unsigned int n,
                        const unsigned int* tableX,
                        const unsigned int* tableY,
                        unsigned int& sum) const {
  if (xIndex == 0) { sum = 0; return(0); }
  else return(countGeneric(0,xIndex-1,
                           0,yIndexSize - 1,
                           n,
                           tableX,
                           tableY,
                           sum));
}

unsigned int
DepthMap::countLargerX(const unsigned int xIndex,
                       const unsigned int yIndex,
                       const unsigned int n,
                       const unsigned int* tableX,
                       const unsigned int* tableY,
                       unsigned int& sum) const {
  if (xIndex >= xIndexSize - 1) { sum = 0; return(0); }
  else return(countGeneric(xIndex+1,xIndexSize - 1,
                           0,yIndexSize - 1,
                           n,
                           tableX,
                           tableY,
                           sum));
}

unsigned int
DepthMap::countSmallerY(const unsigned int xIndex,
                        const unsigned int yIndex,
                        const unsigned int n,
                        const unsigned int* tableX,
                        const unsigned int* tableY,
                        unsigned int& sum) const {
  if (yIndex == 0) { sum = 0; return(0); }
  else return(countGeneric(0,xIndexSize - 1,
                           0,yIndex - 1,
                           n,
                           tableX,
                           tableY,
                           sum));
}

unsigned int
DepthMap::countLargerY(const unsigned int xIndex,
                       const unsigned int yIndex,
                       const unsigned int n,
                       const unsigned int* tableX,
                       const unsigned int* tableY,
                       unsigned int& sum) const {
  if (yIndex >= yIndexSize - 1) { sum = 0; return(0); }
  else return(countGeneric(0,xIndexSize - 1,
                           yIndex + 1,yIndexSize - 1,
                           n,
                           tableX,
                           tableY,
                           sum));
}

unsigned int
DepthMap::countGeneric(const unsigned int xIndexRangeStart,
                       const unsigned int xIndexRangeEnd,
                       const unsigned int yIndexRangeStart,
                       const unsigned int yIndexRangeEnd,
                       const unsigned int n,
                       const unsigned int* tableX,
                       const unsigned int* tableY,
                       unsigned int& sum) const {
  unsigned int howMany = 0;
  sum = 0;
  for (unsigned int i = 0; i < n; i++) {
    unsigned int neighXIndex = tableX[i];
    unsigned int neighYIndex = tableY[i];
    if (neighXIndex < xIndexRangeStart || neighXIndex > xIndexRangeEnd) continue;
    if (neighYIndex < yIndexRangeStart || neighYIndex > yIndexRangeEnd) continue;
    if (materialMatrix.getMaterialMatrix(neighXIndex,neighYIndex)) {
      outlinerdepth depth = getDepth(neighXIndex,neighYIndex);
      sum += depth;
      howMany++;
    }
  }
  return(howMany);
}

outlinerdepth
DepthMap::normalize(outlinerdepth input) const {
  assert(rangeSet);
  if (input < rangeMin || input > rangeMax) {
    infof("    normalize depth %u (%u..%u)", input, rangeMin, rangeMax);
  }
  assert(input >= rangeMin);
  assert(input <= rangeMax);
  outlinerdepth base = 0;
  unsigned int factor = 1;
  unsigned int quarterFactor = 0;
  if      (range <     2)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/   2; quarterFactor = 0; }
  else if (range <     4)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/   4; quarterFactor = 0; }
  else if (range <     8)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/   8; quarterFactor = 0; }
  else if (range <    16)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/  16; quarterFactor = 0; }
  else if (range <    32)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/  32; quarterFactor = 0; }
  else if (range <    64)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/  64; quarterFactor = 0; }
  else if (range <   128)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/ 128; quarterFactor = 0; }
  else if (range <   128+64)     { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/ 256; quarterFactor = 1; }
#if outlinerdepthrgbscale == 8
  else if (range <   256)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/ 256; quarterFactor = 0; }
  else if (range <   512)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/ 512; quarterFactor = 0; }
  else if (range <  1024)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/1024; quarterFactor = 0; }
  else if (range <  2048)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/2048; quarterFactor = 0; }
  else if (range <  4096)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/4096; quarterFactor = 0; }
  else if (range <  8192)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/81926; quarterFactor = 0; }
  else if (range < 16384)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/16384; quarterFactor = 0; }
  else if (range < 32768)        { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/32768; quarterFactor = 0; }
  else if (range < 32768+16384)  { base = rangeMin; factor =  (outlinerdepth_maxvalue+1)/65536; quarterFactor = 1; }
#endif
  outlinerdepth preval = input - base;
  outlinerdepth result = preval * factor + (preval/4) * quarterFactor;
  if (base != 0 || factor != 1 || quarterFactor != 0) {
    debugf("  normalizing depth %u to %u via %ux 0.5*%ux sub %u",
           input, result,
           factor, quarterFactor, base);
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

bool
DepthMap::getRange(outlinerdepth& minRangeOut,
		   outlinerdepth& maxRangeOut) const {
  if (rangeSet) {
    minRangeOut = rangeMin;
    maxRangeOut = rangeMax;
    return(1);
  } else {
    return(0);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Image processing ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
DepthMap::toImage(const char* filename,
                  const unsigned int multiplier,
                  const bool svgYSwap,
                  const bool diff,
                  unsigned int step) const {

  // Sanity checks
  assert(step >= 1);
  assert(xIndexSize == materialMatrix.xIndexSize);
  assert(yIndexSize == materialMatrix.yIndexSize);
  infof("computed depth map (%s) to image: %u..%u",
	diff ? "diff" : "regular",
	rangeMin, rangeMax);
  
  // Allocate an image object
  SvgOptions imageOptions(multiplier,
                          0,0,
                          1,
                          svgYSwap);
  SvgCreator image(filename,
                   xIndexSize,yIndexSize,
                   0,0,
                   1,1,
                   imageOptions);

  // Construct the actual image
  infof("converting depth map to image in file %s", filename);
  toImageAux(image,diff,step);
  
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
                     unsigned int step) const {
  if (!rangeSet) {
     errf("Not enough model to draw a depth map");
    return;
  }
  assert(xIndexSize == materialMatrix.xIndexSize);
  assert(yIndexSize == materialMatrix.yIndexSize);
  for (unsigned int xIndex = 0; xIndex < xIndexSize; xIndex++) {
    for (unsigned int yIndex = 0; yIndex < yIndexSize; yIndex++) {
      if (!materialMatrix.getMaterialMatrix(xIndex,yIndex)) continue;
      OutlinerSvgStyle style =
        (diff ?
         depthDiffToColor(xIndex,yIndex,step) :
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

  // Basic test
  infof("depth map basic test");
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

  // Value compression tests
  infof("depth map  value compression test");
  outlinerdepth result = rgbCompress(0);
  assert(result == outlinerunusablegreyscale);
  result = rgbCompress(outlinerdepth_maxvalue);
  assert(result == 255 - outlinerunusablegreyscale ||
         result == 255 - outlinerunusablegreyscale - 1);
  result = rgbCompress(20);
  infof("compress result %u", result);
  assert((outlinerdepth_maxvalue == 65535 && result == outlinerunusablegreyscale + 0) ||
	 (outlinerdepth_maxvalue == 255 && result == outlinerunusablegreyscale + 18));
  result = rgbCompress(outlinerdepth_maxvalue/2);
  infof("compress result %u", result);
  assert((outlinerdepth_maxvalue == 65535 && result == 127) ||
	 (outlinerdepth_maxvalue == 255 && result == 127));
  result = rgbCompress(outlinerdepth_maxvalue);
  infof("compress result %u", result);
  assert((outlinerdepth_maxvalue == 65535 && result == 255 - outlinerunusablegreyscale) ||
	 (outlinerdepth_maxvalue == 255 && result == outlinerunusablegreyscale + 239));
  
  // Diff test, simple version
  infof("depth map diff simple test");
  OutlinerBox2D mmbb2(0,0,3,3);
  MaterialMatrix2D mm2(mmbb2,1,1);
  DepthMap map2(mm2.xIndexSize,mm2.yIndexSize,mm2);
  infof("fill");
  const unsigned factor = (outlinerdepth_maxvalue+1)/256;
  for (unsigned int x = 0; x < mm2.xIndexSize - 2; x++) {
    for (unsigned int y = 0; y < mm2.yIndexSize - 2; y++) {
      mm2.setMaterialMatrix(x,y);
      map2.setDepth(x,y,(10+x)*factor);
    }
  }
  infof("depth check");
  for (unsigned int x = 0; x < mm2.xIndexSize - 2; x++) {
    for (unsigned int y = 0; y < mm2.yIndexSize - 2; y++) {
      if (mm2.getMaterialMatrix(x,y)) {
        outlinerdepth depth = map2.getDepth(x,y);
        assert(depth == (10+x)*factor);
      }
    }
  }
  infof("color check");
  for (unsigned int x = 0; x < mm2.xIndexSize - 2; x++) {
    for (unsigned int y = 0; y < mm2.yIndexSize - 2; y++) {
      if (mm2.getMaterialMatrix(x,y)) {
        outlinerdepth depth = map2.getDepth(x,y);
        assert(depth == (10+x)*factor);
        OutlinerSvgStyle style = map2.depthToColor(x,y);
        assert(outlinersvgstyle_getbase(style) == outlinersvgstyle_grey);
        unsigned int greylevel = outlinersvgstyle_greyget(style);
        infof("  color check %u,%u depth %u grey %02x", x ,y, depth, greylevel);
        switch (x) {
        case 0: assert(greylevel == 0xf7); break;
        case 1: assert(greylevel == 0xbb /*0xd9*/); break;
        case 2: assert(greylevel == 0x7f /*0xbb*/); break;
        default: assert(0); break;
        }
      }
    }
  }
  infof("diff check");
  for (unsigned int x = 0; x < mm2.xIndexSize - 2; x++) {
    for (unsigned int y = 0; y < mm2.yIndexSize - 2; y++) {
      if (mm2.getMaterialMatrix(x,y)) {
        outlinerdepth depth = map2.getDepth(x,y);
        assert(depth == (10+x)*factor);
        OutlinerSvgStyle style = map2.depthDiffToColor(x,y,1);
        assert(outlinersvgstyle_getbase(style) == outlinersvgstyle_grey);
        unsigned int greylevel = outlinersvgstyle_greyget(style);
        infof("  color check %u,%u depth %u diff grey %02x", x ,y, depth, greylevel);
        switch (x) {
        case 0: assert(greylevel != 0); break;
        case 1: assert(greylevel != 0); break;
        case 2: assert(greylevel != 0); break;
        default: assert(0); break;
        }
      }
    }
  }
  infof("writing depth maps to files");
  {
    const char* depthmaptestfile = OUTLINER_TEMPDIRECTORY "cave-outliner-unit-test-depthmap.svg";
    const char* depthdiffmaptestfile = OUTLINER_TEMPDIRECTORY "cave-outliner-unit-test-depthdiffmap.svg";
    const unsigned int siz = 200;
    const unsigned int border = 10;
    const unsigned int step = 5;
    const unsigned int div = 7;
    OutlinerBox2D mmbb3(0,0,siz,siz);
    MaterialMatrix2D mm3(mmbb3,1,1);
    DepthMap map3(mm3.xIndexSize,mm3.yIndexSize,mm3);
    for (unsigned int x = border; x < mm3.xIndexSize - 2 - border; x++) {
      for (unsigned int y = border; y < mm3.yIndexSize - 2 - border; y++) {
	mm3.setMaterialMatrix(x,y);
	unsigned int height = (x/step);
	if (height > div) height *= (height/div);
	if (y == border) infof("  depth matrix number 3 x = %u, height = %u", x, height);
	assert(height <= outlinerdepth_maxvalue);
	map3.setDepth(x,y,(outlinerdepth)height);
	outlinerdepth back = map3.getDepth(x,y);
	assert(back == height);
      }
    }
    map3.toImage(depthmaptestfile,
		 1,
		 1,
		 0,
		 1);
    map3.toImage(depthdiffmaptestfile,
		 1,
		 1,
		 1,
		 1);
  }
  infof("depth matrix test ok");
}



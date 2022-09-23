
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

#include <cassert>
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"
#include "outlinerhighprecision.hh"
#include "outlinerdebug.hh"
#include "outlinersvgoptions.hh"
#include "outlinersvg.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class functions ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

SvgOptions::SvgOptions()  :
  multiplier(1),
  smooth(0),
  mergedLines(1),
  linewidth(outlinerdefaultlinewidth),
  ySwap(1),
  maxLinePoints(OutlinerSvgDefaultLinePoints) {
}

SvgOptions::SvgOptions(const unsigned int multiplierIn,
                       const bool smoothIn,
                       const bool mergedLinesIn,
                       const outlinerreal linewidthIn,
                       const bool ySwapIn,
		       const unsigned int maxLinePointsIn) :
  multiplier(multiplierIn),
  smooth(smoothIn),
  mergedLines(mergedLinesIn),
  linewidth(linewidthIn),
  ySwap(ySwapIn),
  maxLinePoints((maxLinePointsIn < OutlinerSvgMinLinePoints ?
		 OutlinerSvgMinLinePoints :
		 (maxLinePointsIn > OutlinerSvgMaxLinePoints ? OutlinerSvgMaxLinePoints : maxLinePointsIn))) {
  assert(multiplier >= 1);
  assert(smooth == 0 || smooth == 1);
  assert(mergedLines == 0 || mergedLines == 1);
  assert(linewidth > 0.0);
  assert(ySwap == 0 || ySwap == 1);
  assert(maxLinePoints >= OutlinerSvgMinLinePoints);
  assert(maxLinePoints <= OutlinerSvgMaxLinePoints);
}

SvgOptions::SvgOptions(const SvgOptions& input) :
  multiplier(input.multiplier),
  smooth(input.smooth),
  mergedLines(input.mergedLines),
  linewidth(input.linewidth),
  ySwap(input.ySwap),
  maxLinePoints(input.maxLinePoints) {
  assert(input.maxLinePoints >= OutlinerSvgMinLinePoints);
  assert(input.maxLinePoints <= OutlinerSvgMaxLinePoints);
  assert(maxLinePoints >= OutlinerSvgMinLinePoints);
  assert(maxLinePoints <= OutlinerSvgMaxLinePoints);
}

SvgOptions&
SvgOptions::operator=(const SvgOptions& input) {
  multiplier = input.multiplier;
  smooth = input.smooth;
  mergedLines = input.mergedLines;
  linewidth = input.linewidth;
  ySwap = input.ySwap;
  maxLinePoints = input.maxLinePoints;
  assert(input.maxLinePoints >= OutlinerSvgMinLinePoints);
  assert(input.maxLinePoints <= OutlinerSvgMaxLinePoints);
  assert(maxLinePoints >= OutlinerSvgMinLinePoints);
  assert(maxLinePoints <= OutlinerSvgMaxLinePoints);
  return(*this);
}

SvgOptions::~SvgOptions() {
  debugf("SvgOptions::~SvgOptions");
}



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
#include "outlinerprocessor.hh"
#include "outlinerdebug.hh"
#include "outlinerprocessoroptions.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class functions ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

ProcessorOptions::ProcessorOptions() :
  floorStyleDiff(1),
  tunnelSpine(0),
  labels(0),
  formAnalysis(0),
  formCondense(1),
  dimensions(0) {
}

ProcessorOptions::ProcessorOptions(const unsigned int holeThresholdIn,
                                   const unsigned int lineHoleThresholdIn,
                                   const unsigned int dustThresholdIn,
                                   const char* floorDepthMapIn,
                                   const char* roofDepthMapIn,
                                   const bool floorStyleDiffIn,
                                   const bool tunnelSpineIn,
                                   const bool labelsIn,
                                   const bool formAnalysisIn,
                                   const unsigned int formCondenseIn,
                                   const bool dimensionsIn) :
  holeThreshold(holeThresholdIn),
  lineHoleThreshold(lineHoleThresholdIn),
  dustThreshold(dustThresholdIn),
  floorDepthMap(floorDepthMapIn),
  roofDepthMap(roofDepthMapIn),
  floorStyleDiff(floorStyleDiffIn),
  tunnelSpine(tunnelSpineIn),
  labels(labelsIn),
  formAnalysis(formAnalysisIn),
  formCondense(formCondenseIn),
  dimensions(dimensionsIn) {
}

ProcessorOptions&
ProcessorOptions::operator=(const ProcessorOptions& input) {
  holeThreshold = input.holeThreshold;
  lineHoleThreshold = input.lineHoleThreshold;
  dustThreshold = input.dustThreshold;
  floorDepthMap = input.floorDepthMap;
  roofDepthMap = input.roofDepthMap;
  floorStyleDiff = input.floorStyleDiff;
  tunnelSpine = input.tunnelSpine;
  labels = input.labels;
  formAnalysis = input.formAnalysis;
  formCondense = input.formCondense;
  dimensions = input.dimensions;
  return(*this);
}

ProcessorOptions::~ProcessorOptions() {
  debugf("ProcessorOptions::~ProcessorOptions");
}


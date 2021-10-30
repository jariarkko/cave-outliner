
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
#include "mainconfig.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class functions ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

MainConfig::MainConfig() :
  test(0),
  info(1),
  debug(0),
  deepdebug(0),
  deepdeepdebug(0),
  stepx(1.0),
  stepy(1.0),
  stepz(1.0),
  boundingBoxSet(0),
  direction(dir_z),
  algorithm(alg_pixel),
  linewidth( outlinerdefaultlinewidth),
  multiplier(1),
  smooth(0),
  mergedLines(1),
  tiles(outlinertiledivision),
  holethreshold(0),
  automaticCrossSections(0),
  nAutomaticCrossSections(0),
  automaticCrossSectionFilenamePattern(0),
  nCrossSections(0),
  labelCrossSections(0) {
}

MainConfig::~MainConfig() {
  if (automaticCrossSectionFilenamePattern != 0) free((void*)automaticCrossSectionFilenamePattern);
  automaticCrossSectionFilenamePattern = 0;
  for (unsigned int i = 0; i < nCrossSections; i++) {
    struct ProcessorCrossSectionInfo& one = crossSections[i];
    if (one.filename != 0) free((void*)one.filename);
    one.filename = 0;
    if (one.label != 0) free((void*)one.label);
    one.label = 0;
  }
}


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
#include "mainconfig.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class functions ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

MainConfig::MainConfig() :
  inputFile(0),
  outputFile(0),
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
  svgYSwap(1),
  tiles(outlinertiledivision),
  holethreshold(0),
  lineholethreshold(0),
  dustThreshold(10),
  automaticCrossSections(0),
  automaticCrossSectionsDirection(dir_x),
  nAutomaticCrossSections(0),
  automaticCrossSectionFilenamePattern(0),
  nCrossSections(0),
  crossSectionWidth(1.0),
  tunnelSpine(0),
  labelCrossSections(0),
  formAnalysis(0),
  formCondense(1),
  dimensions(0),
  crossSectionLabelCount(0) {
}

MainConfig::~MainConfig() {
  if (inputFile != 0) free((void*)inputFile);
  inputFile = 0;
  if (outputFile != 0) free((void*)outputFile);
  outputFile = 0;
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

///////////////////////////////////////////////////////////////////////////////////////////////
// Cross section labels ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

const char*
 MainConfig::getCrossSectionLabel(void) {
  
  if (labelCrossSections) {
    char buf[20];
    const unsigned int nAlphabet = 25;
    memset(buf,0,sizeof(buf));
    if (crossSectionLabelCount <= nAlphabet) {
      snprintf(buf,sizeof(buf)-1,"%c",
               ('A' + crossSectionLabelCount));
    } else {
      snprintf(buf,sizeof(buf)-1,"%c%u",
               ('A' + (crossSectionLabelCount % nAlphabet)),
               1 + (crossSectionLabelCount / nAlphabet));
    }
    crossSectionLabelCount++;
    const char* result = strdup(buf);
    if (result == 0) {
      errf("Cannot allocate a string of %u bytes", strlen(buf));
      exit(1);
    }
    return(result);
  } else {
    return(0);
  }
}
                

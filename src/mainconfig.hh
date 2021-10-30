
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

#ifndef MAINCONFIG_HH
#define MAINCONFIG_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// The MainConfig class represents an object of all configuration
/// information related to the cave-outliner program. It is
/// initialized to default values, and the main program can read
/// options and other command line arguments to set specific values in
/// the configuration. Once the processing of the actual function in
/// cave-outliner begins, the configuration data is read from this
/// object to decide how to process.
///

class MainConfig {

public:
  MainConfig();
  
  bool test;
  bool info;
  bool debug;
  bool deepdebug;
  bool deepdeepdebug;
  float stepx;
  float stepy;
  float stepz;
  bool boundingBoxSet;
  OutlinerBox3D boundingBox;
  enum outlinerdirection direction;
  enum outlineralgorithm algorithm;
  float linewidth;
  unsigned int multiplier;
  bool smooth;
  bool mergedLines;
  unsigned int tiles;
  unsigned int holethreshold;
  bool automaticCrossSections;
  unsigned int nAutomaticCrossSections;
  const char* automaticCrossSectionFilenamePattern;
  unsigned int nCrossSections;
  struct ProcessorCrossSectionInfo crossSections[outlinermaxcrosssections];
  bool labelCrossSections;
};

#endif // MAINCONFIG_HH

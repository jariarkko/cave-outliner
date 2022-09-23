
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

#ifndef SVGOPTIONS_HH
#define SVGOPTIONS_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include "outlinertypes.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// The SvgOptions class represents an object of all configuration
/// information related to the cave-outliner's output SVG images. It is
/// initialized to default values, and the main program can read
/// options and other command line arguments to set specific values in
/// the configuration.
///

class SvgOptions {

public:

  /// Create the configuration object, with default values.
  SvgOptions();
  
  /// Create the configuration object, with given values.
  SvgOptions(const unsigned int multiplierIn,
             const bool smoothIn,
             const bool mergedLinesIn,
             const outlinerreal linewidthIn,
             const bool ySwapIn,
	     const unsigned int maxLinePointsIn);

  /// Create the configuration object, as a copy of another object
  SvgOptions(const SvgOptions& input);
  
  /// Assignment
  SvgOptions& operator=(const SvgOptions& input);
  
  /// Release all resources for the configuration object.
  ~SvgOptions();
  
  unsigned int multiplier;
  bool smooth;
  bool mergedLines;
  outlinerreal linewidth;
  bool ySwap;
  unsigned int maxLinePoints;
  
};

#endif // SVGOPTIONS_HH

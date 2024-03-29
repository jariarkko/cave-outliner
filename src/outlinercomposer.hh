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

#ifndef COMPOSER_HH
#define COMPOSER_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"
#include "outlinertemp.hh"
#include "outlinersvg.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class Composer {

public:
  
  Composer(const char* outputFileIn,
	   unsigned int verticalSpacingIn,
	   const SvgOptions& svgOptionsIn,
	   const char* nameIn,
	   const char* locationIn,
	   const char* coordinatesIn,
	   const char* lengthIn,
	   const char* surveyerIn,
	   const char* surveyToolIn,
	   const char* surveyDateIn,
	   const char* mapDateIn,
	   const char* planViewFileIn,
	   unsigned int nCrossSectionFilesIn,
	   const char** crossSectionFilesIn,
	   TempFiler& tempFilerIn);
  ~Composer();
  bool compose(void);
  static void test(TempFiler& tempFiler);
  
private:

  /// Where daes the resulting image file go?
  const char* outputFile;

  /// How much vertical space should there be between the composite images?
  unsigned int verticalSpacing;

  /// All SVG-related options.
  const SvgOptions& svgOptions;
  
  /// What is the name of the cave?
  const char* name;
  
  /// Where is the cave? What city or municipality or country?
  const char* location;
  
  /// Where is the cave? What are the coordinates?
  const char* coordinates;
  
  /// How long is the cave?
  const char* length;
  
  /// Who performed the 3D scan?
  const char* surveyer;
  
  /// What tool was used in the 3D scan?
  const char* surveyTool;
  
  /// When was the survey performed?
  const char* surveyDate;

  /// When was the map drawn (perhaps generated by this tool)?
  const char* mapDate;

  /// Where is the plan view image file?
  const char* planViewFile;

  /// How many cross section files are there?
  unsigned int nCrossSectionFiles;

  /// Where are the cross section image files?
  const char** crossSectionFiles;

  /// Object to manage temporary files.
  TempFiler& tempFiler;

  /// Create a basic (name, surveyer, tools) image.
  bool makeBasicInfoImage(const char* nameImageFile,
			  unsigned int& xOffsetPixel);

  /// Calculate used font sizes.
  void fontSizeMultipliers(unsigned int inputFontSize,
			   unsigned int& actualFontSize,
			   unsigned int& actualFontHeight) const;
};

#endif // OUTLINERCOMPOSER_HH

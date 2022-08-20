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
#include "outlinerhighprecision.hh"
#include "outlinermath.hh"
#include "outlinerdebug.hh"
#include "outlinersvg.hh"
#include "outlinersvgstacker.hh"
#include "outlinerversion.hh"
#include "outlinercomposer.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors ///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

Composer::Composer(const char* outputFileIn,
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
		   TempFiler& tempFilerIn) :
  outputFile(outputFileIn),
  verticalSpacing(verticalSpacingIn),
  svgOptions(svgOptionsIn),
  name(nameIn != 0 ? nameIn : "Unknown Cave"),
  location(locationIn),
  coordinates(coordinatesIn),
  length(lengthIn),
  surveyer(surveyerIn),
  surveyTool(surveyToolIn),
  surveyDate(surveyDateIn),
  mapDate(mapDateIn),
  planViewFile(planViewFileIn),
  nCrossSectionFiles(nCrossSectionFilesIn),
  crossSectionFiles(crossSectionFilesIn),
  tempFiler(tempFilerIn) {
}

Composer::~Composer() {
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Actual operation ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
Composer::compose(void) {

  // Debugs
  infof("Composing an image of all map parts to %s...", outputFile);
  
  // Create an image for the basic information
  char* nameImageFile = tempFiler.createTempFile("basicinfo","svg");
  if (nameImageFile == 0) {
    return(0);
  }
  unsigned int xOffsetPixel;
  if (!makeBasicInfoImage(nameImageFile,xOffsetPixel)) {
    return(0);
  }
  
  // Compose all images together
  unsigned int nInputImages = 2 + nCrossSectionFiles;
  const char** inputImages = new const char* [nInputImages];
  unsigned int* inputImageIndents = new unsigned int [nInputImages];
  inputImages[0] = nameImageFile;
  inputImageIndents[0] = 0;
  inputImages[1] = planViewFile;
  inputImageIndents[1] = xOffsetPixel;
  for (unsigned int i = 0; i < nCrossSectionFiles; i++) {
    inputImages[2+i] = crossSectionFiles[i];
    inputImageIndents[2+i] = xOffsetPixel;
  }
  SvgOptions options;
  SvgStacker newSvg(outputFile,nInputImages,inputImages,inputImageIndents,options,verticalSpacing);
  if (!newSvg.ok()) {
    errf("Unable to write SVG file to %s", outputFile);
    delete [] inputImages;
    delete [] inputImageIndents;
    free(nameImageFile);
    return(0);
  }
  
  // Done
  delete [] inputImages;
  delete [] inputImageIndents;
  free(nameImageFile);
  return(1);
}

void
Composer::fontSizeMultipliers(unsigned int inputFontSize,
			      unsigned int& actualFontSize,
			      unsigned int& actualFontHeight) const {
  assert(inputFontSize > 0);
  assert(svgOptions.multiplier >= 1.0);
  actualFontSize = (unsigned int)(inputFontSize * sqrt(svgOptions.multiplier * 1.0));
  actualFontHeight = (unsigned int)(actualFontSize / (svgOptions.multiplier * 1.0));
  debugf("  font size multipliers %u results in %u and %u based on multiplier %u",
	 inputFontSize,
	 actualFontSize,
	 actualFontHeight,
	 svgOptions.multiplier);
}

bool
Composer::makeBasicInfoImage(const char* nameImageFile,
			     unsigned int& xOffsetPixel) {
  infof("  Creating a basic information image to %s...", nameImageFile);
  unsigned int actualFontSize;
  unsigned int actualFontHeight;
  unsigned int actualLargeFontSize;
  unsigned int actualLargeFontHeight;
  fontSizeMultipliers(outlinerdefaultfontysize,actualFontSize,actualFontHeight);
  fontSizeMultipliers(outlinerdefaultfontysizelarge,actualLargeFontSize,actualLargeFontHeight);
  outlinerreal xSize = outlinerbasicinfospacex / sqrt(svgOptions.multiplier * 1.0);
  outlinerreal ySize =
    3 * (outlinerbasicinfospaceempty / (svgOptions.multiplier * 1.0)) +
    actualLargeFontHeight +
    (((location != 0) + (coordinates != 0) + (length != 0) + (surveyer != 0) + (mapDate != 0)) * actualFontHeight);
  debugf("  basic info size %.2f x %.2f, lse %.2f to %.2f",
	 xSize, ySize,
	 outlinerbasicinfospaceempty * 1.0,
	 (outlinerbasicinfospaceempty / (svgOptions.multiplier * 1.0)));
  SvgCreator nameImage(nameImageFile,
		       xSize,
		       ySize,
		       0,
		       0,
		       1,
		       1,
		       svgOptions);
  outlinerreal xPosition = outlinerdefaultfontxsize;
  outlinerreal yPosition = ySize - (outlinerbasicinfospaceempty / (svgOptions.multiplier * 1.0)) - actualLargeFontHeight;
  nameImage.getCoordinateXPixel(xPosition,xOffsetPixel);
  nameImage.text(xPosition,yPosition,name,actualLargeFontSize);
  yPosition -= actualFontHeight + (outlinerbasicinfospaceempty / (svgOptions.multiplier * 1.0));
  char buf[1000];
  memset(buf,0,sizeof(buf));
  if (location != 0) {
    snprintf(buf,sizeof(buf)-1,"Location: %s", location);
    nameImage.text(xPosition,yPosition,buf,actualFontSize);
    yPosition -= actualFontHeight;
  }
  if (coordinates != 0) {
    snprintf(buf,sizeof(buf)-1,"Coordinates: %s", coordinates);
    nameImage.text(xPosition,yPosition,buf,actualFontSize);
    yPosition -= actualFontHeight;
  }
  if (length != 0) {
    snprintf(buf,sizeof(buf)-1,"Length: %s", length);
    nameImage.text(xPosition,yPosition,buf,actualFontSize);
    yPosition -= actualFontHeight;
  }
  if (surveyer != 0) {
    snprintf(buf,sizeof(buf)-1,"Survey: %s", surveyer);
    if (surveyTool != 0) {
      snprintf(buf+strlen(buf),sizeof(buf)-strlen(buf)-1," with %s",surveyTool);
    }
    if (surveyDate != 0) {
      snprintf(buf+strlen(buf),sizeof(buf)-strlen(buf)-1," on %s",surveyDate);
    }
    nameImage.text(xPosition,yPosition,buf,actualFontSize);
    yPosition -= actualFontHeight;
  }
  if (mapDate != 0) {
    snprintf(buf,sizeof(buf)-1,"Map: Made with Cave Outliner %s on %s", outlinerVersion, mapDate);
    nameImage.text(xPosition,yPosition,buf,actualFontSize);
    yPosition -= actualFontHeight;
  }
  if (!nameImage.ok()) {
    errf("Cannot write basic information image %s", nameImageFile);
    return(0);
  }

  // Done
  return(1);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Unit tests /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
Composer::test(TempFiler& tempFiler) {
  SvgOptions svgOptions;
  Composer tester("test.svg",0,svgOptions,"name","loc","N 1 E 2","too long","me","lazer!!!","1.1.1970","now","input.svg",0,0,tempFiler);
}

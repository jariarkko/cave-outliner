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
  infof("  Creating a basic information image to %s...", nameImageFile);
  if (nameImageFile == 0) return(0);
  outlinerreal xSize = outlinerbasicinfospacex;
  outlinerreal ySize =
    outlinerdefaultfontysizelarge +
    outlinerdefaultfontysizelarge +
    ((location != 0) + (coordinates != 0) + (surveyer != 0) + (mapDate != 0)) * outlinerdefaultfontysize;
  SvgCreator nameImage(nameImageFile,
		       xSize,
		       ySize,
		       0,
		       0,
		       1,
		       1,
		       svgOptions);
  outlinerreal xPosition = outlinerdefaultfontxsize;
  outlinerreal yPosition = ySize - outlinerdefaultfontysizelarge - outlinerdimensionlinespaceempty;
  nameImage.text(xPosition,yPosition,name,outlinerdefaultfont);
  yPosition -= outlinerdefaultfontysizelarge + outlinerdimensionlinespaceempty;
  char buf[1000];
  memset(buf,0,sizeof(buf));
  if (location != 0) {
    snprintf(buf,sizeof(buf)-1,"Location: %s", location);
    nameImage.text(xPosition,yPosition,buf,outlinersmallfont);
    yPosition -= outlinerdefaultfontysize + outlinerdimensionlinespaceempty;
  }
  if (coordinates != 0) {
    snprintf(buf,sizeof(buf)-1,"Coordinates: %s", coordinates);
    nameImage.text(xPosition,yPosition,buf,outlinersmallfont);
    yPosition -= outlinerdefaultfontysize + outlinerdimensionlinespaceempty;
  }
  if (surveyer != 0) {
    snprintf(buf,sizeof(buf)-1,"Survey: %s", surveyer);
    if (surveyTool != 0) {
      snprintf(buf+strlen(buf),sizeof(buf)-strlen(buf)-1," with %s",surveyTool);
    }
    if (surveyDate != 0) {
      snprintf(buf+strlen(buf),sizeof(buf)-strlen(buf)-1," on %s",surveyDate);
    }
    nameImage.text(xPosition,yPosition,buf,outlinersmallfont);
    yPosition -= outlinerdefaultfontysize + outlinerdimensionlinespaceempty;
  }
  if (mapDate != 0) {
    snprintf(buf,sizeof(buf)-1,"Map: Made with Cave Outliner %s on %s", outlinerVersion, mapDate);
    nameImage.text(xPosition,yPosition,buf,outlinersmallfont);
    yPosition -= outlinerdefaultfontysize + outlinerdimensionlinespaceempty;
  }
  if (!nameImage.ok()) {
    errf("Cannot write basic information image %s", nameImageFile);
    return(0);
  }
  
  // Compose all images together
  unsigned int nInputImages = 1 + nCrossSectionFiles;
  const char** inputImages = new const char* [nInputImages];
  inputImages[0] = planViewFile;
  for (unsigned int i = 0; i < nCrossSectionFiles; i++) {
    inputImages[1+i] = crossSectionFiles[i];
  }
  SvgOptions options;
  SvgStacker newSvg(outputFile,nInputImages,inputImages,options,verticalSpacing);
  if (!newSvg.ok()) {
    errf("Unable to write SVG file to %s", outputFile);
    delete [] inputImages;
    free(nameImageFile);
    return(0);
  }
  
  // Done
  delete [] inputImages;
  free(nameImageFile);
  return(1);
}
  
///////////////////////////////////////////////////////////////////////////////////////////////
// Unit tests /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
Composer::test(TempFiler& tempFiler) {
  SvgOptions svgOptions;
  Composer tester("test.svg",0,svgOptions,"name","loc","N 1 E 2","me","lazer!!!","1.1.1970","now","input.svg",0,0,tempFiler);
}

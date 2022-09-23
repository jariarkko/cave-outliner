
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
#include <stdlib.h>
#include <pwd.h>
#include <string.h>
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"
#include "outlinerhighprecision.hh"
#include "outlinerprocessor.hh"
#include "outlinerdebug.hh"
#include "mainconfig.hh"
#include "outlinersvg.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class functions ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

MainConfig::MainConfig() :
  inputFile(0),
  planViewOutputFile(0),
  compositeOutputFile(0),
  test(0),
  nThreads(outlinerdefaultthreads),
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
  linewidth(outlinerdefaultlinewidth),
  multiplier(1),
  smooth(0),
  mergedLines(1),
  svgYSwap(1),
  svgMaxLinePoints(OutlinerSvgDefaultLinePoints),
  tiles(outlinertiledivision),
  holeThreshold(0),
  lineHoleThreshold(0),
  dustThreshold(10),
  automaticCrossSections(0),
  automaticCrossSectionsDirection(dir_x),
  nAutomaticCrossSections(0),
  automaticCrossSectionFilenamePattern(0),
  nCrossSections(0),
  crossSectionWidth(1.0),
  floorDepthMap(0),
  roofDepthMap(0),
  floorStyleDiff(1),
  tunnelSpine(0),
  labelCrossSections(0),
  formAnalysis(0),
  formCondense(1),
  name(0),
  surveyer(getUserRealName()),
  surveyTool("iPhone 12 Pro"),
  surveyDate(0),
  mapDate(getCurrentDate()),
  location(0),
  coordinates(0),
  length(0),
  compositeMap(0),
  dimensions(0),
  crossSectionLabelCount(0) {
}

MainConfig::~MainConfig() {
  debugf("MainConfig::~MainConfig start");
  if (inputFile != 0) free((void*)inputFile);
  inputFile = 0;
  if (planViewOutputFile != 0) free((void*)planViewOutputFile);
  planViewOutputFile = 0;
  if (compositeOutputFile != 0) free((void*)compositeOutputFile);
  compositeOutputFile = 0;
  if (automaticCrossSectionFilenamePattern != 0) free((void*)automaticCrossSectionFilenamePattern);
  automaticCrossSectionFilenamePattern = 0;
  for (unsigned int i = 0; i < nCrossSections; i++) {
    struct ProcessorCrossSectionInfo& one = crossSections[i];
    if (one.filename != 0) free((void*)one.filename);
    one.filename = 0;
    if (one.label != 0) free((void*)one.label);
    one.label = 0;
  }
  debugf("MainConfig::~MainConfig done");
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Helper functions ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

const char*
MainConfig::getUserRealName() {

  // Find out which user runs the software
  const char* user = getenv("USER");
  if (user == 0) {
    warnf("Cannot find current user id");
    return(0);
  }

  // Get the password entry for the user
  struct passwd entrySpace;
  const unsigned bufSize = 1024;
  char buffer[bufSize];
  struct passwd *entry = 0;
  if (getpwnam_r(user,&entrySpace,buffer,bufSize,&entry) < 0 || entry == 0) {
    warnf("Cannot find current user entry for user id %s", user);
    return(0);
  }
  
  // See if there's a real user name in the entry
  if (strlen(entry->pw_gecos) == 0) {
    warnf("No real name associated with user id %s", user);
    return(0);
  }

  // Get the real name part (before comma)
  size_t n = strcspn(entry->pw_gecos, ",");
  if (n > maxRealNameLength - 1) {
    n = maxRealNameLength - 1;
  }
  memset(realNameBuffer,0,maxRealNameLength);
  strncpy(realNameBuffer,entry->pw_gecos,n);
  return(realNameBuffer);
}

const char*
MainConfig::getCurrentDate() {
  time_t now = time(0);
  convertToDate(now,dateBuffer,sizeof(dateBuffer));
  return(dateBuffer);
}

void
MainConfig::convertToDate(time_t when,
			  char* buf,
			  size_t bufSize) {
  assert(bufSize > 0);
  static const char* months[12] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
  };
  struct tm* decoded = localtime(&when);
  assert(decoded->tm_mday <= 31);
  assert(decoded->tm_mon <= 11);
  memset(buf,0,bufSize);
  snprintf(buf,bufSize-1,"%s %u, %04u",
	   months[decoded->tm_mon],
	   decoded->tm_mday,
	   1900 + decoded->tm_year);
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
      fatalf("Cannot allocate a string of %u bytes", strlen(buf));
      return(0);
    }
    return(result);
  } else {
    return(0);
  }
}
                

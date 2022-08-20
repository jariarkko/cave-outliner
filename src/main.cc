
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

#include <stdarg.h>
#include <cassert>
#include <string.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"
#include "outlinerhighprecision.hh"
#include "outlinerdebug.hh"
#include "outlinerindexedmesh.hh"
#include "outlinerprocessor.hh"
#include "outlinerdescribe.hh"
#include "outlinerboundingboxer.hh"
#include "outlinersvg.hh"
#include "outlinerversion.hh"
#include "outlinercomposer.hh"
#include "outlinertemp.hh"
#include "mainconfig.hh"
#include "mainoptions.hh"
#include "maintest.hh"
#include "main.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static void setProcessorOptions(const MainConfig& config,
                                ProcessorOptions& options);
static void setSvgOptions(const MainConfig& config,
                          SvgOptions& options);
static const aiScene* processImport(Assimp::Importer& importer,
                                    const char* file);
static char* makeFilenameFromPattern(const char* pattern,
                                     unsigned int index);

///////////////////////////////////////////////////////////////////////////////////////////////
// Main program and option handling ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int
main(int argc, char** argv) {

  // Process command line options and arguments
  TempFiler tempFiler;
  MainConfig config;
  MainOptions options(config,tempFiler);
  if (!options.processCommandLine(argc,argv)) {
    return(1);
  }
  
  // Initialize debugs
  debuginit(config.info,config.debug,config.deepdebug,config.deepdeepdebug);
  
  // Run tests if needed
  if (config.test) {
    debugf("running tests");
    MainTest tester(tempFiler);
    tester.test();
    debugf("done with tests");
    return(0);
  }
  
  // Check input and output file names, if we have them. Otherwise we are done.
  if (config.inputFile == 0 || config.planViewOutputFile == 0) {
    return(0);
  }
  
  // Initialize debug
  debuginit(config.info,config.debug,config.deepdebug,config.deepdeepdebug);
  
  // Import the model
  infof("Importing the model...");
  Assimp::Importer importer;
  const aiScene* scene = processImport(importer,config.inputFile);
  if (scene == 0) return(1);
  
  // Describe the model if needed
  if (config.deepdebug) {
    Describer desc(config.deepdebug,config.deepdeepdebug,config.deepdeepdebug,config.deepdeepdebug);
    desc.describeScene(scene);
  }
  
  // Determine bounding box, if not specified
  BoundingBoxer boxer(scene,
                      config.stepx,
                      config.stepy,
                      config.stepz);
  OutlinerBox3D originalBoundingBox;
  boxer.getOriginalBoundingBox(originalBoundingBox);
  if (!config.boundingBoxSet) {
    boxer.getIncreasedBoundingBox(config.boundingBox);
  }
  
  // Derive some size information
  outlinerreal xOutputStart = DirectionOperations::outputx(config.direction,config.boundingBox.start);
  outlinerreal xOutputEnd = DirectionOperations::outputx(config.direction,config.boundingBox.end);
  outlinerreal yOutputStart = DirectionOperations::outputy(config.direction,config.boundingBox.start);
  outlinerreal yOutputEnd = DirectionOperations::outputy(config.direction,config.boundingBox.end);
  
  // Check if we need to make cross sections
  if (config.automaticCrossSections) {
    if (config.nCrossSections + config.nAutomaticCrossSections >= outlinermaxcrosssections) {
      errf("Maximum number of cross sections (%u) reached", outlinermaxcrosssections);
      return(1);
    }
    outlinerreal crossSectionStep;
    outlinerreal crossSectionStart;
    switch (config.automaticCrossSectionsDirection) {
    case dir_x:
      crossSectionStep = (xOutputEnd - xOutputStart) / (1.0*config.nAutomaticCrossSections);
      crossSectionStart = xOutputStart;
      break;
    case dir_y:
      crossSectionStep = (yOutputEnd - yOutputStart) / (1.0*config.nAutomaticCrossSections);
      crossSectionStart = yOutputStart;
      break;
    case dir_z:
      errf("Not supported cross section type");
      return(1);
    default:
      errf("Invalid direction");
      return(1);
    }
    for (unsigned int c = 0; c < config.nAutomaticCrossSections; c++) {
      assert(config.nCrossSections < outlinermaxcrosssections);
      struct ProcessorCrossSectionInfo* newOne = &config.crossSections[config.nCrossSections];
      config.crossSectionPoints[config.nCrossSections] =
        crossSectionStart + crossSectionStep * (((outlinerreal)c)+0.5);
      config.crossSectionDirections[config.nCrossSections] = config.automaticCrossSectionsDirection;
      newOne->line.start.x = 0;
      newOne->line.start.y = 0;
      newOne->line.end.x = 0;
      newOne->line.end.y = 0;
      newOne->filename = makeFilenameFromPattern(config.automaticCrossSectionFilenamePattern,c);
      newOne->width = config.crossSectionWidth;
      debugf("used cross section width %.2f", newOne->width);
      newOne->label = config.getCrossSectionLabel();
      debugf("    cross section %s file %s at %.2f",
             (newOne->label == 0 ? "(none)" : newOne->label),
             newOne->filename,
             config.crossSectionPoints[config.nCrossSections]);
      config.nCrossSections++;
    }
  }
  
  // Assign the cross section line endpoints, as we now know the size
  // of the image
  for (unsigned int c = 0; c < config.nCrossSections; c++) {
    struct ProcessorCrossSectionInfo* thisOne = &config.crossSections[c];
    switch (config.crossSectionDirections[c]) {
    case dir_x:
      thisOne->line.start.x = config.crossSectionPoints[c];
      thisOne->line.start.y = yOutputStart;
      thisOne->line.end.x = thisOne->line.start.x;
      thisOne->line.end.y = yOutputEnd;
      break;
    case dir_y:
      thisOne->line.start.x = xOutputStart;
      thisOne->line.start.y = config.crossSectionPoints[c];
      thisOne->line.end.x = xOutputEnd;
      thisOne->line.end.y = thisOne->line.start.y;
      break;
    case dir_z:
      errf("Not supported cross section type");
      return(1);
    default:
      errf("Invalid type");
      return(1);
    }
    infof("Configured a %s cross section from (%.2f,%.2f) to (%.2f,%.2f)",
	  config.crossSectionDirections[c] == dir_x ? "x" : "y",
          thisOne->line.start.x, thisOne->line.start.y,
          thisOne->line.end.x, thisOne->line.end.y);
  }

  // Build our own data structure
  OutlinerVector2D bounding2DBoxStart(xOutputStart,yOutputStart);
  OutlinerVector2D bounding2DBoxEnd(xOutputEnd,yOutputEnd);
  OutlinerBox2D bounding2DBox(bounding2DBoxStart,bounding2DBoxEnd);
  const outlinerreal minStepsPerTile = 3.0;
  const outlinerreal xSteps = (xOutputEnd - xOutputStart) / config.stepx;
  const outlinerreal ySteps = (yOutputEnd - yOutputStart) / config.stepy;
  const outlinerreal xStepsPerTile = xSteps / ((outlinerreal)config.tiles);
  const outlinerreal yStepsPerTile = ySteps / ((outlinerreal)config.tiles);
  bool tilesChanged = 0;
  infof("Tiles check %f %f %f", xStepsPerTile, yStepsPerTile, minStepsPerTile);
  if (xStepsPerTile < minStepsPerTile) {
    config.tiles = ((unsigned int)(xSteps / minStepsPerTile));
    if (config.tiles < 1) config.tiles = 1;
    tilesChanged = 1;
  }
  if (yStepsPerTile < minStepsPerTile) {
    config.tiles = ((unsigned int)(ySteps / minStepsPerTile));
    if (config.tiles < 1) config.tiles = 1;
    tilesChanged = 1;
  }
  if (tilesChanged) {
    infof("Adjusted number of tiles to %u to ensure enough pixels within a tile", config.tiles);
  }
  IndexedMesh indexed(outlinermaxmeshes,config.tiles,
                      config.boundingBox,
                      bounding2DBox,
                      config.direction);
  indexed.addScene(scene);
  if (config.debug) {
    indexed.describe(std::cout);
  }
  
  // Find the options for the processor and SVG output
  ProcessorOptions processorOptions;
  SvgOptions svgOptions;
  setProcessorOptions(config,processorOptions);
  setSvgOptions(config,svgOptions);
  
  // Process the model
  Processor processor(config.planViewOutputFile,
                      processorOptions,
                      svgOptions,
                      originalBoundingBox,
                      config.boundingBox,
                      config.stepx,
                      config.stepy,
                      config.stepz,
                      config.direction,
                      config.algorithm,
                      config.nCrossSections,
                      config.crossSections,
                      indexed);
  if (!processor.processScene(scene)) {
    return(1);
  }

  // Do we need to construct a composite map file, from planview and
  // cross-sections? If yes, do it based on the previously created
  // files.
  if (config.compositeMap) {
    const char* crossSectionFiles[outlinermaxcrosssections];
    assert(config.nCrossSections <= outlinermaxcrosssections);
    for (unsigned int i = 0; i < config.nCrossSections; i++) {
      crossSectionFiles[i] = config.crossSections[i].filename;
    }
    Composer composer(config.compositeOutputFile,
		      outlinerspacingbetweencompositeimages,
		      svgOptions,
		      config.name,
		      config.location,
		      config.coordinates,
		      config.length,
		      config.surveyer,
		      config.surveyTool,
		      config.surveyDate,
		      config.mapDate,
		      config.planViewOutputFile,
		      config.nCrossSections,
		      crossSectionFiles,
		      tempFiler);
    if (!composer.compose()) {
      return(1);
    }
  }
  
  // Done
  infof("Done");
  return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Model file IO //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static const aiScene*
processImport(Assimp::Importer& importer,
              const char* fileName) {
  
  debugf("processImport");
  
  // Have the importer read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // probably to request more postprocessing than we do in this example.
  const aiScene* scene = importer.ReadFile(fileName,
                                           aiProcess_CalcTangentSpace       |
                                           aiProcess_Triangulate            |
                                           aiProcess_JoinIdenticalVertices  |
                                           aiProcess_SortByPType
                                           /* aiProcess_GenBoundingBoxes */);
  
  // If the import failed, report it
  if (scene == 0) {
    const char* errorString = importer.GetErrorString();
    errf("Import failed: %s", errorString);
    return(0);
  }

  return(scene);

}

///////////////////////////////////////////////////////////////////////////////////////////////
// Configuration conversions //////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static void
setProcessorOptions(const MainConfig& config,
                    ProcessorOptions& options) {
  options.holeThreshold = config.holeThreshold;
  options.lineHoleThreshold = config.lineHoleThreshold;
  options.dustThreshold = config.dustThreshold;
  options.floorDepthMap = config.floorDepthMap;
  options.roofDepthMap = config.roofDepthMap;
  options.floorStyleDiff = config.floorStyleDiff;
  options.tunnelSpine = config.tunnelSpine;
  options.labels = (config.labelCrossSections && config.nCrossSections > 0);
  options.formAnalysis = config.formAnalysis;
  options.formCondense = config.formCondense;
  options.dimensions = config.dimensions;
}

static void
setSvgOptions(const MainConfig& config,
              SvgOptions& options) {
  options.multiplier = config.multiplier;
  options.smooth = config.smooth;
  options.mergedLines = config.mergedLines;
  options.linewidth = config.linewidth;
  options.ySwap = config.svgYSwap;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// File name processing ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static char*
makeFilenameFromPattern(const char* pattern,
                        unsigned int index) {
  assert(pattern != 0);
  const unsigned int maxNumLength = 10;
  const char* rest = strchr(pattern,'%');
  assert(rest != 0);
  unsigned int nFirst = rest - pattern;
  rest++;
  unsigned int newFilenameLength = nFirst + maxNumLength + strlen(rest) + 1;
  char* result = (char*)malloc(newFilenameLength);
  if (result == 0) {
    fatalf("Cannot allocate string of %u bytes", newFilenameLength);
    return(0);
  }
  memset(result,0,newFilenameLength);
  strncpy(result,pattern,nFirst);
  snprintf(&result[nFirst],maxNumLength,"%u",index);
  strncat(result,rest,newFilenameLength);
  return(result);
}




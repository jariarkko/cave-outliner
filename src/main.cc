
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
#include "outlinermaterialmatrix.hh"
#include "outlinerprocessor.hh"
#include "outlinerdescribe.hh"
#include "outlinerboundingboxer.hh"
#include "outlinermath.hh"
#include "outlinersvg.hh"
#include "outlinerversion.hh"
#include "mainconfig.hh"
#include "mainoptions.hh"
#include "main.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static const aiScene* processImport(Assimp::Importer& importer,
                                    const char* file);
static void runTests(void);
static char* makeFilenameFromPattern(const char* pattern,
                                     unsigned int index);

///////////////////////////////////////////////////////////////////////////////////////////////
// Main program and option handling ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int
main(int argc, char** argv) {

  // Process command line options and arguments
  MainConfig config;
  MainOptions options(config);
  if (!options.processCommandLine(argc,argv)) {
    return(1);
  }
  
  // Initialize debugs
  debuginit(config.info,config.debug,config.deepdebug,config.deepdeepdebug);
  
  // Run tests if needed
  if (config.test) {
    debugf("running tests");
    runTests();
    debugf("done with tests");
    return(0);
  }
  
  // Check input and output file names, if we have them. Otherwise we are done.
  if (config.inputFile == 0 || config.outputFile == 0) {
    return(0);
  }
  
  // Initialize debug
  debuginit(config.info,config.debug,config.deepdebug,config.deepdeepdebug);
  
  // Import the model
  infof("importing the model...");
  Assimp::Importer importer;
  const aiScene* scene = processImport(importer,config.inputFile);
  if (scene == 0) return(1);
  
  // Describe the model if needed
  if (config.deepdebug) {
    Describer desc(config.deepdebug,config.deepdeepdebug,config.deepdeepdebug,config.deepdeepdebug);
    desc.describeScene(scene);
  }
  
  // Determine bounding box, if not specified
  if (!config.boundingBoxSet) {
    BoundingBoxer boxer(scene,
                        config.stepx,
                        config.stepy,
                        config.stepz);
    boxer.getBoundingBox(config.boundingBox);
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
      newOne->start.x = 0;
      newOne->start.y = 0;
      newOne->end.x = 0;
      newOne->end.y = 0;
      newOne->filename = makeFilenameFromPattern(config.automaticCrossSectionFilenamePattern,c);
      newOne->width = config.crossSectionWidth;
      infof("used cross section width %.2f", newOne->width);
      newOne->label = config.getCrossSectionLabel();
      debugf("cross section %s file %s at %.2f",
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
      thisOne->start.x = config.crossSectionPoints[c];
      thisOne->start.y = yOutputStart;
      thisOne->end.x = thisOne->start.x;
      thisOne->end.y = yOutputEnd;
      break;
    case dir_y:
      thisOne->start.x = xOutputStart;
      thisOne->start.y = config.crossSectionPoints[c];
      thisOne->end.x = xOutputEnd;
      thisOne->end.y = thisOne->start.y;
      break;
    case dir_z:
      errf("Not supported cross section type");
      return(1);
    default:
      errf("Invalid type");
      return(1);
    }
    infof("configured a cross section from (%.2f,%.2f) to (%.2f,%.2f)",
          thisOne->start.x, thisOne->start.y,
          thisOne->end.x, thisOne->end.y);
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
  infof("tiles check %f %f %f", xStepsPerTile, yStepsPerTile, minStepsPerTile);
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
  
  // Process the model
  Processor processor(config.outputFile,
                      config.multiplier,
                      config.smooth,
                      config.mergedLines,
                      config.linewidth,
                      config.boundingBox,
                      config.stepx,
                      config.stepy,
                      config.stepz,
                      config.direction,
                      config.algorithm,
                      config.holethreshold,
                      config.lineholethreshold,
                      (config.labelCrossSections && config.nCrossSections > 0),
                      indexed);
  if (!processor.processScene(scene,
                              config.nCrossSections,
                              config.crossSections)) {
    return(1);
  }

  // Done
  infof("done");
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
                                           aiProcess_SortByPType            |
                                           aiProcess_GenBoundingBoxes);
  
  // If the import failed, report it
  if (scene == 0) {
    const char* errorString = importer.GetErrorString();
    errf("Import failed: %s", errorString);
    return(0);
  }

  return(scene);

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
    errf("Cannot allocate string of %u bytes", newFilenameLength);
    exit(1);
  }
  memset(result,0,newFilenameLength);
  strncpy(result,pattern,nFirst);
  snprintf(&result[nFirst],maxNumLength,"%u",index);
  strncat(result,rest,newFilenameLength);
  return(result);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Test functions /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static void
runTests(void) {
  infof("running tests");
  OutlinerVector2D::test();
  OutlinerVector3D::test();
  OutlinerLine2D::test();
  OutlinerLine3D::test();
  OutlinerBox2D::test();
  OutlinerBox3D::test();
  OutlinerTriangle2D::test();
  OutlinerTriangle3D::test();
  OutlinerMath::mathTests();
  MaterialMatrix::test();
  infof("tests ok");
}



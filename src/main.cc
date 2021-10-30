
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

///////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static const aiScene* processImport(Assimp::Importer& importer,
                                    const char* file);
static bool checkFileExtension(const char* filename,
                               const char* extension);
static void processVersion(void);
static void processHelp(void);
static void runTests(void);
static char* makeFilenameFromPattern(const char* pattern,
                                     unsigned int index);
static const char* getCrossSectionLabel(const MainConfig& config);

///////////////////////////////////////////////////////////////////////////////////////////////
// Local variables ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static MainConfig config;

///////////////////////////////////////////////////////////////////////////////////////////////
// Main program and option handling ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int
main(int argc, char** argv) {

  while (argc > 1 && argv[1][0] == '-') {
    if (strcmp(argv[1],"--quiet") == 0) {
      config.info = 0;
      debuginit(config.info,config.debug,config.deepdebug,config.deepdeepdebug);
    } else if (strcmp(argv[1],"--debug") == 0) {
      config.debug = 1;
      debuginit(config.info,config.debug,config.deepdebug,config.deepdeepdebug);
    } else if (strcmp(argv[1],"--deepdebug") == 0) {
      config.debug = 1;
      config.deepdebug = 1;
      debuginit(config.info,config.debug,config.deepdebug,config.deepdeepdebug);
    } else if (strcmp(argv[1],"--deepdeepdebug") == 0) {
      config.debug = 1;
      config.deepdebug = 1;
      config.deepdeepdebug = 1;
      debuginit(config.info,config.debug,config.deepdebug,config.deepdeepdebug);
    } else if (strcmp(argv[1],"--z") == 0) {
      config.direction = dir_z;
    } else if (strcmp(argv[1],"--x") == 0) {
      config.direction = dir_x;
    } else if (strcmp(argv[1],"--y") == 0) {
      config.direction = dir_y;
    } else if (strcmp(argv[1],"--pixel") == 0) {
      config.algorithm = alg_pixel;
      debugf("algorithm now %u", config.algorithm);
    } else if (strcmp(argv[1],"--borderpixel") == 0) {
      config.algorithm = alg_borderpixel;
      debugf("algorithm now %u", config.algorithm);
    } else if (strcmp(argv[1],"--borderline") == 0) {
      config.algorithm = alg_borderline;
      debugf("algorithm now %u", config.algorithm);
    } else if (strcmp(argv[1],"--borderactual") == 0) {
      config.algorithm = alg_borderactual;
      debugf("algorithm now %u", config.algorithm);
    } else if (strcmp(argv[1],"--crosssections") == 0 && argc > 3) {
      if (config.nCrossSections == outlinermaxcrosssections) {
        errf("Maximum number of cross sections (%u) reached", outlinermaxcrosssections);
        return(1);
      }
      int num = atoi(argv[2]);
      if (num < 1) {
        errf("A number of cross sections must be 1 or greater, %s given", argv[2]);
        return(1);
      }
      const char* filenamePattern = argv[3];
      if (strchr(filenamePattern,'%') == 0) {
        errf("File name pattern should include the %% sign, %s given", filenamePattern);
        return(1);
      }
      config.automaticCrossSections = 1;
      config.nAutomaticCrossSections = num;
      config.automaticCrossSectionFilenamePattern = filenamePattern;
      argc--;argv++;
      argc--;argv++;
    } else if (strcmp(argv[1],"--crosssection") == 0 && argc > 3) {
      if (config.nCrossSections == outlinermaxcrosssections) {
        errf("Maximum number of cross sections (%u) reached", outlinermaxcrosssections);
        return(1);
      }
      float num = atof(argv[2]);
      if (num <= 0.0) {
        errf("Cross section coordinate value needs to be a positive number, %s given", argv[2]);
        return(1);
      }
      const char* file = argv[3];
      if (strlen(file) < 1) {
        errf("Cross section file name cannot be empty, %s given", file);
        return(1);
      }
      config.crossSections[config.nCrossSections].start.x = num;
      config.crossSections[config.nCrossSections].start.y = 0;
      config.crossSections[config.nCrossSections].end.x = num;
      config.crossSections[config.nCrossSections].end.y = 0;
      config.crossSections[config.nCrossSections].filename = file;
      config.crossSections[config.nCrossSections].label = getCrossSectionLabel(config);
      config.nCrossSections++;
      argc--;argv++;
      argc--;argv++;
    } else if (strcmp(argv[1],"--label") == 0) {
      config.labelCrossSections = 1;
    } else if (strcmp(argv[1],"--linewidth") == 0 && argc > 2) {
      float num = atof(argv[2]);
      if (num <= 0.0) {
        errf("Line width value needs to be a positive number, %s given", argv[2]);
        return(1);
      }
      config.linewidth = num;
      argc--;argv++;
    } else if (strcmp(argv[1],"--multiplier") == 0 && argc > 2) {
      int num = atoi(argv[2]);
      if (num <= 0) {
        errf("Multiplier must be positive, %s given", argv[2]);
        return(1);
      }
      config.multiplier = num;
      argc--;argv++;
    } else if (strcmp(argv[1],"--smooth") == 0) {
      config.smooth = 1;
    } else if (strcmp(argv[1],"--jagged") == 0) {
      config.smooth = 0;
    } else if (strcmp(argv[1],"--holethreshold") == 0 && argc > 2) {
      int num = atoi(argv[2]);
      if (num < 0 || num > 100) {
        errf("Hole threshold value needs to be non-negative and max 100, %s given", argv[2]);
        return(1);
      }
      config.holethreshold = num;
      argc--;argv++;
    } else if (strcmp(argv[1],"--step") == 0 && argc > 2) {
      config.stepz = config.stepx = config.stepy = atof(argv[2]);
      if (config.stepx < 0.0001) {
        errf("Invalid step value");
        return(1);
      }
      argc--;argv++;
    } else if (strcmp(argv[1],"--bounding") == 0 && argc > 1 + 2*3) {
      if (!outlinerisnumber(argv[2])) { errf("bounding element #1 is not a number: %s", argv[2]); return(1); }
      if (!outlinerisnumber(argv[3])) { errf("bounding element #2 is not a number: %s", argv[3]); return(1); }
      if (!outlinerisnumber(argv[4])) { errf("bounding element #3 is not a number: %s", argv[4]); return(1); }
      if (!outlinerisnumber(argv[5])) { errf("bounding element #4 is not a number: %s", argv[5]); return(1); }
      if (!outlinerisnumber(argv[6])) { errf("bounding element #5 is not a number: %s", argv[6]); return(1); }
      if (!outlinerisnumber(argv[7])) { errf("bounding element #6 is not a number: %s", argv[7]); return(1); }
      argc--;argv++; float startx = atof(argv[1]);
      argc--;argv++; float endx = atof(argv[1]);
      argc--;argv++; float starty = atof(argv[1]);
      argc--;argv++; float endy = atof(argv[1]);
      argc--;argv++; float startz = atof(argv[1]);
      argc--;argv++; float endz = atof(argv[1]);
      if (endx - startx < 0.0001) {
        errf("Invalid bounding box x range");
        return(1);
      }
      if (endy - starty < 0.0001) {
        errf("Invalid bounding box y range");
        return(1);
      }
      if (endz - startz < 0.0001) {
        errf("Invalid bounding box z range");
        return(1);
      }
      config.boundingBoxSet = 1;
      config.boundingBox.start = OutlinerVector3D(startx,starty,startz);
      config.boundingBox.end = OutlinerVector3D(endx,endy,endz);
    } else if (strcmp(argv[1],"--tiling") == 0 && argc > 2) {
      if (atoi(argv[2]) < 1 || atoi(argv[2]) > 10000) {
        errf("Invalid tile count, must be at least one and a not too big for memory, %s given", argv[2]);
        return(1);
      }
      config.tiles = atoi(argv[2]);
      argc--;argv++;
    } else if (strcmp(argv[1],"--test") == 0) {
      config.test = 1;
    } else if (strcmp(argv[1],"--version") == 0) {
      processVersion();
      return(0);
    } else if (strcmp(argv[1],"--help") == 0) {
      processHelp();
      return(0);
    } else {
      errf("Unrecognised option: %s", argv[1]);
      return(1);
    }
    argc--;
    argv++;
  }
  
  // Run tests if needed
  if (config.test) {
    debuginit(config.info,config.debug,config.deepdebug,config.deepdeepdebug);
    runTests();
    return(0);
  }
  
  // Check input and output file names
  if (argc != 3) {
    errf("Expected two arguments, an input and output file name, got %u", argc-1);
    if (argc > 1) {
      errf("First argument was %s", argv[1]);
    }
    return(1);
  }
  const char* input = argv[1];
  const char* output = argv[2];
  if (outlineralgorithm_generatespicture(config.algorithm)) {
    if (!checkFileExtension(output,"svg")) {
      errf("Output file must be an SVG file, %s given", output);
      return(1);
    }
  }
  
  // Initialize debug
  debuginit(config.info,config.debug,config.deepdebug,config.deepdeepdebug);
  
  // Import the model
  infof("importing the model...");
  Assimp::Importer importer;
  const aiScene* scene = processImport(importer,input);
  if (scene == 0) return(1);
  
  // Describe the model if needed
  if (config.deepdebug) {
    Describer desc(config.deepdebug,config.deepdeepdebug,config.deepdeepdebug,config.deepdeepdebug);
    desc.describeScene(scene);
  }
  
  // Determine bounding box, if not specified
  if (!config.boundingBoxSet) {
    BoundingBoxer boxer(scene);
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
    outlinerreal crossSectionStep = (xOutputEnd - xOutputStart) / (1.0*config.nAutomaticCrossSections);
    for (unsigned int c = 0; c < config.nAutomaticCrossSections; c++) {
      assert(config.nCrossSections < outlinermaxcrosssections);
      char* newFilename = makeFilenameFromPattern(config.automaticCrossSectionFilenamePattern,c);
      config.crossSections[config.nCrossSections].start.x =
        config.crossSections[config.nCrossSections].end.x =
          xOutputStart + crossSectionStep * (((outlinerreal)c)+0.5);
      config.crossSections[config.nCrossSections].start.y = yOutputStart;
      config.crossSections[config.nCrossSections].end.y = yOutputEnd;
      config.crossSections[config.nCrossSections].filename = newFilename;
      config.crossSections[config.nCrossSections].label = getCrossSectionLabel(config);
      debugf("cross section %s file %s at x %.2f from y %.2f to %.2f, step was %.2f",
             (config.crossSections[config.nCrossSections].label == 0 ?
              "(none)" :
              config.crossSections[config.nCrossSections].label),
             newFilename,
             config.crossSections[config.nCrossSections].start.x,
             config.crossSections[config.nCrossSections].start.y,
             config.crossSections[config.nCrossSections].end.y,
             crossSectionStep);
      config.nCrossSections++;
    }
  } else {
    // Fix the cross section endpoint on y dimension, as we now know the size of the image
    for (unsigned int c = 0; c < config.nCrossSections; c++) {
      if (config.crossSections[config.nCrossSections].end.y == 0) {
        config.crossSections[config.nCrossSections].end.y = yOutputEnd;
      }
    }
  }

  // Build our own data structure
  OutlinerVector2D bounding2DBoxStart(xOutputStart,yOutputStart);
  OutlinerVector2D bounding2DBoxEnd(xOutputEnd,yOutputEnd);
  OutlinerBox2D bounding2DBox(bounding2DBoxStart,bounding2DBoxEnd);
  IndexedMesh indexed(outlinermaxmeshes,config.tiles,
                      config.boundingBox,
                      bounding2DBox,
                      config.direction);
  indexed.addScene(scene);
  
  // Process the model
  Processor processor(output,
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

static void
processVersion(void) {
  std::cout << "\n";
  std::cout << OUTLINER_PROG " version ";
  std::cout << outlinerVersion;
  std::cout << "\n";
  std::cout << "\n";
}

static void
processHelp(void) {
  std::cout << "\n";
  std::cout << OUTLINER_PROG " [options] inputfile outputfile\n";
  std::cout << "\n";
  std::cout << "Processes an input 3D model in STL format to a SVG picture that\n";
  std::cout << "represents the cave horizontal plane. This can be used to produce\n";
  std::cout << "maps.\n";
  std::cout << "\n";
  std::cout << "Options:\n";
  std::cout << "\n";
  std::cout << "  --bounding x x y y z z   Set the bounding box area. Default is the model's bounding box.\n";
  std::cout << "  --step i                 Set the granularity increment. Default is 1.\n";
  std::cout << "  --z                      Generate output as viewed from the z direction, i.e., showing x/y picture.\n";
  std::cout << "  --x                      Generate output as viewed from the x direction, i.e., showing z/y picture.\n";
  std::cout << "  --y                      Generate output as viewed from the y direction, i.e., showing x/z picture.\n";
  std::cout << "  --pixel                  Use the pixel output drawing algorithm (default, fills cave with pixels).\n";
  std::cout << "  --borderpixel            Use the border-only drawing algorithm, draws only the cave walls,\n";
  std::cout << "                           with pixels.\n";
  std::cout << "  --borderline             Use the border-only drawing algorithm, draws only the cave walls,\n";
  std::cout << "                           with lines.\n";
  std::cout << "  --borderactual           Use the border-only drawing algorithm, draws the cave walls using\n";
  std::cout << "                           model triangle sides.\n";
  std::cout << "  --crosssection x file    Produce also a cross section at a given x position, output to file.\n";
  std::cout << "  --crosssections n pat    Produce n cross sections at different x positions, output to files (percent\n";
  std::cout << "                           sign denotes the cross section number in the file name pattern).\n";
  std::cout << "  --label                  Label cross sections.\n";
  std::cout << "  --multiplier n           Multiply image size by n (default 1).\n";
  std::cout << "  --linewidth n            Set the width of the lines in output picture. The value can be a\n";
  std::cout << "                           decimal number.\n";
  std::cout << "  --smooth                 Set the line drawings use smooth curves.\n";
  std::cout << "  --jagged                 Set the line drawings use hard lines (default).\n";
  std::cout << "  --holethreshold n        Ignore holes in the model if they are n or less pixels.\n";
  std::cout << "  --tiling n               Optimize search process with n x n tiles. ";
  std::cout <<                            "Default is " << outlinertiledivision << ",\n";
  std::cout << "                           and --tiling 1 implies no optimization.\n";
  std::cout << "  --quiet                  Turn on informative messages (default is they are on).\n";
  std::cout << "  --debug                  Turn on debugging messages (level 0, least)\n";
  std::cout << "  --deepdebug              Turn on debugging messages (level 1)\n";
  std::cout << "  --deepdeepdebug          Turn on debugging messages (level 2, most)\n";
  std::cout << "  --version                Output version information.\n";
  std::cout << "  --help                   Print this message\n";
  std::cout << "\n";
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
// Utility functions //////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static bool
checkFileExtension(const char* filename,
                   const char* extension) {
  assert(filename != 0);
  assert(extension != 0);
  const char* foundExtension = rindex(filename,'.');
  if (foundExtension == 0) return(0);
  foundExtension++;
  if (strcasecmp(foundExtension,extension) != 0) return(0);
  else return(1);
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
// Cross section labels ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static const char*
getCrossSectionLabel(const MainConfig& config) {
  static unsigned int crossSectionLabelCount = 0;
  if (config.labelCrossSections) {
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



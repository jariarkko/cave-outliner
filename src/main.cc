
///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"
#include "outlinerdebug.hh"
#include "outlinerindexedmesh.hh"
#include "outlinermaterialmatrix.hh"
#include "outlinerprocessor.hh"
#include "outlinerdescribe.hh"
#include "outlinermath.hh"
#include "outlinersvg.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static const aiScene* processImport(Assimp::Importer& importer,
                                    const char* file);
static bool checkFileExtension(const char* filename,
                               const char* extension);
static void processHelp(void);
static void runTests(void);

///////////////////////////////////////////////////////////////////////////////////////////////
// Local variables ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static bool debug = 0;
static bool deepdebug = 0;
static bool deepdeepdebug = 0;
static float stepx = 1.0;
static float stepy = 1.0;
static aiVector3D boundingBoxStart = {-2,-2,-2};
static aiVector3D boundingBoxEnd = {2,2,2};
static enum outlinerdirection direction = dir_z;
static enum outlineralgorithm algorithm = alg_pixel;
static unsigned int tiles = outlinertiledivision;
static unsigned int holethreshold = 1;

///////////////////////////////////////////////////////////////////////////////////////////////
// Main program and option handling ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int
main(int argc, char** argv) {

  while (argc > 1 && argv[1][0] == '-') {
    if (strcmp(argv[1],"--debug") == 0) {
        debug = 1;
        debuginit(debug,deepdebug,deepdeepdebug);
    } else if (strcmp(argv[1],"--deepdebug") == 0) {
        debug = 1;
        deepdebug = 1;
        debuginit(debug,deepdebug,deepdeepdebug);
    } else if (strcmp(argv[1],"--deepdeepdebug") == 0) {
        debug = 1;
        deepdebug = 1;
        deepdeepdebug = 1;
        debuginit(debug,deepdebug,deepdeepdebug);
    } else if (strcmp(argv[1],"--z") == 0) {
        direction = dir_z;
    } else if (strcmp(argv[1],"--x") == 0) {
        direction = dir_x;
    } else if (strcmp(argv[1],"--y") == 0) {
        direction = dir_y;
    } else if (strcmp(argv[1],"--pixel") == 0) {
        algorithm = alg_pixel;
        debugf("algorithm now %u", algorithm);
     } else if (strcmp(argv[1],"--borderpixel") == 0) {
        algorithm = alg_borderpixel;
        debugf("algorithm now %u", algorithm);
     } else if (strcmp(argv[1],"--borderline") == 0) {
        algorithm = alg_borderline;
        debugf("algorithm now %u", algorithm);
     } else if (strcmp(argv[1],"--borderactual") == 0) {
        algorithm = alg_borderactual;
        debugf("algorithm now %u", algorithm);
    } else if (strcmp(argv[1],"--holethreshold") == 0 && argc > 2) {
      int num = atoi(argv[2]);
      if (num < 0 || num > 100) {
        errf("Hole threshold value needs to be non-negative and max 100, %s given", argv[2]);
        return(1);
      }
      holethreshold = num;
      argc--;argv++;
    } else if (strcmp(argv[1],"--step") == 0 && argc > 2) {
      stepx = stepy = atof(argv[2]);
      if (stepx < 0.0001) {
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
      boundingBoxStart = aiVector3D(startx,starty,startz);
      boundingBoxEnd = aiVector3D(endx,endy,endz);
    } else if (strcmp(argv[1],"--tiling") == 0 && argc > 2) {
      if (atoi(argv[2]) < 1 || atoi(argv[2]) > 10000) {
        errf("Invalid tile count, must be at least one and a not too big for memory, %s given", argv[2]);
        return(1);
      }
      tiles = atoi(argv[2]);
      argc--;argv++;
    } else if (strcmp(argv[1],"--test") == 0) {
      debuginit(debug,deepdebug,deepdeepdebug);
      runTests();
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
  if (outlineralgorithm_generatespicture(algorithm)) {
    if (!checkFileExtension(output,"svg")) {
      errf("Output file must be an SVG file, %s given", output);
      return(1);
    }
  }
  
  // Initialize debug
  debuginit(debug,deepdebug,deepdeepdebug);
  
  // Import the model
  Assimp::Importer importer;
  const aiScene* scene = processImport(importer,input);
  if (scene == 0) return(1);

  // Describe the model if needed
  if (deepdebug) {
    describeScene(scene,deepdebug,deepdeepdebug,deepdeepdebug,deepdeepdebug);
  }

  // Open the output
  float xOutputStart = DirectionOperations::outputx(direction,boundingBoxStart);
  float xOutputEnd = DirectionOperations::outputx(direction,boundingBoxEnd);
  float yOutputStart = DirectionOperations::outputy(direction,boundingBoxStart);
  float yOutputEnd = DirectionOperations::outputy(direction,boundingBoxEnd);
  float xSize = (xOutputEnd - xOutputStart) / stepx;
  float ySize = (yOutputEnd - yOutputStart) / stepy;
  unsigned int xSizeInt = xSize;
  unsigned int ySizeInt = ySize;
  float xFactor = 1 / stepx;
  float yFactor = 1 / stepy;
  debugf("SVG size will be %u x %u", xSize, ySize);
  SvgCreator svg(output,
                 xSizeInt,ySizeInt,
                 xOutputStart,yOutputStart,
                 xFactor,yFactor);
  
  // Check that we were able to open the file
  if (!svg.ok()) {
    errf("File open for writing  to %s failed", output);
    return(1);
  }
  
  // Build our own data structure
  aiVector2D bounding2DBoxStart(xOutputStart,yOutputStart);
  aiVector2D bounding2DBoxEnd(xOutputEnd,yOutputEnd);
  IndexedMesh indexed(outlinermaxmeshes,tiles,bounding2DBoxStart,bounding2DBoxEnd,direction);
  indexed.addScene(scene);
  
  // Process the model
  Processor processor(boundingBoxStart,
                      boundingBoxEnd,
                      stepx,
                      stepy,
                      direction,
                      algorithm,
                      holethreshold,
                      indexed);
  if (!processor.processScene(scene,svg)) {
    return(1);
  }

  // Check that file I/O was ok
  if (!svg.ok()) {
    errf("File output to %s failed", output);
    return(1);
  }
  
  // Done
  return(0);
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
  std::cout << "  --bounding x x y y z z   Set the bounding box area. Default is -2 2 -2 2 -2 2.\n";
  std::cout << "  --step i                 Set the granularity increment. Default is 1.\n";
  std::cout << "  --z                      Generate output as viewed from the z direction, i.e., showing x/y picture.\n";
  std::cout << "  --x                      Generate output as viewed from the x direction, i.e., showing z/y picture.\n";
  std::cout << "  --y                      Generate output as viewed from the y direction, i.e., showing x/z picture.\n";
  std::cout << "  --pixel                  Use the pixel output drawing algorithm (default, fills cave with pixels).\n";
  std::cout << "  --borderpixel            Use the border-only drawing algorithm, draws only the cave walls, with pixels.\n";
  std::cout << "  --borderline             Use the border-only drawing algorithm, draws only the cave walls, with lines.\n";
  std::cout << "  --borderactual           Use the border-only drawing algorithm, draws the cave walls using model triangle sides.\n";
  std::cout << "  --holethreshold n        Ignore holes in the model if they are n or less pixels.\n";
  std::cout << "  --tiling n               Optimize search process with n x n tiles. ";
  std::cout <<                            "Default is " << outlinertiledivision << ",\n";
  std::cout << "                           and --tiling 1 implies no optimization.\n";
  std::cout << "  --debug                  Turn on debugging messages (level 0, least)\n";
  std::cout << "  --deepdebug              Turn on debugging messages (level 1)\n";
  std::cout << "  --deepdeepdebug          Turn on debugging messages (level 2, most)\n";
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
// Test functions /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static void
runTests(void) {
  debugf("running tests");
  mathTests();
  MaterialMatrix::test();
  debugf("tests OK");
}




#include <stdarg.h>
#include <assert.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "debug.hh"
#include "indexedmesh.hh"
#include "processor.hh"
#include "describe.hh"
#include "outlinermath.hh"
#include "svg.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static const aiScene* processImport(Assimp::Importer& importer,
                                    const char* file);
static void processHelp(void);
static void runTests(void);

///////////////////////////////////////////////////////////////////////////////////////////////
// Local variables ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static bool debug = 0;
static bool deepdebug = 0;
static bool deepdeepdebug = 0;
static float step = 1.0;
static aiVector3D boundingboxstart = {-2,-2,-2};
static aiVector3D boundingboxend = {2,2,2};
enum algorithm {
  alg_pixel,
  alg_border
};
static enum algorithm alg = alg_pixel;

///////////////////////////////////////////////////////////////////////////////////////////////
// Main program and option handling ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

int
main(int argc, char** argv) {

  while (argc > 1 && argv[1][0] == '-') {
    if (strcmp(argv[1],"--debug") == 0) {
        debug = 1;
    } else if (strcmp(argv[1],"--deepdebug") == 0) {
        debug = 1;
        deepdebug = 1;
    } else if (strcmp(argv[1],"--deepdeepdebug") == 0) {
        debug = 1;
        deepdebug = 1;
        deepdeepdebug = 1;
    } else if (strcmp(argv[1],"--pixel") == 0) {
        alg = alg_pixel;
    } else if (strcmp(argv[1],"--border") == 0) {
        alg = alg_border;
    } else if (strcmp(argv[1],"--step") == 0 && argc > 2) {
      step = atof(argv[2]);
      if (step < 0.0001) {
        errf("Invalid step value");
        return(0);
      }
      argc--;argv++;
    } else if (strcmp(argv[1],"--bounding") == 0 && argc > 1 + 2*3) {
      argc--;argv++; float startx = atof(argv[1]);
      argc--;argv++; float endx = atof(argv[1]);
      argc--;argv++; float starty = atof(argv[1]);
      argc--;argv++; float endy = atof(argv[1]);
      argc--;argv++; float startz = atof(argv[1]);
      argc--;argv++; float endz = atof(argv[1]);
      if (endx - startx < 0.0001) {
        errf("Invalid bounding box x range");
        return(0);
      }
      if (endy - starty < 0.0001) {
        errf("Invalid bounding box y range");
        return(0);
      }
      if (endz - startz < 0.0001) {
        errf("Invalid bounding box z range");
        return(0);
      }
      boundingboxstart = aiVector3D(startx,starty,startz);
      boundingboxend = aiVector3D(endx,endy,endz);
    } else if (strcmp(argv[1],"--test") == 0) {
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
  
  if (argc != 3) {
    errf("Expected two arguments, an input and output file name");
    return(1);
  }
  const char* input = argv[1];
  const char* output = argv[2];
  
  // Initialize debug
  debuginit(debug,deepdebug,deepdeepdebug);
  
  // Import the model
  Assimp::Importer importer;
  const aiScene* scene = processImport(importer,input);
  if (scene == 0) return(1);

  // Describe the model if needed
  if (deepdebug) {
    describeScene(scene);
  }

  // Open the output
  unsigned int xSize = (boundingboxend.x - boundingboxstart.x) / step;
  unsigned int ySize = (boundingboxend.y - boundingboxstart.y) / step;
  debugf("SVG size will be %u x %u", xSize, ySize);
  SvgCreator svg(output,xSize,ySize);
  
  // Check that we were able to open the file
  if (!svg.ok()) {
    errf("File open for writing  to %s failed", output);
    return(0);
  }
  
  // Process the model
  if (!processScene(scene,svg)) {
    return(1);
  }

  // Check that file I/O was ok
  if (!svg.ok()) {
    errf("File output to %s failed", output);
    return(0);
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
  std::cout << "  --pixel or --border      Choose the output drawing algorithm. Default is pixel.\n";
  std::cout << "  --debug                  Turn on debugging messages (level 0, least)\n";
  std::cout << "  --deepdebug              Turn on debugging messages (level 1)\n";
  std::cout << "  --deepdeepdebug          Turn on debugging messages (level 2, most)\n";
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
// Test functions /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static void
runTests(void) {
  debugf("running tests");
  mathTests();
  debugf("tests OK");
}



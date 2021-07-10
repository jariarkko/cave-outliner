
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"

static const aiScene* processImport(Assimp::Importer& importer,
                                    const char* file);
static bool processScene(const aiScene* scene);
static void processHelp(void);

static bool debug = 0;

int
main(int argc, char** argv) {

  while (argc > 1 && argv[1][0] == '-') {
    if (strcmp(argv[1],"--debug") == 0) {
        debug = 1;
    } else if (strcmp(argv[1],"--help") == 0) {
      processHelp();
      return(0);
    } else {
      std::cerr << OUTLINER_ERRPREFIX "Unrecognised option: ";
      std::cerr << argv[1];
      std::cerr << " -- exit\n";
      return(1);
    }
    argc--;
    argv++;
  }
  
  if (argc != 3) {
    std::cerr << OUTLINER_ERRPREFIX "Expected two arguments, an input and output file name -- exit\n";
    return(1);
  }
  const char* input = argv[1];
  const char* output = argv[2];

  // Import the model
  Assimp::Importer importer;
  const aiScene* scene = processImport(importer,input);
  if (scene == 0) return(1);

  // Process the model
  if (!processScene(scene)) {
    return(1);
  }
  
  // Done
  return(0);
}

static const aiScene*
processImport(Assimp::Importer& importer,
              const char* fileName) {
  
  std::cerr << OUTLINER_DEBUGPREFIX "processImport\n";
  
  // Have the importer read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // probably to request more postprocessing than we do in this example.
  const aiScene* scene = importer.ReadFile(fileName,
                                           aiProcess_CalcTangentSpace       |
                                           aiProcess_Triangulate            |
                                           aiProcess_JoinIdenticalVertices  |
                                           aiProcess_SortByPType);
  
  // If the import failed, report it
  if (scene == 0) {
    std::cerr << OUTLINER_ERRPREFIX "Import failed: ";
    std::cerr << importer.GetErrorString();
    std::cerr << " -- exit\n";
    return(0);
  }

  return(scene);

}

static bool
processScene(const aiScene* scene) {
  std::cerr << OUTLINER_DEBUGPREFIX "processScene\n";
  return(1);
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
  std::cout << "  --debug     Turn on debugging messages\n";
  std::cout << "\n";
}

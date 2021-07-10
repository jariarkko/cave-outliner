
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"

static const aiScene* processImport(Assimp::Importer& importer,
                                    const char* file);
static bool processScene(const aiScene* scene);

int
main(int argc, char** argv) {
  if (argc != 2) {
    std::cerr << OUTLINER_ERRPREFIX "Expected an argument -- exit\n";
    return(1);
  }
  const char* input = argv[1];

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



#include <stdarg.h>
#include <assert.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static const aiScene* processImport(Assimp::Importer& importer,
                                    const char* file);
static bool processScene(const aiScene* scene);
static void describeScene(const aiScene* scene);
static void describeNode(const aiScene* scene,
                         const aiNode* node);
static void describeFace(const aiScene* scene,
                         const aiMesh* mesh,
                         const aiFace* face);
static void describeVertex(const aiScene* scene,
                           const aiMesh* mesh,
                           const aiVector3D* vertex);
static void describeMesh(const aiScene* scene,
                         unsigned int no,
                         const aiMesh* mesh);
static void describeTransformation(const aiMatrix4x4& x,
                                   char* buf,
                                   unsigned bufsiz);
static void describeVector3D(const aiVector3D& x,
                             char* buf,
                             unsigned bufsiz);
static void processHelp(void);
static void errf(const char* format, ...);
static void debugf(const char* format, ...);
static void deepdebugf(const char* format, ...);
static void deepdeepdebugf(const char* format, ...);

///////////////////////////////////////////////////////////////////////////////////////////////
// Local variables ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static bool debug = 0;
static bool deepdebug = 0;
static bool deepdeepdebug = 0;

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
// Model processing ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static bool
processScene(const aiScene* scene) {
  debugf("processScene");
  assert(scene != 0);
  if (deepdebug) {
    describeScene(scene);
  }
  return(1);
}

static void
describeScene(const aiScene* scene) {
  assert(scene != 0);
  deepdebugf("scene:");
  deepdebugf("  mFlags = 0x%x", scene->mFlags);
  deepdebugf("  mNumMeshes = %u", scene->mNumMeshes);
  deepdebugf("  mNumMaterials = %u", scene->mNumMaterials);
  deepdebugf("  mNumAnimations = %u", scene->mNumAnimations);
  deepdebugf("  mNumTextures = %u", scene->mNumTextures);
  deepdebugf("  mNumLights = %u", scene->mNumLights);
  deepdebugf("  mNumCameras = %u", scene->mNumCameras);
  describeNode(scene,scene->mRootNode);
  for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
    describeMesh(scene,m,scene->mMeshes[m]);
  }
}

static void
describeVector3D(const aiVector3D& x,
                 char* buf,
                 unsigned bufsiz) {
  memset(buf,0,bufsiz);
  snprintf(buf,bufsiz-1,"<%f,%f,%f>",
           x.x,
           x.y,
           x.z);
}

static void
describeTransformation(const aiMatrix4x4& x,
                       char* buf,
                       unsigned bufsiz) {
  memset(buf,0,bufsiz);
  if (x.IsIdentity()) {
    strncpy(buf,"identity",bufsiz-1);
  } else {
    aiVector3D scaling;
    aiVector3D rotationAxis;
    ai_real rotationAngle;
    aiVector3D position;
    x.Decompose(scaling,rotationAxis,rotationAngle,position);
    char scalingBuf[100];
    char rotationAxisBuf[100];
    char positionBuf[100];
    describeVector3D(scaling,scalingBuf,sizeof(scalingBuf));
    describeVector3D(rotationAxis,rotationAxisBuf,sizeof(rotationAxisBuf));
    describeVector3D(position,positionBuf,sizeof(positionBuf));
    snprintf(buf,bufsiz-1,"scale %s, rotation %s, angle %f, position %s",
             scalingBuf,rotationAxisBuf,rotationAngle,positionBuf);
  }
}

static void
describeNode(const aiScene* scene,
             const aiNode* node) {
  assert(scene != 0);
  assert(node != 0);
  deepdebugf("  node %s", node->mName.C_Str());
  deepdebugf("    mNumChildren = %u", node->mNumChildren);
  deepdebugf("    mNumMeshes = %u", node->mNumMeshes);
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    deepdebugf("      mesh %u", node->mMeshes[j]);
  }
  if (deepdebug) {
    char buf[200];
    describeTransformation(node->mTransformation,buf,sizeof(buf));
    deepdebugf("    mTransformation = %s", buf);
  }
  if (deepdeepdebug) {
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      describeNode(scene,node->mChildren[i]);
    }
  }
}

static void
describeMesh(const aiScene* scene,
             unsigned int no,
             const aiMesh* mesh) {
  assert(scene != 0);
  assert(mesh != 0);
  deepdebugf("  mesh %u", no);
  deepdebugf("    mPrimitiveTypes = 0x%x", mesh->mPrimitiveTypes);
  deepdebugf("    mNumVertices = %u", mesh->mNumVertices);
  deepdebugf("    mNumFaces = %u", mesh->mNumFaces);
  if (deepdeepdebug) {
    for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
      describeVertex(scene,mesh,&mesh->mVertices[v]);
    }
    for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
      describeFace(scene,mesh,&mesh->mFaces[f]);
    }
  }
}

static void
describeFace(const aiScene* scene,
             const aiMesh* mesh,
             const aiFace* face) {
  deepdebugf("      face mNumIndices = %u", face->mNumIndices);
  for (unsigned int i = 0; i < face->mNumIndices; i++) {
    deepdeepdebugf("        indice %u", face->mIndices[i]);
  }
}

static void
describeVertex(const aiScene* scene,
               const aiMesh* mesh,
               const aiVector3D* vertex) {
  char buf[100];
  describeVector3D(*vertex,
                   buf,
                   sizeof(buf));
  deepdebugf("      vertex %s", buf);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Output picture file IO /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
// Debug and output functions /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

__attribute__((__format__ (__printf__, 1, 0)))
void
debugf(const char* format, ...) {

  assert(format != 0);

  if (debug) {

    va_list args;
    char buf[500];
    memset(buf,0,sizeof(buf));
    va_start (args, format);
    vsnprintf(buf,sizeof(buf)-1,format,args);
    va_end (args);
    std::cerr << OUTLINER_DEBUGPREFIX;
    std::cerr << buf;
    std::cerr << "\n";
    
  }
  
}

__attribute__((__format__ (__printf__, 1, 0)))
void
deepdebugf(const char* format, ...) {

  assert(format != 0);

  if (deepdebug) {

    va_list args;
    char buf[500];
    memset(buf,0,sizeof(buf));
    va_start (args, format);
    vsnprintf(buf,sizeof(buf)-1,format,args);
    va_end (args);
    std::cerr << OUTLINER_DEBUGPREFIX;
    std::cerr << buf;
    std::cerr << "\n";
    
  }
  
}

__attribute__((__format__ (__printf__, 1, 0)))
void
deepdeepdebugf(const char* format, ...) {

  assert(format != 0);

  if (deepdeepdebug) {

    va_list args;
    char buf[500];
    memset(buf,0,sizeof(buf));
    va_start (args, format);
    vsnprintf(buf,sizeof(buf)-1,format,args);
    va_end (args);
    std::cerr << OUTLINER_DEBUGPREFIX;
    std::cerr << buf;
    std::cerr << "\n";
    
  }
  
}

__attribute__((__format__ (__printf__, 1, 0)))
void
errf(const char* format, ...) {

  assert(format != 0);

  va_list args;
  char buf[500];
  memset(buf,0,sizeof(buf));
  va_start (args, format);
  vsnprintf(buf,sizeof(buf)-1,format,args);
  va_end (args);
  std::cerr << OUTLINER_ERRPREFIX;
  std::cerr << buf;
  std::cerr << " -- exit\n";
  
}

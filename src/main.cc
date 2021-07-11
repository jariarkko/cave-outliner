
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
static bool sceneHasMaterial(const aiScene* scene,
                             float x,
                             float y);
static bool nodeHasMaterial(const aiScene* scene,
                            const aiNode* node,
                            float x,
                            float y);
static bool meshHasMaterial(const aiScene* scene,
                            const aiMesh* node,
                            float x,
                            float y);
static bool faceHasMaterial(const aiScene* scene,
                            const aiMesh* mesh,
                            const aiFace* face,
                            float x,
                            float y);
static bool pointInsideTriangle2D(const aiVector2D* a,
                                  const aiVector2D* b,
                                  const aiVector2D* c,
                                  const aiVector2D* point);
static void vectorTo(const aiVector2D* from,
                     const aiVector2D* to,
                     aiVector2D* result);
static float determinant2x2(const aiVector2D* u,
                            const aiVector2D* v);
static void processHelp(void);
static void runTests(void);
static void mathTests(void);
static void vectorTests(void);
static void detTests(void);
static void triangleTests(void);
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
static float step = 1.0;
static aiVector3D boundingboxstart = {-2,-2,-2};
static aiVector3D boundingboxend = {2,2,2};

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

  // Import the model
  Assimp::Importer importer;
  const aiScene* scene = processImport(importer,input);
  if (scene == 0) return(1);

  // Describe the model if needed
  if (deepdebug) {
    describeScene(scene);
  }
  
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
  std::cout << "  --bounding x x y y z z   Set the bounding box area. Default is -2 2 -2 2 -2 2.\n";
  std::cout << "  --step i                 Set the granularity increment. Default is 1.\n";
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
// Model processing ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static bool
processScene(const aiScene* scene) {
  debugf("processScene");
  assert(scene != 0);
  for (float x = boundingboxstart.x; x <= boundingboxend.x; x += step)  {
    for (float y = boundingboxstart.y; y <= boundingboxend.y; y += step)  {
      deepdebugf("checking (%.2f,%.2f)",x,y);
      if (sceneHasMaterial(scene,x,y)) {
        debugf("material at (%.2f,%.2f)",x,y);
      }
    }
  }
  return(1);
}

static bool
sceneHasMaterial(const aiScene* scene,
                 float x,
                 float y) {
  assert(scene != 0);
  deepdeepdebugf("checking for material at (%.2f,%.2f)", x, y);
  return(nodeHasMaterial(scene,scene->mRootNode,x,y));
}

static bool
nodeHasMaterial(const aiScene* scene,
                const aiNode* node,
                float x,
                float y) {
  assert(scene != 0);
  assert(node != 0);
  if (!node->mTransformation.IsIdentity()) {
    errf("Cannot handle transformations yet");
    exit(1);
  }
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    if (meshHasMaterial(scene,scene->mMeshes[node->mMeshes[j]],x,y)) {
      return(1);
    }
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    if (nodeHasMaterial(scene,node->mChildren[i],x,y)) {
      return(1);
    }
  }
  return(0);
}

static bool
meshHasMaterial(const aiScene* scene,
                const aiMesh* mesh,
                float x,
                float y) {
  assert(scene != 0);
  assert(mesh != 0);
  for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
    if (faceHasMaterial(scene,mesh,&mesh->mFaces[f],x,y)) {
      return(1);
    }
  }
  return(0);
}

static bool
faceHasMaterial(const aiScene* scene,
                const aiMesh* mesh,
                const aiFace* face,
                float x,
                float y) {
  assert(scene != 0);
  assert(mesh != 0);
  assert(face != 0);
  if (face->mNumIndices != 3) {
    errf("Cannot handle a face with %u indices", face->mNumIndices);
    exit(1);
  }
  if (face->mIndices[0] >= mesh->mNumVertices) {
    errf("Face points to a vertex %u that does not exist", face->mIndices[0]);
    exit(1);
  }
  if (face->mIndices[1] >= mesh->mNumVertices) {
    errf("Face points to a vertex %u that does not exist", face->mIndices[1]);
    exit(1);
  }
  if (face->mIndices[2] >= mesh->mNumVertices) {
    errf("Face points to a vertex %u that does not exist", face->mIndices[2]);
    exit(1);
  }
  aiVector3D* vertexA = &mesh->mVertices[face->mIndices[0]];
  aiVector3D* vertexB = &mesh->mVertices[face->mIndices[1]];
  aiVector3D* vertexC = &mesh->mVertices[face->mIndices[2]];
  aiVector2D a(vertexA->x,vertexA->y);
  aiVector2D b(vertexB->x,vertexB->y);
  aiVector2D c(vertexC->x,vertexC->y);
  aiVector2D point(x,y);
  if (pointInsideTriangle2D(&a,&b,&c,&point)) {
    debugf("found out that (%.2f,%.2f) is hitting a face",x,y);
    return(1);
  }
  return(0);
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
// Math functions /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static bool
pointInsideTriangle2D(const aiVector2D* triangleA,
                      const aiVector2D* triangleB,
                      const aiVector2D* triangleC,
                      const aiVector2D* point) {
  // Algorithm from https://mathworld.wolfram.com/TriangleInterior.html
  aiVector2D v = *point; 
  aiVector2D v0 = *triangleA;
  aiVector2D v1; vectorTo(triangleA,triangleB,&v1);
  aiVector2D v2; vectorTo(triangleA,triangleC,&v2);
  deepdebugf("triangle v = (%.2f,%.2f)", v.x, v.y);
  deepdebugf("triangle v0 = (%.2f,%.2f)", v0.x, v0.y);
  deepdebugf("triangle v1 = (%.2f,%.2f)", v1.x, v1.y);
  deepdebugf("triangle v2 = (%.2f,%.2f)", v2.x, v2.y);
  float a = (determinant2x2(&v,&v2) - determinant2x2(&v0,&v2)) / determinant2x2(&v1,&v2);
  float b = -((determinant2x2(&v,&v1) - determinant2x2(&v0,&v1)) / determinant2x2(&v1,&v2));
  deepdebugf("triangle check a %.2f b %.2f a+b %.2f", a, b, a+b);
  return(a >= 0 && b >= 0 && a+b <= 1);
}

static void
vectorTo(const aiVector2D* from,
         const aiVector2D* to,
         aiVector2D* result) {
  *result = *to;
  result->x -= from->x;
  result->y -= from->y;
}

static float
determinant2x2(const aiVector2D* u,
               const aiVector2D* v) {
  return(u->x * v->y - u->y * v->x);
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

static void
mathTests(void) {
  debugf("running math tests");
  vectorTests();
  detTests();
  triangleTests();
}

static void
vectorTests(void) {
  aiVector2D a(2,2);
  aiVector2D b(3,3);
  aiVector2D result;
  vectorTo(&a,&b,&result);
  deepdebugf("vector test: result: (%f,%f)", result.x, result.y);
  assert(result.x == 1);
  assert(result.y == 1);
}

static void
detTests(void) {
  aiVector2D C1(4,2);
  aiVector2D C2(1,3);
  ai_real result = determinant2x2(&C1,&C2);
  deepdebugf("determinant result = %.2f", result);
  assert(result == 10);
}

static void
triangleTests(void) {
  aiVector2D a(0,0);
  aiVector2D b(0,2);
  aiVector2D c(2,0);
  aiVector2D pointfar(2,2);
  aiVector2D pointnear(0.5,0.5);
  aiVector2D pointverynear(0.1,0.2);
  aiVector2D pointata = a;
  aiVector2D pointatb = b;
  aiVector2D pointatc = c;
  aiVector2D pointbeyondb = b;
  pointbeyondb.x += 0.01;
  pointbeyondb.y += 0.01;
  aiVector2D pointbefore1(-0.001,0);
  aiVector2D pointbefore2(0,-0.001);
  aiVector2D pointbefore3(-0.001,-0.001);
  bool ansfar = pointInsideTriangle2D(&a,&b,&c,&pointfar);
  deepdebugf("triangle test: pointfar = %u", ansfar);
  bool ansnear = pointInsideTriangle2D(&a,&b,&c,&pointnear);
  deepdebugf("triangle test: pointnear = %u", ansnear);
  bool ansverynear = pointInsideTriangle2D(&a,&b,&c,&pointverynear);
  deepdebugf("triangle test: pointverynear = %u", ansverynear);
  bool ansata = pointInsideTriangle2D(&a,&b,&c,&pointata);
  deepdebugf("triangle test: pointata = %u", ansata);
  bool ansatb = pointInsideTriangle2D(&a,&b,&c,&pointatb);
  deepdebugf("triangle test: pointatb = %u", ansatb);
  bool ansatc = pointInsideTriangle2D(&a,&b,&c,&pointatc);
  deepdebugf("triangle test: pointatc = %u", ansatc);
  bool ansbeyondb = pointInsideTriangle2D(&a,&b,&c,&pointbeyondb);
  deepdebugf("triangle test: pointbeyondb = %u", ansbeyondb);
  bool ansbefore1 = pointInsideTriangle2D(&a,&b,&c,&pointbefore1);
  deepdebugf("triangle test: pointbefore1 = %u", ansbefore1);
  bool ansbefore2 = pointInsideTriangle2D(&a,&b,&c,&pointbefore2);
  deepdebugf("triangle test: pointbefore2 = %u", ansbefore2);
  bool ansbefore3 = pointInsideTriangle2D(&a,&b,&c,&pointbefore3);
  deepdebugf("triangle test: pointbefore3 = %u", ansbefore3);
  assert(ansfar == 0);
  assert(ansnear == 1);
  assert(ansverynear == 1);
  assert(ansata == 1);
  assert(ansatb == 1);
  assert(ansatc == 1);
  assert(ansbeyondb == 0);
  assert(ansbefore1 == 0);
  assert(ansbefore2 == 0);
  assert(ansbefore3 == 0);
}

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

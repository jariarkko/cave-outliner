
///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinermath.hh"
#include "outlinerdebug.hh"
#include "outlinerprocessor.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static bool sceneHasMaterial(const aiScene* scene,
                  IndexedMesh& indexed,
                             float x,
                             float y);
static bool nodeHasMaterial(const aiScene* scene,
                            const aiNode* node,
                            IndexedMesh& indexed,
                            float x,
                            float y);
static bool meshHasMaterial(const aiScene* scene,
                            const aiMesh* node,
                            IndexedMesh& indexed,
                            float x,
                            float y);
static bool faceHasMaterial(const aiScene* scene,
                            const aiMesh* mesh,
                            const aiFace* face,
                            float x,
                            float y);

///////////////////////////////////////////////////////////////////////////////////////////////
// Model processing ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
processScene(const aiScene* scene,
             aiVector3D boundingboxstart,
             aiVector3D boundingboxend,
             float stepx,
             float stepy,
             enum outlineralgorithm alg,
             IndexedMesh& indexed,
             SvgCreator& svg) {
  debugf("processScene");
  assert(scene != 0);
  for (float x = boundingboxstart.x; x <= boundingboxend.x; x += stepx)  {
    for (float y = boundingboxstart.y; y <= boundingboxend.y; y += stepy)  {
      deepdebugf("checking (%.2f,%.2f)",x,y);
      if (sceneHasMaterial(scene,indexed,x,y)) {
        debugf("material at (%.2f,%.2f)",x,y);
        switch (alg) {
        case alg_pixel:
          svg.pixel(x,y);
          break;
        case alg_border:
          errf("Border algorithm is not yet implemented");
          exit(1);
        default:
          errf("Invalid algorithm %u", alg);
          exit(1);
        }
      }
    }
  }
  return(1);
}

static bool
sceneHasMaterial(const aiScene* scene,
                 IndexedMesh& indexed,
                 float x,
                 float y) {
  assert(scene != 0);
  deepdeepdebugf("checking for material at (%.2f,%.2f)", x, y);
  return(nodeHasMaterial(scene,scene->mRootNode,indexed,x,y));
}

static bool
nodeHasMaterial(const aiScene* scene,
                const aiNode* node,
                IndexedMesh& indexed,
                float x,
                float y) {
  assert(scene != 0);
  assert(node != 0);
  if (!node->mTransformation.IsIdentity()) {
    errf("Cannot handle transformations yet");
    exit(1);
  }
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    if (meshHasMaterial(scene,scene->mMeshes[node->mMeshes[j]],indexed,x,y)) {
      return(1);
    }
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    if (nodeHasMaterial(scene,node->mChildren[i],indexed,x,y)) {
      return(1);
    }
  }
  return(0);
}

static bool
meshHasMaterial(const aiScene* scene,
                const aiMesh* mesh,
                IndexedMesh& indexed,
                float x,
                float y) {
  assert(scene != 0);
  assert(mesh != 0);
  if (1) {
    unsigned int nFaces = 0;
    const aiFace** faces = 0;
    indexed.getFaces(&nFaces,&faces);
    debugf("meshHasMaterial normally %u faces but on this tile %u faces", mesh->mNumFaces,nFaces);
    for (unsigned int f = 0; f < nFaces; f++) {
      if (faceHasMaterial(scene,mesh,faces[f],x,y)) {
        return(1);
      }
    }
  } else {
    for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
      if (faceHasMaterial(scene,mesh,&mesh->mFaces[f],x,y)) {
        return(1);
      }
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



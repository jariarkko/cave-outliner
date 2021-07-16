
///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdlib.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinerdebug.hh"
#include "outlinerindexedmesh.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class methods //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

IndexedMesh::IndexedMesh(unsigned int maxMeshesIn,
                         unsigned int subdivisionsIn,
                         const aiVector2D& viewBoundingBoxStartIn,
                         const aiVector2D& viewBoundingBoxEndIn) {
  assert(maxMeshesIn >= 1);
  assert(subdivisionsIn >= 1);
  maxMeshes = maxMeshesIn;
  nMeshes = 0;
  subdivisions = subdivisionsIn;
  debugf("%u x %u tiles, or %u tiles", subdivisions, subdivisions, subdivisions * subdivisions);
  viewBoundingBoxStart = viewBoundingBoxStartIn;
  viewBoundingBoxEnd = viewBoundingBoxEndIn;
  float viewX = viewBoundingBoxEnd.x - viewBoundingBoxStart.x;
  float viewY = viewBoundingBoxEnd.y - viewBoundingBoxStart.y;
  tileSizeX = viewX / subdivisions;
  tileSizeY = viewY / subdivisions;
  debugf("view bounding box %f x %f", viewX, viewY);
  debugf("tile size %f x %f", tileSizeX, tileSizeY);
  meshes = new IndexedMeshOneMesh [maxMeshes];
  if (meshes == 0) {
    errf("Cannot allocate %u indexed meshes", maxMeshes);
    exit(1);
  }
  for (unsigned int i = 0; i < maxMeshes; i++) {
    meshes[i].mesh = 0;
    meshes[i].tileMatrix = new struct IndexedMeshOneMeshOneTileFaces* [subdivisions];
    if (meshes[i].tileMatrix == 0) {
      errf("Cannot allocate %u tile matrix", subdivisions);
      exit(1);
    }
    memset(meshes[i].tileMatrix,0,sizeof(struct IndexedMeshOneMeshOneTileFaces*) * subdivisions);
    for (unsigned int j = 0; j < subdivisions; j++) {
      meshes[i].tileMatrix[j] = new struct IndexedMeshOneMeshOneTileFaces [subdivisions];
      if (meshes[i].tileMatrix[j] == 0) {
        errf("Cannot allocate %u tile matrix second dimension", subdivisions);
        exit(1);
      }
      for (unsigned int k = 0; k < subdivisions;k++) {
        meshes[i].tileMatrix[j][k].nFaces = 0;
        meshes[i].tileMatrix[j][k].maxNFaces = 0;
        meshes[i].tileMatrix[j][k].faces = 0;
      }
    }
  }
}

void
IndexedMesh::addScene(const aiScene* scene) {
  addNode(scene,scene->mRootNode);
}

void
IndexedMesh::addNode(const aiScene* scene,
                     const aiNode* node) {
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    addMesh(scene,scene->mMeshes[node->mMeshes[j]]);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    addNode(scene,node->mChildren[i]);
  }
}

void
IndexedMesh::addMesh(const aiScene* scene,
                     const aiMesh* mesh) {
  if (nMeshes == maxMeshes) {
    errf("Cannot another mesh, already have %u", maxMeshes);
    exit(1);
  }
  meshes[nMeshes].mesh = mesh;
  nMeshes++;
  addFaces(meshes[nMeshes],scene,mesh);
}

void
IndexedMesh::addFaces(struct IndexedMeshOneMesh& shadow,
                      const aiScene* scene,
                      const aiMesh* mesh) {
  for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
    addFace(shadow,scene,mesh->mFaces[f]);
  }
}

void
IndexedMesh::addFace(struct IndexedMeshOneMesh& shadow,
                     const aiScene* scene,
                     const aiMesh* mesh,
                     const aiFace* face) {

  // Sanity checks
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

  // Calculate bounding box
  aiVector3D* vertexA = &mesh->mVertices[face->mIndices[0]];
  aiVector3D* vertexB = &mesh->mVertices[face->mIndices[1]];
  aiVector3D* vertexC = &mesh->mVertices[face->mIndices[2]];
  aiVector2D a(vertexA->x,vertexA->y);
  aiVector2D b(vertexB->x,vertexB->y);
  aiVector2D c(vertexC->x,vertexC->y);
  aiVector2D boundingBoxStart;
  aiVector2D boundingBoxEnd;
  triangleBoundingBox2D(&a,&b,&c,
                        &boundingBoxStart,
                        &boundingBoxEnd);

  // Calculate which tiles this belongs to (for big faces, may be more
  // than one)
  for (float x = outlinermax(viewBoundingBoxStart.x,boundingBoxStart.x);
       x <= boundingBoxEnd.x && x <= viewBoundingBoxEnd.x;
       x += tileSizeX) {
    for (float y = outlinermax(viewBoundingBoxStart.y,boundingBoxStart.y);
         y <= boundingBoxEnd.y && y <= viewBoundingBoxEnd.y;
         y += tileSizeY) {
      float xInView = x - viewBoundingBoxStart.x;
      float yInView = y - viewBoundingBoxStart.y;
      unsigned int tileX = xInView / tileSizeX;
      unsigned int tileY = yInView / tileSizeY;
      addToTile(shadow,scene,mesh,face,tileX,tileY);
    }
  }
}

void
IndexedMesh::addToTile(struct IndexedMeshOneMesh& shadow,
                       const aiScene* scene,
                       const aiMesh* mesh,
                       const aiFace* face,
                       unsigned int tileX,
                       unsigned int tileY) {
}

void
IndexedMesh::getFaces(unsigned int* p_nFaces,
                      const aiFace*** p_faces) {
}

void
IndexedMesh::getFacesTile(unsigned int xTile,
                          unsigned int yTile,
                          unsigned int* p_nFaces,
                          const aiFace*** p_faces) {
}

IndexedMesh::~IndexedMesh() {
  if (meshes != 0) {
    for (unsigned int i = 0; i < maxMeshes; i++) {
      if (meshes[i].tileMatrix == 0) continue;
      for (unsigned int j = 0; j < subdivisions; j++) {
        if (meshes[i].tileMatrix[j] == 0) continue;
        for (unsigned int k = 0; k < subdivisions;k++) {
          if (meshes[i].tileMatrix[j][k].faces != 0) {
            delete meshes[i].tileMatrix[j][k].faces;
          }
        }
        delete meshes[i].tileMatrix[j];
      }
      delete meshes[i].tileMatrix;
    }
    
    delete meshes;
    meshes = 0;
  }
  
  maxMeshes = 0;
  subdivisions = 0;
}


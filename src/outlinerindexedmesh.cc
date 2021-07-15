
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
                         unsigned int subdivisionsIn) {
  assert(maxMeshesIn >= 1);
  assert(subdivisionsIn >= 1);
  maxMeshes = maxMeshesIn;
  nMeshes = 0;
  subdivisions = subdivisionsIn;
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
}

void
IndexedMesh::addFace(struct IndexedMeshOneMesh& shadow,
                     const aiScene* scene,
                     const aiFace* face) {
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



#include <assert.h>
#include <stdlib.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "debug.hh"
#include "indexedmesh.hh"

IndexedMesh::IndexedMesh(unsigned int maxMeshesIn,
              unsigned int subdivisionsIn) {
  assert(maxMeshesIn >= 1);
  assert(subdivisionsIn >= 1);
  maxMeshes = maxMeshesIn;
  subdivisions = subdivisionsIn;
  meshes = new IndexedMeshOneMesh [maxMeshes];
  if (meshes == 0) {
    errf("Cannot allocate %u indexed meshes", maxMeshes);
    exit(1);
  }
  for (unsigned int i = 0; i < maxMeshes; i++) {
    meshes[i].mesh = 0;
  }
}

IndexedMesh::~IndexedMesh() {
  delete meshes;
  meshes = 0;
  maxMeshes = 0;
  subdivisions = 0;
}


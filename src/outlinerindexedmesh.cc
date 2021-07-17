
///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdlib.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinerdebug.hh"
#include "outlinermath.hh"
#include "outlinerindexedmesh.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class methods //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

IndexedMesh::IndexedMesh(unsigned int maxMeshesIn,
                         unsigned int subdivisionsIn,
                         const aiVector2D& viewBoundingBoxStartIn,
                         const aiVector2D& viewBoundingBoxEndIn,
                         enum outlinerdirection directionIn) {
  assert(maxMeshesIn >= 1);
  assert(subdivisionsIn >= 1);
  maxMeshes = maxMeshesIn;
  nMeshes = 0;
  subdivisions = subdivisionsIn;
  debugf("%u x %u tiles, or %u tiles", subdivisions, subdivisions, subdivisions * subdivisions);
  viewBoundingBoxStart = viewBoundingBoxStartIn;
  viewBoundingBoxEnd = viewBoundingBoxEndIn;
  direction = directionIn;
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
  assert(scene != 0);
  assert(mesh != 0);
  if (nMeshes == maxMeshes) {
    errf("Cannot another mesh, already have %u", maxMeshes);
    exit(1);
  }
  struct IndexedMeshOneMesh& newMesh = meshes[nMeshes];
  newMesh.mesh = mesh;
  assert(newMesh.mesh == mesh);
  nMeshes++;
  addFaces(newMesh,scene,mesh);
}

void
IndexedMesh::addFaces(struct IndexedMeshOneMesh& shadow,
                      const aiScene* scene,
                      const aiMesh* mesh) {
  assert(scene != 0);
  assert(shadow.mesh == mesh);
  assert(mesh != 0);
  for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
    addFace(shadow,scene,mesh,&mesh->mFaces[f]);
  }
}

void
IndexedMesh::addFace(struct IndexedMeshOneMesh& shadow,
                     const aiScene* scene,
                     const aiMesh* mesh,
                     const aiFace* face) {

  // Sanity checks
  assert(shadow.mesh != 0);
  assert(shadow.mesh == mesh);
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
      unsigned int tileX;
      unsigned int tileY;
      coordsToTile(x,y,tileX,tileY);
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

  // Sanity checks
  assert(shadow.mesh == mesh);
  assert(scene != 0);
  assert(mesh != 0);
  assert(face != 0);
  assert(tileX < subdivisions);
  assert(tileY < subdivisions);
  assert(shadow.tileMatrix != 0);
  deepdebugf("adding face to tile (%u/%u,%u/%u)",
             tileX, subdivisions, tileY, subdivisions);

  // Find the right row (x) in a matrix of tiles
  struct IndexedMeshOneMeshOneTileFaces* row = shadow.tileMatrix[tileX];
  assert(row != 0);

  // Find the right cell (y) in that row
  struct IndexedMeshOneMeshOneTileFaces* tile = &row[tileY];
  assert(tile->maxNFaces > 0 ||  tile->faces == 0);

  // Did we allocate a face table for this cell yet? If not, do it
  if (tile->maxNFaces == 0) {
    tile->maxNFaces = 200;
    tile->nFaces = 0;
    tile->faces = new const aiFace* [tile->maxNFaces];
    if (tile->faces == 0) {
      errf("Cannot allocate face table in tile (%u,%u)", tileX, tileY);
      exit(1);
    }
    memset(tile->faces,0,sizeof(const aiFace*) * tile->maxNFaces);
  }

  // Check to see if face table is big enough. If not, expand.
  if (tile->nFaces == tile->maxNFaces) {
    unsigned int oldMaxNFaces = tile->maxNFaces;
    const aiFace** oldFaces = tile->faces;
    unsigned int newMaxNFaces = tile->maxNFaces * 2 + 3;
    const aiFace** newFaces = new const aiFace* [newMaxNFaces];
    if (newFaces == 0) {
      errf("Cannot expand face table in tile (%u,%u) from %u to %u",
           tileX, tileY, oldMaxNFaces, newMaxNFaces);
      exit(1);
    }
    debugf("Expanded face table in tile (%u,%u) from %u to %u",
           tileX, tileY, oldMaxNFaces, newMaxNFaces);
    memset(newFaces,0,sizeof(const aiFace*) * newMaxNFaces);
    memcpy(newFaces,oldFaces,sizeof(const aiFace*) * tile->nFaces);
    tile->faces = newFaces;
    tile->maxNFaces = newMaxNFaces;
    delete oldFaces;
  }
  
  // Add the face to the table
  assert(tile->nFaces < tile->maxNFaces);
  tile->faces[tile->nFaces++] = face;
}

void
IndexedMesh::getShadow(const aiMesh* mesh,
                       struct IndexedMeshOneMesh** shadow) {
  for (unsigned int i = 0; i < nMeshes; i++) {
    if (meshes[i].mesh == mesh) {
      *shadow = &meshes[i];
      return;
    }
  }
  *shadow = 0;
}

void
IndexedMesh::getFaces(const aiMesh* mesh,
                      float x,
                      float y,
                      unsigned int* p_nFaces,
                      const aiFace*** p_faces) {
  unsigned int tileX;
  unsigned int tileY;
  coordsToTile(x,y,tileX,tileY);
  struct IndexedMeshOneMesh* shadow = 0;
  getShadow(mesh,&shadow);
  assert(shadow != 0);
  getFacesTile(*shadow,
               mesh,
               tileX,
               tileY,
               p_nFaces,
               p_faces);
}

void
IndexedMesh::getFacesTile(struct IndexedMeshOneMesh& shadow,
                          const aiMesh* mesh,
                          unsigned int tileX,
                          unsigned int tileY,
                          unsigned int* p_nFaces,
                          const aiFace*** p_faces) {
  // Sanity checks
  assert(shadow.mesh != 0);
  assert(shadow.mesh == mesh);
  assert(mesh != 0);
  if (tileX >= subdivisions || tileY >= subdivisions) {
    debugf("getFacesTile(%u,%u)", tileX, tileY);
  }
  assert(tileX < subdivisions);
  assert(tileY < subdivisions);
  assert(shadow.tileMatrix != 0);
  assert(p_nFaces != 0);
  assert(p_faces != 0);
  deepdebugf("faces faces in tile (%u/%u,%u/%u)",
             tileX, subdivisions, tileY, subdivisions);
  
  // Find the right row (x) in a matrix of tiles
  struct IndexedMeshOneMeshOneTileFaces* row = shadow.tileMatrix[tileX];
  assert(row != 0);
  
  // Find the right cell (y) in that row
  struct IndexedMeshOneMeshOneTileFaces* tile = &row[tileY];
  assert(tile->maxNFaces > 0 ||  tile->faces == 0);

  // Return the result
  if (tile->nFaces == 0) {
    *p_nFaces = 0;
    *p_faces = 0;
  } else {
    *p_nFaces = tile->nFaces;
    *p_faces = tile->faces;
  }

  deepdebugf("returning %u faces", *p_nFaces);
}

void
IndexedMesh::coordsToTile(float x,
                          float y,
                          unsigned int& tileX,
                          unsigned int& tileY) {
  if (x == viewBoundingBoxEnd.x) {
    tileX = subdivisions - 1;
  } else {
    float xInView = x - viewBoundingBoxStart.x;
    tileX = xInView / tileSizeX;
  }
  if (y == viewBoundingBoxEnd.y) {
    tileY = subdivisions - 1;
  } else {
    float yInView = y - viewBoundingBoxStart.y;
    tileY = yInView / tileSizeY;
  }
  deepdebugf("coordinate (%f,%f) tile is (%u,%u)",
             x, y,
             tileX, tileY);
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



#include <cassert>
#include <stdlib.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinerdebug.hh"
#include "outlinermath.hh"
#include "outlinerboundingboxer.hh"

BoundingBoxer::BoundingBoxer(const aiScene* scene) {
  infof("calculating bounding box...");
  boundingBoxSet = 0;
  boundingScene(scene);
  if (!boundingBoxSet) {
    errf("Cannot determine bounding box (empty model?)");
  }
  outlinerhighprecisionreal xSize = boundingBoxEnd.x - boundingBoxStart.x;
  outlinerhighprecisionreal ySize = boundingBoxEnd.y - boundingBoxStart.y;
  outlinerhighprecisionreal zSize = boundingBoxEnd.z - boundingBoxStart.z;
  outlinerhighprecisionreal xIncrease = xSize * 0.05;
  outlinerhighprecisionreal yIncrease = ySize * 0.05;
  outlinerhighprecisionreal zIncrease = zSize * 0.05;
  boundingBoxStart.x -= xIncrease;
  boundingBoxEnd.x += xIncrease;
  boundingBoxStart.y -= yIncrease;
  boundingBoxEnd.y += yIncrease;
  boundingBoxStart.z -= zIncrease;
  boundingBoxEnd.z += zIncrease;
  infof("  discovered bounding box (%.2f,%.2f,%.2f) to (%.2f,%.2f,%.2f)",
        boundingBoxStart.x, boundingBoxStart.y, boundingBoxStart.z,
        boundingBoxEnd.x, boundingBoxEnd.y, boundingBoxEnd.z);
}

void
BoundingBoxer::getBoundingBox(HighPrecisionVector3D& boundingBoxStartOut,
                              HighPrecisionVector3D& boundingBoxEndOut) {
  assert(boundingBoxSet);
  boundingBoxStartOut = boundingBoxStart;
  boundingBoxEndOut = boundingBoxEnd;
}

BoundingBoxer::~BoundingBoxer() {
}

void
BoundingBoxer::boundingScene(const aiScene* scene) {
  boundingNode(scene,scene->mRootNode);
}

void
BoundingBoxer::boundingNode(const aiScene* scene,
                            const aiNode* node) {
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    boundingMesh(scene->mMeshes[node->mMeshes[j]]);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    boundingNode(scene,node->mChildren[i]);
  }
}

void
BoundingBoxer::boundingMesh(const aiMesh* mesh) {
  assert(mesh != 0);
  boundingFaces(mesh);
}

void
BoundingBoxer::boundingFaces(const aiMesh* mesh) {
  assert(mesh != 0);
  for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
    boundingFace(mesh,&mesh->mFaces[f]);
  }
}

void
BoundingBoxer::boundingFace(const aiMesh* mesh,
                            const aiFace* face) {

  // Sanity checks
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
  const aiVector3D& vertexA = mesh->mVertices[face->mIndices[0]];
  const aiVector3D& vertexB = mesh->mVertices[face->mIndices[1]];
  const aiVector3D& vertexC = mesh->mVertices[face->mIndices[2]];
  HighPrecisionVector3D elementBoundingBoxStart;
  HighPrecisionVector3D elementBoundingBoxEnd;
  OutlinerMath::triangleBoundingBox3D(vertexA,vertexB,vertexC,
                                      elementBoundingBoxStart,
                                      elementBoundingBoxEnd);

  // See if this is the first bounding box we see
  if (!boundingBoxSet) {
    boundingBoxStart = elementBoundingBoxStart;
    boundingBoxEnd = elementBoundingBoxEnd;
    boundingBoxSet = 1;
    return;
  } 

  // See if the new element should extend the existing bounding somehow
  if (elementBoundingBoxStart.x < boundingBoxStart.x) {
    boundingBoxStart.x = elementBoundingBoxStart.x;
  }
  if (elementBoundingBoxEnd.x > boundingBoxEnd.x) {
    boundingBoxEnd.x = elementBoundingBoxEnd.x;
  }
  if (elementBoundingBoxStart.y < boundingBoxStart.y) {
    boundingBoxStart.y = elementBoundingBoxStart.y;
  }
  if (elementBoundingBoxEnd.y > boundingBoxEnd.y) {
    boundingBoxEnd.y = elementBoundingBoxEnd.y;
  }
  if (elementBoundingBoxStart.z < boundingBoxStart.z) {
    boundingBoxStart.z = elementBoundingBoxStart.z;
  }
  if (elementBoundingBoxEnd.z > boundingBoxEnd.z) {
    boundingBoxEnd.z = elementBoundingBoxEnd.z;
  }
}



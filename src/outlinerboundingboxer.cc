///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//
//      CCC    AAA   V     V EEEEE       OOO   UU   UU TTTTTT LL    II NN   NN EEEEE RRRRR
//    CC   CC AA  AA  V   V  EE        OO   OO UU   UU   TT   LL    II NNN  NN EE    RR  RR
//    CC      AA  AA  V   V  EEE       OO   OO UU   UU   TT   LL    II NN N NN EEE   RRRRR
//    CC   CC AAAAAA   V V   EE        OO   OO UU   UU   TT   LL    II NN  NNN EE    RR R
//      CCc   AA  AA    V    EEEEE       OOO    UUUUU    TT   LLLLL II NN   NN EEEEE RR  R
//
//    CAVE OUTLINER -- Cave 3D model processing software
//
//    Copyright (C) 2021 by Jari Arkko -- See LICENSE.txt for license information.
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <stdlib.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinerdebug.hh"
#include "outlinermath.hh"
#include "outlinerboundingboxer.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Functions //////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

BoundingBoxer::BoundingBoxer(const aiScene* scene,
                             outlinerreal xIncrease,
                             outlinerreal yIncrease,
                             outlinerreal zIncrease) {
  infof("Calculating bounding box...");
  boundingBoxSet = 0;
  boundingScene(scene);
  if (!boundingBoxSet) {
    errf("Cannot determine bounding box (empty model?)");
  }
  increasedBoundingBox = boundingBox;
  increasedBoundingBox.start.x -= xIncrease;
  increasedBoundingBox.end.x += xIncrease;
  increasedBoundingBox.start.y -= yIncrease;
  increasedBoundingBox.end.y += yIncrease;
  increasedBoundingBox.start.z -= zIncrease;
  increasedBoundingBox.end.z += zIncrease;
  infof("  Discovered bounding box (%.2f,%.2f,%.2f) to (%.2f,%.2f,%.2f)",
        boundingBox.start.x, boundingBox.start.y, boundingBox.start.z,
        boundingBox.end.x, boundingBox.end.y, boundingBox.end.z);
}

void
BoundingBoxer::getOriginalBoundingBox(OutlinerBox3D& boundingBoxOut) {
  assert(boundingBoxSet);
  boundingBoxOut = boundingBox;
}

void
BoundingBoxer::getIncreasedBoundingBox(OutlinerBox3D& boundingBoxOut) {
  assert(boundingBoxSet);
  boundingBoxOut = increasedBoundingBox;
}

BoundingBoxer::~BoundingBoxer() {
  debugf("BoundingBoxer::~BoundingBoxer");
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
    return;
  }
  if (face->mIndices[0] >= mesh->mNumVertices) {
    errf("Face points to a vertex %u that does not exist", face->mIndices[0]);
    return;
  }
  if (face->mIndices[1] >= mesh->mNumVertices) {
    errf("Face points to a vertex %u that does not exist", face->mIndices[1]);
    return;
  }
  if (face->mIndices[2] >= mesh->mNumVertices) {
    errf("Face points to a vertex %u that does not exist", face->mIndices[2]);
    return;
  }

  // Calculate bounding box
  const aiVector3D& vertexA = mesh->mVertices[face->mIndices[0]];
  const aiVector3D& vertexB = mesh->mVertices[face->mIndices[1]];
  const aiVector3D& vertexC = mesh->mVertices[face->mIndices[2]];
  OutlinerBox3D elementBoundingBox;
  OutlinerTriangle3D triangle3(vertexA,vertexB,vertexC);
  OutlinerMath::triangleBoundingBox3D(triangle3,elementBoundingBox);
  
  // See if this is the first bounding box we see
  if (!boundingBoxSet) {
    boundingBox = elementBoundingBox;
    boundingBoxSet = 1;
    return;
  } 

  // See if the new element should extend the existing bounding somehow
  if (elementBoundingBox.start.x < boundingBox.start.x) {
    boundingBox.start.x = elementBoundingBox.start.x;
  }
  if (elementBoundingBox.end.x > boundingBox.end.x) {
    boundingBox.end.x = elementBoundingBox.end.x;
  }
  if (elementBoundingBox.start.y < boundingBox.start.y) {
    boundingBox.start.y = elementBoundingBox.start.y;
  }
  if (elementBoundingBox.end.y > boundingBox.end.y) {
    boundingBox.end.y = elementBoundingBox.end.y;
  }
  if (elementBoundingBox.start.z < boundingBox.start.z) {
    boundingBox.start.z = elementBoundingBox.start.z;
  }
  if (elementBoundingBox.end.z > boundingBox.end.z) {
    boundingBox.end.z = elementBoundingBox.end.z;
  }
}



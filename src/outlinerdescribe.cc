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

#include <stdlib.h>
#include <cassert>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdescribe.hh"
#include "outlinerdebug.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Functions //////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
describeScene(const aiScene* scene,
              bool transforms,
              bool recurse,
              bool vertexes,
              bool faces) {
  assert(scene != 0);
  deepdebugf("scene:");
  deepdebugf("  mFlags = 0x%x", scene->mFlags);
  deepdebugf("  mNumMeshes = %u", scene->mNumMeshes);
  deepdebugf("  mNumMaterials = %u", scene->mNumMaterials);
  deepdebugf("  mNumAnimations = %u", scene->mNumAnimations);
  deepdebugf("  mNumTextures = %u", scene->mNumTextures);
  deepdebugf("  mNumLights = %u", scene->mNumLights);
  deepdebugf("  mNumCameras = %u", scene->mNumCameras);
  describeNode(scene,scene->mRootNode,transforms,recurse);
  for (unsigned int m = 0; m < scene->mNumMeshes; m++) {
    describeMesh(scene,m,scene->mMeshes[m],vertexes,faces);
  }
}

void
describeVector3D(const aiVector3D& x,
                 char* buf,
                 unsigned bufsiz) {
  memset(buf,0,bufsiz);
  snprintf(buf,bufsiz-1,"<%f,%f,%f>",
           x.x,
           x.y,
           x.z);
}

void
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

void
describeNode(const aiScene* scene,
             const aiNode* node,
             bool transforms,
             bool recurse) {
  assert(scene != 0);
  assert(node != 0);
  deepdebugf("  node %s", node->mName.C_Str());
  deepdebugf("    mNumChildren = %u", node->mNumChildren);
  deepdebugf("    mNumMeshes = %u", node->mNumMeshes);
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    deepdebugf("      mesh %u", node->mMeshes[j]);
  }
  if (transforms) {
    char buf[200];
    describeTransformation(node->mTransformation,buf,sizeof(buf));
    deepdebugf("    mTransformation = %s", buf);
  }
  if (recurse) {
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      describeNode(scene,node->mChildren[i],transforms,recurse);
    }
  }
}

void
describeMesh(const aiScene* scene,
             unsigned int no,
             const aiMesh* mesh,
             bool vertexes,
             bool faces) {
  assert(scene != 0);
  assert(mesh != 0);
  deepdebugf("  mesh %u", no);
  deepdebugf("    mPrimitiveTypes = 0x%x", mesh->mPrimitiveTypes);
  deepdebugf("    mNumVertices = %u", mesh->mNumVertices);
  deepdebugf("    mNumFaces = %u", mesh->mNumFaces);
  if (vertexes) {
    for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
      describeVertex(scene,mesh,&mesh->mVertices[v]);
    }
  }
  if (faces) {
    for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
      describeFace(scene,mesh,&mesh->mFaces[f]);
    }
  }
}

void
describeFace(const aiScene* scene,
             const aiMesh* mesh,
             const aiFace* face) {
  deepdebugf("      face mNumIndices = %u", face->mNumIndices);
  for (unsigned int i = 0; i < face->mNumIndices; i++) {
    unsigned int ind = face->mIndices[i];
    char buf[100];
    describeVector3D(mesh->mVertices[ind],
                     buf,
                     sizeof(buf));
    deepdeepdebugf("        indice %u %s",
                   ind,
                   buf);
  }
}

void
describeVertex(const aiScene* scene,
               const aiMesh* mesh,
               const aiVector3D* vertex) {
  char buf[100];
  describeVector3D(*vertex,
                   buf,
                   sizeof(buf));
  deepdebugf("      vertex %s", buf);
}


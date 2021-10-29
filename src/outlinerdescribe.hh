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

#ifndef DESCRIBE_HH
#define DESCRIBE_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Functions //////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void describeScene(const aiScene* scene,
                   bool transforms,
                   bool recurse,
                   bool vertexes,
                   bool faces);
void describeNode(const aiScene* scene,
                  const aiNode* node,
                  bool transforms,
                  bool recurse);
void describeFace(const aiScene* scene,
                  const aiMesh* mesh,
                  const aiFace* face);
void describeVertex(const aiScene* scene,
                    const aiMesh* mesh,
                    const aiVector3D* vertex);
void describeMesh(const aiScene* scene,
                  unsigned int no,
                  const aiMesh* mesh,
                   bool vertexes,
                  bool faces);
void describeTransformation(const aiMatrix4x4& x,
                            char* buf,
                            unsigned bufsiz);
void describeVector3D(const aiVector3D& x,
                      char* buf,
                      unsigned bufsiz);
 
#endif // DESCRIBE_HH


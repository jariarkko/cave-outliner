
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
// Model processing ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

Processor::Processor() {
  xIndexSize = 0;
  yIndexSize = 0;
  bitMatrix = 0;
}

Processor::~Processor() {
  if (bitMatrix != 0) {
    delete bitMatrix;
  }
}

bool
Processor::processScene(const aiScene* scene,
                        aiVector3D boundingboxstart,
                        aiVector3D boundingboxend,
                        float stepx,
                        float stepy,
                        enum outlinerdirection direction,
                        enum outlineralgorithm algorithm,
                        IndexedMesh& indexed,
                        SvgCreator& svg) {
  
  debugf("processScene");
  assert(scene != 0);
  setUpMaterialMatrix(boundingboxstart,boundingboxend,stepx,stepy);

  // First, go through each part of the picture, and determine if
  // there's material in it. Construct a matrix representing the
  // results.
  unsigned int xIndex = 0;
  for (float x = boundingboxstart.x; x <= boundingboxend.x; x += stepx) {
    unsigned int yIndex = 0;
    assert(xIndex < xIndexSize);
    for (float y = boundingboxstart.y; y <= boundingboxend.y; y += stepy) {
      assert(yIndex < yIndexSize);
      deepdebugf("checking (%.2f,%.2f)",x,y);
      if (sceneHasMaterial(scene,indexed,x,y)) {
        debugf("material at (%.2f,%.2f) ie. %u,%u",x,y,xIndex,yIndex);
        setMaterialMatrix(xIndex,yIndex);
      }
      yIndex++;
    }
    xIndex++;
  }
  
  // Now there's a matrix filled with a flag for each coordinate,
  // whether there was material or not. Draw the output based on
  // that.
  for (xIndex = 0; xIndex < xIndexSize; xIndex++) {
    for (unsigned int yIndex = 0; yIndex < yIndexSize; yIndex++) {
      if (getMaterialMatrix(xIndex,yIndex)) {
        float x = boundingboxstart.x + xIndex * stepx;
        float y = boundingboxstart.y + yIndex * stepy;
        switch (algorithm) {
        case alg_pixel:
          svg.pixel(x,y);
          break;
        case alg_borderpixel:
          errf("Borderpixel algorithm is not yet implemented");
          exit(1);
        case alg_borderline:
          errf("Borderline algorithm is not yet implemented");
          exit(1);
        case alg_borderactual:
          errf("Borderactual algorithm is not yet implemented");
          exit(1);
        default:
          errf("Invalid algorithm %u", algorithm);
          exit(1);
        }
      }
    }
  }
  
  // Done, all good
  return(1);
}

bool
Processor::sceneHasMaterial(const aiScene* scene,
                            IndexedMesh& indexed,
                            float x,
                            float y) {
  assert(scene != 0);
  deepdeepdebugf("checking for material at (%.2f,%.2f)", x, y);
  return(nodeHasMaterial(scene,scene->mRootNode,indexed,x,y));
}

bool
Processor::nodeHasMaterial(const aiScene* scene,
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

bool
Processor::meshHasMaterial(const aiScene* scene,
                           const aiMesh* mesh,
                           IndexedMesh& indexed,
                           float x,
                           float y) {
  assert(scene != 0);
  assert(mesh != 0);
  if (1) {
    unsigned int nFaces = 0;
    const aiFace** faces = 0;
    indexed.getFaces(mesh,x,y,&nFaces,&faces);
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

bool
Processor::faceHasMaterial(const aiScene* scene,
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

///////////////////////////////////////////////////////////////////////////////////////////////
// Material matrix maintenance ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
Processor::setUpMaterialMatrix(aiVector3D boundingboxstart,
                               aiVector3D boundingboxend,
                               float stepx,
                               float stepy) {
  xIndexSize = ((boundingboxend.x - boundingboxstart.x) / stepx) + 1;
  yIndexSize = ((boundingboxend.y - boundingboxstart.y) / stepy) + 1;
  unsigned int n = xIndexSize * yIndexSize;
  unsigned int nChars = (n / 8) + 1;
  bitMatrix = new unsigned char [nChars];
  memset(bitMatrix,0,nChars);
}

void
Processor::setMaterialMatrix(unsigned int xIndex,
                             unsigned int yIndex) {
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  unsigned int index = xIndex * xIndexSize + yIndex;
  unsigned int charpart = index / 8;
  unsigned int bitpart = index % 8;
  unsigned char bitMask = (1 << bitpart);
  debugf("setting material matrix %u (%u,%u) elem %u with mask %x", index, xIndex, yIndex, charpart, bitMask);
  bitMatrix[charpart] |= bitMask;
}

bool
Processor::getMaterialMatrix(unsigned int xIndex,
                             unsigned int yIndex) {
  assert(xIndex < xIndexSize);
  assert(yIndex < yIndexSize);
  assert(bitMatrix != 0);
  unsigned int index = xIndex * xIndexSize + yIndex;
  unsigned int charpart = index / 8;
  unsigned int bitpart = index % 8;
  unsigned char thechar = bitMatrix[charpart];
  unsigned char bitMask = (1 << bitpart);
  deepdebugf("getting material matrix %u (%u,%u) elem %u value %x with mask %x", index, xIndex, yIndex, charpart, thechar, bitMask);
  if ((thechar & bitMask) != 0) return(1);
  else return(0);
}

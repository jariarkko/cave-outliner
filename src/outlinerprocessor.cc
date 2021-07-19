  
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

Processor::Processor(aiVector3D boundingboxstartIn,
                     aiVector3D boundingboxendIn,
                     float stepxIn,
                     float stepyIn,
                     enum outlinerdirection directionIn,
                     enum outlineralgorithm algorithmIn,
                     unsigned int holethresholdIn,
                     IndexedMesh& indexedIn) : boundingboxstart(boundingboxstartIn),
                                               boundingboxend(boundingboxendIn),
                                               stepx(stepxIn),
                                               stepy(stepyIn),
                                               direction(directionIn),
                                               algorithm(algorithmIn),
                                               holethreshold(holethresholdIn),
                                               matrix(boundingboxstartIn,
                                                      boundingboxendIn,
                                                      directionIn,
                                                      stepxIn,
                                                      stepyIn),
                                               indexed(indexedIn) {
  debugf("algorithm %u=%u", algorithm, algorithmIn);
}

Processor::~Processor() {
}

bool
Processor::processScene(const aiScene* scene,
                        SvgCreator& svg) {
  
  debugf("processScene");
  assert(scene != 0);

  // First, go through each part of the picture, and determine if
  // there's material in it. Construct a matrix representing the
  // results.
  unsigned int xIndex = 0;
  for (float x = DirectionOperations::outputx(direction,boundingboxstart); x <= DirectionOperations::outputx(direction,boundingboxend); x += stepx) {
    assert(x >= DirectionOperations::outputx(direction,boundingboxstart) && x <= DirectionOperations::outputx(direction,boundingboxend));
    unsigned int yIndex = 0;
    if (xIndex >= matrix.xIndexSize) {
      debugf("processScene %u/%u", xIndex, matrix.xIndexSize);
    }
    assert(xIndex < matrix.xIndexSize);
    for (float y = DirectionOperations::outputy(direction,boundingboxstart);
         y <= DirectionOperations::outputy(direction,boundingboxend);
         y += stepy) {
      assert(x >= DirectionOperations::outputx(direction,boundingboxstart) &&
             x <= DirectionOperations::outputx(direction,boundingboxend));
      assert(y >= DirectionOperations::outputy(direction,boundingboxstart) &&
             y <= DirectionOperations::outputy(direction,boundingboxend));
      if (yIndex >= matrix.yIndexSize) {
        debugf("processScene %u,%u/%u,%u", xIndex, yIndex, matrix.xIndexSize, matrix.yIndexSize);
      }
      assert(yIndex < matrix.yIndexSize);
      deepdebugf("checking (%.2f,%.2f)",x,y);
      if (sceneHasMaterial(scene,indexed,x,y)) {
        debugf("material at (%.2f,%.2f) ie. %u,%u",x,y,xIndex,yIndex);
        matrix.setMaterialMatrix(xIndex,yIndex);
      }
      yIndex++;
    }
    xIndex++;
  }
  
  // Now there's a matrix filled with a flag for each coordinate,
  // whether there was material or not. Draw the output based on
  // that.
  for (xIndex = 0; xIndex < matrix.xIndexSize; xIndex++) {
    for (unsigned int yIndex = 0; yIndex < matrix.yIndexSize; yIndex++) {
      if (matrix.getMaterialMatrix(xIndex,yIndex)) {
        float x = DirectionOperations::outputx(direction,boundingboxstart) + xIndex * stepx;
        float y = DirectionOperations::outputy(direction,boundingboxstart) + yIndex * stepy;
        debugf("algorithm %u", algorithm);
        switch (algorithm) {
        case alg_pixel:
          debugf("pixel alg %u,%u", xIndex, yIndex);
          svg.pixel(x,y);
          break;
        case alg_borderpixel:
          debugf("borderpixel alg %u,%u", xIndex, yIndex);
          if (isBorder(xIndex,yIndex)) {
            svg.pixel(x,y);
          }
          break;
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
  aiVector2D a(DirectionOperations::outputx(direction,*vertexA),DirectionOperations::outputy(direction,*vertexA));
  aiVector2D b(DirectionOperations::outputx(direction,*vertexB),DirectionOperations::outputy(direction,*vertexB));
  aiVector2D c(DirectionOperations::outputx(direction,*vertexC),DirectionOperations::outputy(direction,*vertexC));
  aiVector2D point(x,y);
  if (pointInsideTriangle2D(&a,&b,&c,&point)) {
    debugf("found out that (%.2f,%.2f) is hitting a face",x,y);
    return(1);
  }
  return(0);
}

void
Processor::getNeighbours(unsigned int xIndex,
                         unsigned int yIndex,
                         unsigned int& n,
                         unsigned int tableSize,
                         unsigned int* tableX,
                         unsigned int* tableY) {
  assert(tableSize <= 8);
  n = 0;
  
  // Left side three neighbours
  if (xIndex > 0 && yIndex > 0)                                       { tableX[n] = xIndex-1; tableY[n] = yIndex-1; n++; }
  if (xIndex > 0)                                                     { tableX[n] = xIndex-1; tableY[n] = yIndex;   n++; }
  if (xIndex > 0 && yIndex < matrix.yIndexSize-1)                     { tableX[n] = xIndex-1; tableY[n] = yIndex+1; n++; }

  // Top and bottom neighbours
  if (yIndex > 0)                                                     { tableX[n] = xIndex;   tableY[n] = yIndex-1; n++; }
  if (yIndex < matrix.yIndexSize-1)                                   { tableX[n] = xIndex;   tableY[n] = yIndex+1; n++; }
  
  // Right side three neighbours
  if (xIndex < matrix.xIndexSize-1 && yIndex > 0)                     { tableX[n] = xIndex+1; tableY[n] = yIndex-1; n++; }
  if (xIndex < matrix.xIndexSize-1)                                   { tableX[n] = xIndex+1; tableY[n] = yIndex;   n++; }
  if (xIndex < matrix.xIndexSize-1 && yIndex < matrix.yIndexSize-1) { tableX[n] = xIndex+1; tableY[n] = yIndex+1; n++; }
  
  // Done
  assert(n <= tableSize);
}

bool
Processor::isBorder(unsigned int xIndex,
                    unsigned int yIndex) {
  unsigned int n;
  const unsigned int tableSize = 8;
  unsigned int tableX[tableSize];
  unsigned int tableY[tableSize];
  getNeighbours(xIndex,yIndex,n,tableSize,tableX,tableY);
  debugf("point %u,%u has %u neighbors", xIndex, yIndex, n);
  for (unsigned int i = 0; i < n; i++) {
    if (!matrix.getMaterialMatrix(tableX[i],tableY[i])) {
      debugf("point %u,%u is a border due to %u,%u", xIndex, yIndex, tableX[i], tableY[i]);
      return(1);
    }
  }
  debugf("point %u,%u is inside cave", xIndex, yIndex);
  return(0);
}

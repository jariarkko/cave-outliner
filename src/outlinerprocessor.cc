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
#include "outlinerhighprecision.hh"
#include "outlinermath.hh"
#include "outlinerdebug.hh"
#include "outlinerprocessor.hh"
#include "outlinerprocessorcrosssection.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Model processing ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

Processor::Processor(const char* fileNameIn,
                     unsigned int multiplierIn,
                     bool smoothIn,
                     bool mergedLinesIn,
                     float linewidthIn,
                     OutlinerBox3D boundingBoxIn,
                     outlinerreal stepxIn,
                     outlinerreal stepyIn,
                     outlinerreal stepzIn,
                     enum outlinerdirection directionIn,
                     enum outlineralgorithm algorithmIn,
                     unsigned int holethresholdIn,
                     bool labelsIn,
                     IndexedMesh& indexedIn) :
  fileName(fileNameIn),
  multiplier(multiplierIn),
  smooth(smoothIn),
  mergedLines(mergedLinesIn),
  linewidth(linewidthIn),
  svg(0),
  boundingBox(boundingBoxIn),
  stepx(stepxIn),
  stepy(stepyIn),
  stepz(stepzIn),
  direction(directionIn),
  algorithm(algorithmIn),
  holethreshold(holethresholdIn),
  labels(labelsIn),
  planviewBoundingBoxStart(DirectionOperations::outputx(direction,boundingBox.start),
                           DirectionOperations::outputy(direction,boundingBox.start)),
  planviewBoundingBoxEnd(DirectionOperations::outputx(direction,boundingBox.end),
                         DirectionOperations::outputy(direction,boundingBox.end)),
  planviewBoundingBox(planviewBoundingBoxStart,planviewBoundingBoxEnd),
  matrix(planviewBoundingBox,
         stepxIn,
         stepyIn),
  indexed(indexedIn) {
  debugf("algorithm %u=%u", algorithm, algorithmIn);
  if (holethreshold > outlinermaxholethreshold) {
    errf("Cannot compute hole thresholds larger than %u (%u given)", outlinermaxholethreshold, holethreshold);
  }
  boundingBox2D.start.x = DirectionOperations::outputx(direction,boundingBox.start);
  boundingBox2D.start.y = DirectionOperations::outputy(direction,boundingBox.start);
  boundingBox2D.end.x = DirectionOperations::outputx(direction,boundingBox.end);
  boundingBox2D.end.y = DirectionOperations::outputy(direction,boundingBox.end);
  OutlinerBox2D boundingBox2DExtended(boundingBox2D);
  if (labels) {
    boundingBox2DExtended.start.y -= ((outlinertitlespacey * stepy) / multiplier);
    boundingBox2DExtended.end.y += ((outlinertitlespacey * stepy) / multiplier);
  }
  svg = createSvg(fileName,boundingBox2DExtended,direction);
}

Processor::~Processor() {
  svgDone();
}

void
Processor::svgDone() {
  if (svg != 0) {
    
    // Check that file I/O was ok
    if (!svg->ok()) {
      errf("File output to %s failed", fileName);
      exit(1);
    }

    // Delete the object
    delete svg;
    svg = 0;
    
  }
}

bool
Processor::processScene(const aiScene* scene,
                        unsigned int nCrossSections,
                        struct ProcessorCrossSectionInfo* crossSections) {
  
  debugf("processScene");
  assert(scene != 0);

  // First, go through each part of the picture, and determine if
  // there's material in it. Construct a matrix representing the
  // results.
  
  unsigned int xIndex = 0;

  infof("computing material matrix...");
  for (outlinerreal x = DirectionOperations::outputx(direction,boundingBox.start);
       x <= DirectionOperations::outputx(direction,boundingBox.end);
       x += stepx) {

    debugf("main loop x = %.2f (%u)", x, xIndex);
    assert(x >= DirectionOperations::outputx(direction,boundingBox.start));
    assert(x <= DirectionOperations::outputx(direction,boundingBox.end));
    unsigned int yIndex = 0;
    if (xIndex >= matrix.xIndexSize) {
      debugf("processScene %u/%u", xIndex, matrix.xIndexSize);
    }
    assert(xIndex < matrix.xIndexSize);
    
    for (outlinerreal y = DirectionOperations::outputy(direction,boundingBox.start);
         y <= DirectionOperations::outputy(direction,boundingBox.end);
         y += stepy) {
      
      assert(x >= DirectionOperations::outputx(direction,boundingBox.start));
      assert(x <= DirectionOperations::outputx(direction,boundingBox.end));
      assert(y >= DirectionOperations::outputy(direction,boundingBox.start));
      assert(y <= DirectionOperations::outputy(direction,boundingBox.end));
      if (yIndex >= matrix.yIndexSize) {
        debugf("processScene %u,%u/%u,%u", xIndex, yIndex, matrix.xIndexSize, matrix.yIndexSize);
      }
      assert(yIndex < matrix.yIndexSize);
      if (x > -0.605 && x < -0.595) {
        debugf("checking (%.2f,%.2f) or (%u,%u)",x,y,xIndex,yIndex);
      } else {
        deepdebugf("checking (%.2f,%.2f)",x,y);
      }
      if (sceneHasMaterial(scene,indexed,x,y)) {
        debugf("material at (%.2f,%.2f) ie. %u,%u",x,y,xIndex,yIndex);
        matrix.setMaterialMatrix(xIndex,yIndex);
      }
      
      yIndex++;
      
    }
    
    xIndex++;
    
  }

  // Now there's a matrix filled with a flag for each coordinate,
  // whether there was material or not. Determine if we want to fill
  // small imperfections, removing small holes.
  unsigned int holes = 0;
  unsigned int holeMinSize = 9999;
  unsigned int holeMaxSize = 0;
  if (holethreshold > 0) {
    infof("filtering holes...");
    for (xIndex = 1; xIndex < matrix.xIndexSize; xIndex++) {
      for (unsigned int yIndex = 0; yIndex < matrix.yIndexSize; yIndex++) {
        if (!matrix.getMaterialMatrix(xIndex-1,yIndex)) continue;
        if (!matrix.getMaterialMatrix(xIndex,yIndex)) {
          unsigned int n;
          unsigned int holeXtable[outlinermaxholethreshold];
          unsigned int holeYtable[outlinermaxholethreshold];
          if (holeIsEqualOrSmallerThan(xIndex,yIndex,holethreshold,n,outlinermaxholethreshold,holeXtable,holeYtable)) {
            debugf("  correcting a hole of %u pixels at (%u,%u)", n, xIndex, yIndex);
            holes++;
            if (holeMinSize > n) holeMinSize = n;
            if (holeMaxSize < n) holeMaxSize = n;
            for (unsigned int i = 0; i < n; i++) {
              matrix.setMaterialMatrix(holeXtable[i],holeYtable[i]);
            }
          }
        }
      }
    }
  }

  if (holes > 0) {
    infof("  removed %u holes of size %u..%u pixels", holes, holeMinSize, holeMaxSize);
  }
  
  // Now there's a matrix filled with a flag for each coordinate,
  // whether there was material or not. And small holes have been filled per above.
  // Draw the output based on all this.
  matrixToSvg(&matrix,svg,
              DirectionOperations::outputx(direction,boundingBox.start),
              DirectionOperations::outputy(direction,boundingBox.start),
              stepx,
              stepy);

  // Process cross sections
  if (!processSceneCrossSections(scene,nCrossSections,crossSections)) {
    return(0);
  }
  
  // Main result (plan view) is also done, flush the image output
  svgDone();
  
  // Done, all good
  return(1);
}

void
Processor::matrixToSvg(MaterialMatrix* theMatrix,
                       SvgCreator* theSvg,
                       outlinerreal xStart,
                       outlinerreal yStart,
                       outlinerreal xStep,
                       outlinerreal yStep) {
  infof("constructing output core of %ux%u pixels...",
        theMatrix->xIndexSize, theMatrix->yIndexSize);
  deepdebugf("algorithm %u", algorithm);
  debugf("  covering model from (%.2f,%.2,f) to (%.2f,%.2f)",
         xStart, yStart,
         xStart + theMatrix->xIndexSize * xStep,
         yStart + theMatrix->yIndexSize * yStep);
  unsigned int nBorderTo;
  bool borderTablePrev[maxNeighbors];
  unsigned int borderTableX[maxNeighbors];
  unsigned int borderTableY[maxNeighbors];
  for (unsigned int xIndex = 0; xIndex < theMatrix->xIndexSize; xIndex++) {
    for (unsigned int yIndex = 0; yIndex < theMatrix->yIndexSize; yIndex++) {
      if (theMatrix->getMaterialMatrix(xIndex,yIndex)) {
        outlinerreal x = xStart + xIndex * xStep;
        outlinerreal y = yStart + yIndex * yStep;
        debugf("algorithm %u", algorithm);
        switch (algorithm) {
        case alg_pixel:
          infof("pixel alg %u,%u", xIndex, yIndex);
          theSvg->pixel(x,y);
          break;
        case alg_borderpixel:
          debugf("borderpixel alg %u,%u", xIndex, yIndex);
          if (isBorder(xIndex,yIndex,theMatrix,nBorderTo,0,0,0,0)) {
            theSvg->pixel(x,y);
          }
          break;
        case alg_borderline:
          debugf("borderline alg %u,%u", xIndex, yIndex);
          if (isBorder(xIndex,yIndex,theMatrix,nBorderTo,maxNeighbors,borderTablePrev,borderTableX,borderTableY)) {
            if (nBorderTo == 0) {
              theSvg->pixel(x,y);
            } else {
              for (unsigned int b = 0; b < nBorderTo; b++) {
                assert(outlinersaneindex(borderTableX[b]));
                assert(outlinersaneindex(borderTableY[b]));
                if (borderTablePrev[b]) {
                  outlinerreal otherX = indexToCoordinateX(borderTableX[b]);
                  outlinerreal otherY = indexToCoordinateY(borderTableY[b]);
                  deepdeepdebugf("calling theSvg->line");
                  theSvg->line(otherX,otherY,x,y);
                }
              }
            }
          }
          break;
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
}

bool
Processor::sceneHasMaterial(const aiScene* scene,
                            IndexedMesh& indexed,
                            outlinerreal x,
                            outlinerreal y) {
  assert(scene != 0);
  deepdeepdebugf("checking for material at (%.2f,%.2f)", x, y);
  return(nodeHasMaterial(scene,scene->mRootNode,indexed,x,y));
}

bool
Processor::nodeHasMaterial(const aiScene* scene,
                           const aiNode* node,
                           IndexedMesh& indexed,
                           outlinerreal x,
                           outlinerreal y) {
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
                           outlinerreal x,
                           outlinerreal y) {
  assert(scene != 0);
  assert(mesh != 0);
  unsigned int nFaces = 0;
  const aiFace** faces = 0;
  indexed.getFaces(mesh,x,y,&nFaces,&faces);
  deepdebugf("meshHasMaterial normally %u faces but on this tile %u faces", mesh->mNumFaces,nFaces);
  for (unsigned int f = 0; f < nFaces; f++) {
    if (faceHasMaterial(scene,mesh,faces[f],x,y)) {
      return(1);
    }
  }
  return(0);
}

void
Processor::faceGetVertices2D(const aiMesh* mesh,
                             const aiFace* face,
                             enum outlinerdirection thisDirection,
                             OutlinerTriangle2D& t) {
  OutlinerTriangle3D ft;
  faceGetVertices3D(mesh,face,ft);
  aiVector2D aOne(DirectionOperations::outputx(thisDirection,ft.a),DirectionOperations::outputy(thisDirection,ft.a));
  aiVector2D bOne(DirectionOperations::outputx(thisDirection,ft.b),DirectionOperations::outputy(thisDirection,ft.b));
  aiVector2D cOne(DirectionOperations::outputx(thisDirection,ft.c),DirectionOperations::outputy(thisDirection,ft.c));
  t.a = aOne;
  t.b = bOne;
  t.c = cOne;
}

void
Processor::faceGetVertices3D(const aiMesh* mesh,
                             const aiFace* face,
                             OutlinerTriangle3D& t) {
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
  t.a = *vertexA;
  t.b = *vertexB;
  t.c = *vertexC;
}

bool
Processor::faceHasMaterial(const aiScene* scene,
                           const aiMesh* mesh,
                           const aiFace* face,
                           outlinerreal x,
                           outlinerreal y) {
  assert(scene != 0);
  assert(mesh != 0);
  OutlinerTriangle2D t;
  faceGetVertices2D(mesh,face,direction,t);
  OutlinerVector2D point(x,y);
  OutlinerVector2D stepBoundingBox(x+stepx,y+stepy);
  OutlinerBox2D thisBox(point,stepBoundingBox);
  if (OutlinerMath::boundingBoxIntersectsTriangle2D(t,thisBox)) {
    deepdebugf("found out that (%.2f,%.2f) is hitting a face",x,y);
    return(1);
  }
  return(0);
}

bool
Processor::coordinatesInTable(const unsigned int xIndex,
                              const unsigned int yIndex,
                              const unsigned int n,
                              const unsigned int* tableX,
                              const unsigned int* tableY) {
  assert(tableX != 0);
  assert(tableY != 0);
  assert(tableX != tableY);
  for (unsigned int i = 0; i < n; i++) {
    if (tableX[i] == xIndex && tableY[i] == yIndex) {
      return(1);
    }
  }
  return(0);
}

bool
Processor::holeIsEqualOrSmallerThan(unsigned int xIndex,
                                    unsigned int yIndex,
                                    unsigned int holethreshold,
                                    unsigned int& n,
                                    unsigned int tableSize,
                                    unsigned int* holeXtable,
                                    unsigned int* holeYtable) {
  assert(tableSize <= outlinermaxholethreshold);
  assert(holethreshold <= outlinermaxholethreshold);
  assert(!matrix.getMaterialMatrix(xIndex,yIndex));
  unsigned int nInvestigate = 1;
  const unsigned int maxInvestigate = 2 * outlinermaxholethreshold;
  unsigned int investigateXtable[maxInvestigate];
  unsigned int investigateYtable[maxInvestigate];
  investigateXtable[0] = xIndex;
  investigateYtable[0] = yIndex;
  n = 0;
  while (nInvestigate > 0) {

    // Take the first entry from the investigation table
    xIndex = investigateXtable[0];
    yIndex = investigateYtable[0];
    for (unsigned int m = 1; m < nInvestigate; m++) {
      investigateXtable[m-1] = investigateXtable[m];
      investigateYtable[m-1] = investigateYtable[m];
    }
    nInvestigate--;
    
    // Does the entry have material? If yes, we don't need to consider it further.
    if (matrix.getMaterialMatrix(xIndex,yIndex)) continue;

    // Otherwise, this entry is in the hole. But is the hole already too large?
    if (n == holethreshold) return(0);
    
    // Move the entry to the list of coordinates that are in the hole
    assert(n < outlinermaxholethreshold);
    assert(n < tableSize);
    holeXtable[n] = xIndex;
    holeYtable[n] = yIndex;
    n++;

    // And then we need to investigate the neighbours of that entry as
    // well. Find out who the neighbours are.
    const unsigned int maxNeighbours = 8;
    unsigned int nNeigh = 0;
    unsigned int neighXtable[maxNeighbours];
    unsigned int neighYtable[maxNeighbours];
    getNeighbours(xIndex,yIndex,nNeigh,maxNeighbours,neighXtable,neighYtable);

    // See if the neighbours have already seen as part of the hole or
    // are already in the investigation table. If so, no need to
    // consider them more. Otherwise, add the neighbours to the
    // investigation table.
    for (unsigned int i = 0; i < nNeigh; i++) {
      unsigned int neighX = neighXtable[i];
      unsigned int neighY = neighYtable[i];
      if (coordinatesInTable(neighX,neighY,n,holeXtable,holeYtable)) continue;
      if (coordinatesInTable(neighX,neighY,nInvestigate,investigateXtable,investigateYtable)) continue;
      if (nInvestigate == maxInvestigate) return(0);
      investigateXtable[nInvestigate] = neighX;
      investigateYtable[nInvestigate] = neighY;
      nInvestigate++;
    }
  }
  return(1);
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

  // Order is important, callers want the neighbors so that the
  // immediate connecting (non-diagonal) neighbors are first.
  
  // Left and right neighbors at the same level
  if (xIndex > 0)                                                     { tableX[n] = xIndex-1; tableY[n] = yIndex;   n++; }
  if (xIndex < matrix.xIndexSize-1)                                   { tableX[n] = xIndex+1; tableY[n] = yIndex;   n++; }
  
  // Top and bottom neighbours
  if (yIndex > 0)                                                     { tableX[n] = xIndex;   tableY[n] = yIndex-1; n++; }
  if (yIndex < matrix.yIndexSize-1)                                   { tableX[n] = xIndex;   tableY[n] = yIndex+1; n++; }
  
  // Left side corner neighbours
  if (xIndex > 0 && yIndex > 0)                                       { tableX[n] = xIndex-1; tableY[n] = yIndex-1; n++; }
  if (xIndex > 0 && yIndex < matrix.yIndexSize-1)                     { tableX[n] = xIndex-1; tableY[n] = yIndex+1; n++; }

  // Right side corner neighbours
  if (xIndex < matrix.xIndexSize-1 && yIndex > 0)                     { tableX[n] = xIndex+1; tableY[n] = yIndex-1; n++; }
  if (xIndex < matrix.xIndexSize-1 && yIndex < matrix.yIndexSize-1)   { tableX[n] = xIndex+1; tableY[n] = yIndex+1; n++; }
  
  // Done
  assert(n <= tableSize);
}

bool
Processor::closerNeighborExists(const unsigned int thisX,
                                const unsigned int thisY,
                                const unsigned int xIndex,
                                const unsigned int yIndex,
                                const unsigned int nNeighbors,
                                const unsigned int* neighborTableX,
                                const unsigned int* neighborTableY) {
  if (thisX == xIndex || thisY == yIndex) return(0);
  for (unsigned int n = 0; n < nNeighbors; n++) {
    unsigned int neighX = neighborTableX[n];
    unsigned int neighY = neighborTableY[n];
    if ((xIndex == neighX - 1 || xIndex == neighX + 1) && yIndex == neighY) return(1);
    if ((yIndex == neighY - 1 || yIndex == neighY + 1) && xIndex == neighX) return(1);
  }
  return(0);
}

bool
Processor::isBorder(unsigned int xIndex,
                    unsigned int yIndex,
                    MaterialMatrix* theMatrix,
                    unsigned int& nBorderTo,
                    unsigned int borderTableSize,
                    bool* borderTablePrev,
                    unsigned int* borderTableX,
                    unsigned int* borderTableY) {
  assert(borderTableSize <= maxNeighbors);
  assert(borderTableSize == 0 || borderTableX != 0);
  assert(borderTableSize == 0 || borderTableY != 0);
  unsigned int n;
  unsigned int tableX[maxNeighbors];
  unsigned int tableY[maxNeighbors];
  getNeighbours(xIndex,yIndex,n,maxNeighbors,tableX,tableY);
  debugf("point %u,%u has %u neighbors", xIndex, yIndex, n);
  nBorderTo = 0;
  bool ans = 0;
  for (unsigned int i = 0; i < n; i++) {
    unsigned int neighX = tableX[i];
    unsigned int neighY = tableY[i];
    if (!theMatrix->getMaterialMatrix(neighX,neighY)) {
      debugf("point %u,%u is a border due to %u,%u", xIndex, yIndex, neighX, neighY);
      ans = 1;
    } else {
      if (nBorderTo < borderTableSize) {
        unsigned int dummy;
        if (!isBorder(neighX, neighY, theMatrix, dummy, 0, 0, 0, 0)) continue;
        if (closerNeighborExists(xIndex, yIndex, neighX, neighY, nBorderTo, borderTableX, borderTableY)) continue;
        bool prev =  ((neighX < xIndex ||
                       (neighX == xIndex && neighY < yIndex)));
        deepdebugf("adding %uth border (prev=%u) to from (%u,%u) to (%u,%u)",
                   nBorderTo+1,
                   prev,
                   neighX,neighY,
                   xIndex,yIndex);
        borderTablePrev[nBorderTo] = prev;
        borderTableX[nBorderTo] = neighX;
        borderTableY[nBorderTo] = neighY;
        nBorderTo++;
      }
    }
  }
  if (!ans) {
    debugf("point %u,%u is inside cave", xIndex, yIndex);
  }
  return(ans);
}

unsigned int
Processor::coordinateXToIndex(outlinerreal x) {
  outlinerreal xStart = DirectionOperations::outputx(direction,boundingBox.start);
  outlinerreal xEnd = DirectionOperations::outputx(direction,boundingBox.end);
  assert(x >= xStart);
  assert(x <= xEnd);
  return((x - xStart)/stepx);
}

unsigned int
Processor::coordinateYToIndex(outlinerreal y) {
  outlinerreal yStart = DirectionOperations::outputy(direction,boundingBox.start);
  outlinerreal yEnd = DirectionOperations::outputy(direction,boundingBox.end);
  assert(y >= yStart);
  assert(y <= yEnd);
  return((y - yStart)/stepy);
}

outlinerreal
Processor::indexToCoordinateX(unsigned int xIndex) {
  outlinerreal xStart = DirectionOperations::outputx(direction,boundingBox.start);
  return(xStart + stepx * xIndex);
}

outlinerreal
Processor::indexToCoordinateY(unsigned int yIndex) {
  outlinerreal yStart = DirectionOperations::outputy(direction,boundingBox.start);
  return(yStart + stepy * yIndex);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// SVG Object Creation ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

SvgCreator*
Processor::createSvg(const char* svgFileName,
                     const OutlinerBox2D& svgBoundingBox,
                     enum outlinerdirection svgDirection) {
  
  // Calculate sizes
  outlinerreal xOutputStart;
  outlinerreal xOutputEnd;
  outlinerreal yOutputStart;
  outlinerreal yOutputEnd;
  outlinerreal xSize;
  outlinerreal ySize;
  unsigned int xSizeInt;
  unsigned int ySizeInt;
  outlinerreal xFactor;
  outlinerreal yFactor;
  createSvgCalculateSizes(svgBoundingBox,
                          stepx,stepy,
                          svgDirection,
                          xOutputStart,xOutputEnd,
                          yOutputStart,yOutputEnd,
                          xSize,ySize,
                          xSizeInt,ySizeInt,
                          xFactor,yFactor);
  infof("SVG %s size x %.2f..%.2f step %.2f xsize %.2f xsizeint %u",
        svgFileName,
        xOutputStart, xOutputEnd, stepx, xSize, xSizeInt);
  infof("  size y %.2f..%.2f step %.2f ysize %.2f ysizeint %u",
        yOutputStart, yOutputEnd, stepy, ySize, ySizeInt);
  infof("SVG size will be %u x %u (%u x %u multiplied)",
        xSizeInt, ySizeInt,
        xSizeInt*multiplier, ySizeInt*multiplier);
  
  // Create the object
  SvgCreator* result = new SvgCreator(svgFileName,
                                      xSizeInt,ySizeInt,
                                      multiplier,
                                      xOutputStart,yOutputStart,
                                      xFactor,yFactor,
                                      smooth,mergedLines,
                                      linewidth);
  
  // Check for allocation success
  deepdebugf("svg allocated");
  if (result == 0) {
    errf("Cannot allocate SvgCreator object");
    exit(1);
  }
  
  // Check that we were able to open the file
  deepdebugf("svg initial check");
  if (!result->ok()) {
    errf("File open for writing  to %s failed", fileName);
    exit(1);
  }

  // All good. Return.
  deepdebugf("svg creation in processor done");
  return(result);
}

void
Processor::createSvgCalculateSizes(const OutlinerBox2D& svgBoundingBox,
                                   const outlinerreal stepx,
                                   const outlinerreal stepy,
                                   const enum outlinerdirection svgDirection,
                                   outlinerreal& xOutputStart,
                                   outlinerreal& xOutputEnd,
                                   outlinerreal& yOutputStart,
                                   outlinerreal& yOutputEnd,
                                   outlinerreal& xSize,
                                   outlinerreal& ySize,
                                   unsigned int& xSizeInt,
                                   unsigned int& ySizeInt,
                                   outlinerreal& xFactor,
                                   outlinerreal& yFactor) {
  
  xOutputStart = svgBoundingBox.start.x;
  xOutputEnd = svgBoundingBox.end.x;
  yOutputStart = svgBoundingBox.start.y;
  yOutputEnd = svgBoundingBox.end.y;
  xSize = (xOutputEnd - xOutputStart) / stepx;
  ySize = (yOutputEnd - yOutputStart) / stepy;
  xSizeInt = xSize;
  ySizeInt = ySize;
  xFactor = 1 / stepx;
  yFactor = 1 / stepy;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Cross-Sections /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
Processor::processSceneCrossSections(const aiScene* scene,
                                     unsigned int nCrossSections,
                                     struct ProcessorCrossSectionInfo* crossSections) {
  for (unsigned int c = 0; c < nCrossSections; c++) {
    const struct ProcessorCrossSectionInfo* crossSection = &crossSections[c];
    if (!processSceneCrossSection(scene,c,crossSection)) {
      return(0);
    }
  }
  return(1);
}

bool
Processor::processSceneCrossSection(const aiScene* scene,
                                    unsigned int c,
                                    const struct ProcessorCrossSectionInfo* crossSection) {
  infof("cross section %u at (%.2f,%.2f)-(%.2f,%.2f) to file %s",
        c,
        crossSection->start.x,
        crossSection->start.y,
        crossSection->end.x,
        crossSection->end.y,
        crossSection->filename);
  ProcessorCrossSection csproc(crossSection->filename,
                               crossSection->label,
                               DirectionOperations::screenx(direction),
                               crossSection->start,
                               crossSection->end,
                               stepz,
                               *this);
  csproc.processSceneCrossSection(scene);
  if (crossSection->label != 0) {
    OutlinerLine2D actualLine;
    csproc.getLineActualEndPoints(actualLine,
                                  outlinercrosssectionextraline * stepy);
    addCrossSectionLine(crossSection->label,actualLine);
  }
  return(1);
}

void
Processor::addCrossSectionLine(const char* label,
                               OutlinerLine2D& actualLine) {
  assert(svg != 0);
  assert(label != 0);
  infof("addCrossSectionLine (%.2f,%.2f) to (%.2f,%.2f)",
        actualLine.start.x,actualLine.start.y,
        actualLine.end.x,actualLine.end.y);
  svg->line(actualLine.start.x,actualLine.start.y,
            actualLine.end.x,actualLine.end.y,
            1);
  infof("    process cross-section line text %.2f - font %u * 0.5 * pixelXSize %.2f = %.2f",
        actualLine.end.x, outlinerdefaultfontxsize, svg->getPixelXSize(),
        actualLine.end.x - outlinerdefaultfontxsize * 0.5 * svg->getPixelXSize());
  svg->text(actualLine.end.x - outlinerdefaultfontxsize * 0.5 * svg->getPixelXSize(),
            actualLine.end.y + outlinerdefaultfontysize * 0.1 * svg->getPixelYSize(),
            label);
}


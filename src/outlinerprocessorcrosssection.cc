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
// Object creation and destruction ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

ProcessorCrossSection::ProcessorCrossSection(const char* fileNameIn,
                                             const char* labelIn, // 0 if no label desired
                                             enum outlinerdirection sliceDirectionIn,
                                             const OutlinerVector2D& lineStartIn,
                                             const OutlinerVector2D& lineEndIn,
                                             outlinerreal stepzIn,
                                             Processor& procIn) :
  fileName(fileNameIn),
  label(labelIn),
  sliceDirection(sliceDirectionIn),
  lineStart(lineStartIn),
  lineEnd(lineEndIn),
  stepz(stepzIn),
  matrix(0),
  proc(procIn),
  svg(0) {
  calculateLineEquation();
}

ProcessorCrossSection::~ProcessorCrossSection() {
  deleteSvg();
  deleteMatrix();
}

void
ProcessorCrossSection::deleteSvg(void) {
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

void
ProcessorCrossSection::deleteMatrix(void) {
  if (matrix != 0) {
    
    // Delete the object
    delete matrix;
    matrix = 0;
    
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Main program for generating a cross section ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
ProcessorCrossSection::processSceneCrossSection(const aiScene* scene) {

  // Get rid of previous image and matrix, if any
  deleteSvg();
  deleteMatrix();
  
  // Debugs
  debugf("process scene cross section");
  
  // Determine the bounding box of what we see in the cross section
  getSliceVerticalBoundingBox(scene,sliceVerticalBoundingBox);
  infof("  cross section bounding box (%.2f,%.2f) to (%.2f,%.2f)",
        sliceVerticalBoundingBox.start.x, sliceVerticalBoundingBox.start.y,
        sliceVerticalBoundingBox.end.x, sliceVerticalBoundingBox.end.y);
  debugf("cross section bounding box (%.2f,%.2f) to (%.2f,%.2f)",
        sliceVerticalBoundingBox.start.x, sliceVerticalBoundingBox.start.y,
        sliceVerticalBoundingBox.end.x, sliceVerticalBoundingBox.end.y);
  debugf("steps %.2f and %.2f", proc.stepy, stepz);
  
  // Increase bounding box to each side for the actual image
  OutlinerBox2D sliceVerticalBoundingBoxExtended = sliceVerticalBoundingBox;
  sliceVerticalBoundingBoxExtended.start.x -= freespacearound*proc.stepy;
  sliceVerticalBoundingBoxExtended.start.y -= freespacearound*stepz;
  sliceVerticalBoundingBoxExtended.end.x += freespacearound*proc.stepy;
  sliceVerticalBoundingBoxExtended.end.y += freespacearound*stepz;
  if (label != 0) {
    outlinerreal incr = (outlinertitlespacey*stepz) / proc.multiplier;
    infof("increasing cross-section image vertical size by %.2f (%u*%.2f)/%.2f to accommodate label",
          incr, outlinertitlespacey, stepz, proc.multiplier);
    sliceVerticalBoundingBoxExtended.end.y += incr;
    if ((sliceVerticalBoundingBoxExtended.end.x - sliceVerticalBoundingBoxExtended.start.x)/proc.stepy < outlinertitlespacex) {
      outlinerreal incr2 = (outlinertitlespacex*proc.stepy) / proc.multiplier;
      infof("increasing cross-section image horizontal size by %.2f (%u*%.2f)/%.2f to accommodate label",
            incr2, outlinertitlespacex, proc.stepy, proc.multiplier);
      sliceVerticalBoundingBoxExtended.end.x = sliceVerticalBoundingBoxExtended.start.x + incr2;
    }
  }
  infof("  cross section bounding box after adjustment (%.2f,%.2f) to (%.2f,%.2f)",
        sliceVerticalBoundingBox.start.x, sliceVerticalBoundingBox.start.y,
        sliceVerticalBoundingBox.end.x, sliceVerticalBoundingBox.end.y);
  
  // Create a material matrix
  matrix = new MaterialMatrix(sliceVerticalBoundingBox,
                              proc.stepy,
                              stepz);
  infof("  slice bounding box after matrix creation (%.2f,%.2f) to (%.2f,%.2f) and steps %.2f and %.2f",
         sliceVerticalBoundingBox.start.x, sliceVerticalBoundingBox.start.y,
         sliceVerticalBoundingBox.start.x + proc.stepy * matrix->xIndexSize,
         sliceVerticalBoundingBox.start.y + stepz * matrix->yIndexSize,
         proc.stepy, stepz);
  infof("  extended slice bounding box after matrix creation (%.2f,%.2f) to (%.2f,%.2f) and steps %.2f and %.2f",
        sliceVerticalBoundingBoxExtended.start.x, sliceVerticalBoundingBoxExtended.start.y,
        sliceVerticalBoundingBoxExtended.end.x, sliceVerticalBoundingBoxExtended.end.y,
        proc.stepy, stepz);
  
  // Create an image base of the bounding size
  svg = proc.createSvg(fileName,sliceVerticalBoundingBoxExtended,sliceDirection);

  debugf("  slice bounding box after svg creation (%.2f,%.2f) to (%.2f,%.2f) and steps %.2f and %.2f",
         sliceVerticalBoundingBox.start.x, sliceVerticalBoundingBox.start.y,
         sliceVerticalBoundingBox.end.x, sliceVerticalBoundingBox.end.y,
         proc.stepy, stepz);
  
  // Process the actual cross section
  drawCrossSection(scene);  
  debugf("  slice bounding box after drawing (%.2f,%.2f) to (%.2f,%.2f) and steps %.2f and %.2f",
         sliceVerticalBoundingBox.start.x, sliceVerticalBoundingBox.start.y,
         sliceVerticalBoundingBox.end.x, sliceVerticalBoundingBox.end.y,
         proc.stepy, stepz);

  // Add label, if needed
  if (label != 0) {
    char labelText[50];
    const char* labelPrefix = "Cross-section ";
    unsigned int neededSpace = strlen(labelPrefix) + strlen(label) + 1;
    memset(labelText,0,sizeof(labelText));
    if (sizeof(labelText) < neededSpace) {
      errf("Cross-section label length too long");
      snprintf(labelText,sizeof(labelText)-1,"...");
    } else {
      snprintf(labelText,sizeof(labelText)-1,"%s%s", labelPrefix, label);
    }
    debugf("    cross section label y from end %.2f + (empty %u *  pixelysize %.2f) = %.2f",
           sliceVerticalBoundingBox.end.y, outlinertitlespaceempty, svg->getPixelYSize(),
           sliceVerticalBoundingBox.end.y + (outlinertitlespaceempty * svg->getPixelYSize()));
    svg->text(sliceVerticalBoundingBox.start.x,
              sliceVerticalBoundingBox.end.y + (outlinertitlespaceempty * svg->getPixelYSize()),
              labelText);
  }
  
  // Now there's a matrix filled with a flag for each coordinate,
  // whether there was material or not. And small holes have been filled per above.
  // Draw the output based on all this.
  proc.matrixToSvg(matrix,svg,
                   sliceVerticalBoundingBox.start.x,
                   sliceVerticalBoundingBox.start.y,
                   proc.stepx,
                   stepz);

  // Main result (plan view) is done, flush the image output
  deleteSvg();

  // Done
  return(1);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Line equations and iterators ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
ProcessorCrossSection::calculateLineEquation(void) {

  // Debugs
  debugf("calculate cross section line equation for (%.2f,%.2f)-(%.2f,%.2f)",
         lineStart.x, lineStart.y, lineEnd.x, lineEnd.y);

  //
  // Calculate the line as an equation
  //
  //   (x,y) = (x0 + n * xStep, y0 + n * yStep)
  //
  // where x0 = lineStart.x, y0 = lineStart.y, xStep = lineStepX, and
  // yStep = lineStepY. From this equation we can also calculate y for
  // any given x:
  //
  //   n = (x-x0) / xStep
  //   y = y0 + n * yStep
  //
  // Similarly, for the other direction:
  //
  //   n = (y-y0) / yStep
  //   x = x0 + n * xStep
  //
  
  // Calculations
  xDifference = lineEnd.x - lineStart.x;
  yDifference = lineEnd.y - lineStart.y;
  if (xDifference == 0 && yDifference == 0) {
    errf("Cross-section starting and ending points cannot be same");
    exit(1);
  }
  outlinerreal totalDifference = xDifference + yDifference;
  outlinerreal xDifferenceFraction = xDifference / totalDifference;
  outlinerreal yDifferenceFraction = yDifference / totalDifference;
  lineLength = sqrt(xDifference*xDifference + yDifference*yDifference);
  lineStep = (proc.stepx * xDifferenceFraction) + (proc.stepy * yDifferenceFraction);
  lineSteps = lineLength/lineStep;
  lineStepX = xDifference / lineSteps;
  lineStepY = yDifference / lineSteps;
  debugf("cross section line equation length %.2f steps %.2f stepX %.2f stepY %.2f",
         lineLength,
         lineSteps,
         lineStepX,
         lineStepY);
}

outlinerreal
ProcessorCrossSection::calculateLineXBasedOnY(outlinerreal y) {
  
  //
  // We can do this per the equations
  // derived earlier, i.e., calculate x for any given y:
  //
  //   n = (y-y0) / yStep
  //   x = x0 + n * xStep
  //

  // Sanity checks
  if (lineStepY == 0) {
    errf("Cannot calculate actual line end points for a cross section that is horizontal");
    exit(1);
  }

  // Do the actual calculations
  outlinerreal n = (y - lineStart.y) / lineStepY;
  outlinerreal x = lineStart.x + n*lineStepX;
  return(x);
}

void
ProcessorCrossSection::getLineActualEndPoints(OutlinerLine2D& actualLine,
                                              outlinerreal extralineatends) {
  // We need to take the bounding box of the cross section, and the xy
  // plane (plan-view) position of the points where the bounding box
  // and the line equation intersect. We can do this per the equations
  // derived earlier, i.e., calculate x for any given y:
  //
  //   n = (y-y0) / yStep
  //   x = x0 + n * xStep
  //
  // In addition, we will adjust the line such that there is
  // extralineatends more space in y direction at both ends of the
  // line.
  //
  // Also, note that the slice bounding box (x,y) is (y,z) in the 3d
  // model.
  //
  
  outlinerreal bottomY = sliceVerticalBoundingBox.start.x - extralineatends;
  outlinerreal bottomX = calculateLineXBasedOnY(bottomY);
  
  outlinerreal topY = sliceVerticalBoundingBox.end.x + extralineatends;
  outlinerreal topX = calculateLineXBasedOnY(topY);

  actualLine.start.x = bottomX;
  actualLine.start.y = bottomY;
  
  actualLine.end.x = topX;
  actualLine.end.y = topY;
}

void
ProcessorCrossSection::lineIteratorInit(struct ProcessorCrossSectionLineIterator& iter) {
  iter.point.x = lineStart.x;
  iter.point.y = lineStart.y;
  iter.step = 0;
}

bool
ProcessorCrossSection::lineIteratorDone(struct ProcessorCrossSectionLineIterator& iter) {
  if (lineEnd.x > lineStart.x && iter.point.x > lineEnd.x) return(1);
  if (lineEnd.x < lineStart.x && iter.point.x < lineEnd.x) return(1);
  if (lineEnd.y > lineStart.y && iter.point.y > lineEnd.y) return(1);
  if (lineEnd.y < lineStart.y && iter.point.y < lineEnd.y) return(1);
  return(0);
}

void
ProcessorCrossSection::lineIteratorNext(struct ProcessorCrossSectionLineIterator& iter) {
  iter.point.x += lineStepX;
  iter.point.y += lineStepY;
  iter.step++;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Drawing a cross section ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
ProcessorCrossSection::drawCrossSection(const aiScene* scene) {

  // Sanity checks
  assert(scene != 0);
  debugf("drawing cross section scene");

  // Actual matrix calculations
  drawCrossSectionNode(scene,scene->mRootNode);
}

void
ProcessorCrossSection::drawCrossSectionNode(const aiScene* scene,
                                            const aiNode* node) {
  assert(scene != 0);
  assert(node != 0);
  if (!node->mTransformation.IsIdentity()) {
    errf("Cannot handle transformations yet");
    exit(1);
  }
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    drawCrossSectionMesh(scene,scene->mMeshes[node->mMeshes[j]]);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    drawCrossSectionNode(scene,node->mChildren[i]);
  }
}

void
ProcessorCrossSection::drawCrossSectionMesh(const aiScene* scene,
                                            const aiMesh* mesh) {
  assert(scene != 0);
  assert(mesh != 0);
  bool firstStepInBoundingBoxSeen = 0;
  unsigned int firstStepInBoundingBox = 0;
  struct ProcessorCrossSectionLineIterator iter;
  lineIteratorInit(iter);
  for (; !lineIteratorDone(iter); lineIteratorNext(iter)) {
    deepdebugf("  drawCrossSectionMesh: line iterator step (%.2f,%.2f)", iter.point.x, iter.point.y);
    if (!outlinerbetweenanyorder(sliceVerticalBoundingBox.start.x,
                                 iter.point.y,
                                 sliceVerticalBoundingBox.end.x)) {
      deepdeepdebugf("  skip iterator step outside bounding box");
      continue;
    }
    if (!firstStepInBoundingBoxSeen) {
      firstStepInBoundingBox = iter.step;
      firstStepInBoundingBoxSeen = 1;
      deepdebugf("  setting first step to %u", firstStepInBoundingBox);
    }
    unsigned int nFaces = 0;
    const aiFace** faces = 0;
    infof("  cross section line iterator step %u (starting from %u)", iter.step, firstStepInBoundingBox);
    proc.indexed.getFaces(mesh,iter.point.x,iter.point.y,&nFaces,&faces);
    if (nFaces > 0) {
      deepdebugf("  drawCrossSectionMesh: got %u cross section faces from (%.2f,%.2f)",
                 nFaces, iter.point.x, iter.point.y);
      unsigned int zStep = 0;
      for  (outlinerreal z = sliceVerticalBoundingBox.start.y;
            z <= sliceVerticalBoundingBox.end.y;
            z += stepz) {
        infof("    z step %u (%.2f)", zStep, z);
        deepdebugf("  drawCrossSectionMesh: z iterator step (%.2f,%.2f,%.2f)",
                   iter.point.x, iter.point.y, z);
        for (unsigned int f = 0; f < nFaces; f++) {
          drawCrossSectionFace(scene,mesh,faces[f],
                               iter.step - firstStepInBoundingBox,
                               zStep,
                               iter.point.x, iter.point.y, z);
        }
        zStep++;
      }
    }
  }
}

void
ProcessorCrossSection::drawCrossSectionFace(const aiScene* scene,
                                            const aiMesh* mesh,
                                            const aiFace* face,
                                            unsigned int xyStep,
                                            unsigned int zStep,
                                            outlinerreal x,
                                            outlinerreal y,
                                            outlinerreal z) {
  assert(scene != 0);
  assert(mesh != 0);
  assert(face != 0);
  assert(matrix != 0);
  OutlinerTriangle3D t;
  proc.faceGetVertices3D(mesh,face,t);
  OutlinerBox3D thisBox(x,y,z,x+lineStepX,y+lineStepY,z+stepz);
  char buf[80];
  OutlinerMath::triangleDescribe(t,buf,sizeof(buf));
  deepdeepdebugf("describe done, result = %s", buf);
  if (OutlinerMath::boundingBoxIntersectsTriangle3D(t,thisBox)) {
    infof("    cross section face match at %u,%u (%.2f,%.2f,%.2f)",
          xyStep, zStep, x, y, z);
    deepdebugf("    cross section draw face xyz match     at (%5.2f..%5.2f,%5.2f..%5.2f,%5.2f..%5.2f) triangle %s => matrix %u,%u",
               x, thisBox.end.x, y, thisBox.end.y, z, thisBox.end.z,
               buf, xyStep, zStep);
    matrix->setMaterialMatrix(xyStep,zStep);
  } else {
    infof("    no match to a face at %u,%u",
          xyStep, zStep);
    deepdeepdebugf("      going to debug");
    deepdebugf("    cross section draw face xyz non-match at (%5.2f..%5.2f,%5.2f..%5.2f,%5.2f..%5.2f) "
               "face (%5.2f,%5.2f,%5.2f) x (%5.2f,%5.2f,%5.2f) x (%5.2f,%5.2f,%5.2f) triangle %s",
               x, thisBox.end.x, y, thisBox.end.y, z, thisBox.end.z,
               t.a.x, t.a.y, t.a.z,
               t.b.x, t.b.y, t.b.z,
               t.c.x, t.c.y, t.c.z,
               buf);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Cross section bounding box calculation /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
ProcessorCrossSection::getSliceVerticalBoundingBox(const aiScene* scene,
                                                OutlinerBox2D& sliceVerticalBoundingBox) {
  assert(scene != 0);
  deepdebugf("process cross section bounding box");
  bool set = 0;
  getSliceVerticalBoundingBoxNode(scene,scene->mRootNode,set,sliceVerticalBoundingBox);
}

void
ProcessorCrossSection::getSliceVerticalBoundingBoxNode(const aiScene* scene,
                                                       const aiNode* node,
                                                       bool& set,
                                                       OutlinerBox2D& sliceVerticalBoundingBox) {
  assert(scene != 0);
  assert(node != 0);
  debugf("process node cross section");
  if (!node->mTransformation.IsIdentity()) {
    errf("Cannot handle transformations yet");
    exit(1);
  }
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    getSliceVerticalBoundingBoxMesh(scene,scene->mMeshes[node->mMeshes[j]],
                                    set,
                                    sliceVerticalBoundingBox);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    getSliceVerticalBoundingBoxNode(scene,node->mChildren[i],
                                    set,
                                    sliceVerticalBoundingBox);
  }
}

void
ProcessorCrossSection::getSliceVerticalBoundingBoxMesh(const aiScene* scene,
                                                       const aiMesh* mesh,
                                                       bool& set,
                                                       OutlinerBox2D& sliceVerticalBoundingBox) {
  assert(scene != 0);
  assert(mesh != 0);
  debugf("process mesh cross section");
  struct ProcessorCrossSectionLineIterator iter;
  lineIteratorInit(iter);
  for (; !lineIteratorDone(iter); lineIteratorNext(iter)) {
    unsigned int nFaces = 0;
    const aiFace** faces = 0;
    proc.indexed.getFaces(mesh,iter.point.x,iter.point.y,&nFaces,&faces);
    if (nFaces > 0) {
      deepdebugf("got %u cross section faces from (%.2f,%.2f)", nFaces, iter.point.x, iter.point.y);
      for (unsigned int f = 0; f < nFaces; f++) {
        getSliceVerticalBoundingBoxFace(scene,mesh,faces[f],
                                        iter.point.x, iter.point.y,
                                        set,
                                        sliceVerticalBoundingBox);
      }
    }
  }
}

void
ProcessorCrossSection::getSliceVerticalBoundingBoxFace(const aiScene* scene,
                                                       const aiMesh* mesh,
                                                       const aiFace* face,
                                                       outlinerreal x,
                                                       outlinerreal y,
                                                       bool& set,
                                                       OutlinerBox2D& sliceVerticalBoundingBox) {
  OutlinerTriangle3D t3;
  proc.faceGetVertices3D(mesh,face,t3);
  OutlinerVector2D a(DirectionOperations::outputx(proc.direction,t3.a),DirectionOperations::outputy(proc.direction,t3.a));
  OutlinerVector2D b(DirectionOperations::outputx(proc.direction,t3.b),DirectionOperations::outputy(proc.direction,t3.b));
  OutlinerVector2D c(DirectionOperations::outputx(proc.direction,t3.c),DirectionOperations::outputy(proc.direction,t3.c));
  OutlinerTriangle2D t2(a,b,c);
  OutlinerBox2D thisBox(x,y,x+lineStepX,y+lineStepY);
  if (OutlinerMath::boundingBoxIntersectsTriangle2D(t2,thisBox)) {
    deepdeepdebugf("cross section face (%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f) hits step bounding box (%.2f,%.2f)-(%.2f,%.2f)",
                   mesh->mVertices[face->mIndices[0]].x,mesh->mVertices[face->mIndices[0]].y,mesh->mVertices[face->mIndices[0]].z,
                   mesh->mVertices[face->mIndices[1]].x,mesh->mVertices[face->mIndices[1]].y,mesh->mVertices[face->mIndices[1]].z,
                   mesh->mVertices[face->mIndices[2]].x,mesh->mVertices[face->mIndices[2]].y,mesh->mVertices[face->mIndices[2]].z,
                   thisBox.start.x, thisBox.start.y,
                   thisBox.end.x, thisBox.end.y);
    deepdeepdebugf("cross section direction %s and %s",
                   DirectionOperations::toString(proc.direction),
                   DirectionOperations::toString(sliceDirection));
    OutlinerVector2D aSlice(DirectionOperations::outputy(proc.direction,t3.a),DirectionOperations::outputz(proc.direction,t3.a));
    OutlinerVector2D bSlice(DirectionOperations::outputy(proc.direction,t3.b),DirectionOperations::outputz(proc.direction,t3.b));
    OutlinerVector2D cSlice(DirectionOperations::outputy(proc.direction,t3.c),DirectionOperations::outputz(proc.direction,t3.c));
    OutlinerBox2D faceBoundingBox;
    OutlinerTriangle2D tSlice(aSlice,bSlice,cSlice);
    OutlinerMath::triangleBoundingBox2D(tSlice,faceBoundingBox);
    deepdebugf("    adding a face to cross section bounding box, triangle box (%.2f,%.2f) - (%.2f,%.2f)",
               faceBoundingBox.start.x, faceBoundingBox.start.y,
               faceBoundingBox.end.x, faceBoundingBox.end.y);
    if (!set) {
      sliceVerticalBoundingBox = faceBoundingBox;
      deepdebugf("setting initial cross section bounding box at (%.2f,%.2f) "
                 "to (%.2f,%.2f)-(%.2f,%.2f)",
                 x, y,
                 sliceVerticalBoundingBox.start.x, sliceVerticalBoundingBox.start.y,
                 sliceVerticalBoundingBox.end.x, sliceVerticalBoundingBox.end.y);
      set = 1;
    } else {
      OutlinerBox2D newBoundingBox;
      faceBoundingBox.boxUnion(sliceVerticalBoundingBox,newBoundingBox);
      deepdebugf("testing new cross section bounding box "
                 "for face (%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f) "
                 "at (%.2f,%.2f) as (%.2f,%.2f)-(%.2f,%.2f)",
                 mesh->mVertices[face->mIndices[0]].x,mesh->mVertices[face->mIndices[0]].y,mesh->mVertices[face->mIndices[0]].z,
                 mesh->mVertices[face->mIndices[1]].x,mesh->mVertices[face->mIndices[1]].y,mesh->mVertices[face->mIndices[1]].z,
                 mesh->mVertices[face->mIndices[2]].x,mesh->mVertices[face->mIndices[2]].y,mesh->mVertices[face->mIndices[2]].z,
                 x, y,
                 newBoundingBox.start.x, newBoundingBox.start.y,
                 newBoundingBox.end.x, newBoundingBox.end.y);
      if (!sliceVerticalBoundingBox.equal(newBoundingBox)) {
        sliceVerticalBoundingBox = newBoundingBox;
        deepdebugf("setting new cross section bounding box to (%.2f,%.2f)-(%.2f,%.2f)",
                   sliceVerticalBoundingBox.start.x, sliceVerticalBoundingBox.start.y,
                   sliceVerticalBoundingBox.end.x, sliceVerticalBoundingBox.end.y);
      }
    } 
  } else {
    deepdeepdebugf("cross section face (%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f) "
                   "for direction %s (%.2f,%.2f)-(%.2f,%.2f)-(%.2f,%.2f) does NOT hit "
                   "step bounding box (%.2f,%.2f)-(%.2f,%.2f)",
                   mesh->mVertices[face->mIndices[0]].x,mesh->mVertices[face->mIndices[0]].y,mesh->mVertices[face->mIndices[0]].z,
                   mesh->mVertices[face->mIndices[1]].x,mesh->mVertices[face->mIndices[1]].y,mesh->mVertices[face->mIndices[1]].z,
                   mesh->mVertices[face->mIndices[2]].x,mesh->mVertices[face->mIndices[2]].y,mesh->mVertices[face->mIndices[2]].z,
                   DirectionOperations::toString(proc.direction),
                   t2.a.x, t2.a.y, t2.b.x, t2.b.y, t2.c.x, t2.c.y,
                   thisBox.start.x, thisBox.start.y,
                   thisBox.end.x, thisBox.end.y);
  }
}

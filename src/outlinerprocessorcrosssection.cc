  
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
                                             const HighPrecisionVector2D& lineStartIn,
                                             const HighPrecisionVector2D& lineEndIn,
                                             outlinerhighprecisionreal stepzIn,
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
  sliceVerticalBoundingBox(scene,sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
  infof("  cross section bounding box (%.2f,%.2f) to (%.2f,%.2f)",
        sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
        sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y);
  debugf("cross section bounding box (%.2f,%.2f) to (%.2f,%.2f)",
        sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
        sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y);
  debugf("steps %.2f and %.2f", proc.stepy, stepz);
  
  // Increase bounding box to each side for the actual image
  HighPrecisionVector2D sliceVerticalBoundingBoxExtendedStart = sliceVerticalBoundingBoxStart;
  HighPrecisionVector2D sliceVerticalBoundingBoxExtendedEnd = sliceVerticalBoundingBoxEnd;
  sliceVerticalBoundingBoxExtendedStart.x -= freespacearound*proc.stepy;
  sliceVerticalBoundingBoxExtendedStart.y -= freespacearound*stepz;
  sliceVerticalBoundingBoxExtendedEnd.x += freespacearound*proc.stepy;
  sliceVerticalBoundingBoxExtendedEnd.y += freespacearound*stepz;
  if (label != 0) {
    outlinerhighprecisionreal incr = (outlinertitlespacey*stepz) / proc.multiplier;
    infof("increasing cross-section image vertical size by %.2f (%u*%.2f)/%.2f to accommodate label",
          incr, outlinertitlespacey, stepz, proc.multiplier);
    sliceVerticalBoundingBoxExtendedEnd.y += incr;
    if ((sliceVerticalBoundingBoxExtendedEnd.x - sliceVerticalBoundingBoxExtendedStart.x)/proc.stepy < outlinertitlespacex) {
      outlinerhighprecisionreal incr2 = (outlinertitlespacex*proc.stepy) / proc.multiplier;
      infof("increasing cross-section image horizontal size by %.2f (%u*%.2f)/%.2f to accommodate label",
            incr2, outlinertitlespacex, proc.stepy, proc.multiplier);
      sliceVerticalBoundingBoxExtendedEnd.x = sliceVerticalBoundingBoxExtendedStart.x + incr2;
    }
  }
  infof("  cross section bounding box after adjustment (%.2f,%.2f) to (%.2f,%.2f)",
        sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
        sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y);
  
  // Create a material matrix
  matrix = new MaterialMatrix(sliceVerticalBoundingBoxStart,
                              sliceVerticalBoundingBoxEnd,
                              proc.stepy,
                              stepz);
  infof("  slice bounding box after matrix creation (%.2f,%.2f) to (%.2f,%.2f) and steps %.2f and %.2f",
         sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
         sliceVerticalBoundingBoxStart.x + proc.stepy * matrix->xIndexSize,
         sliceVerticalBoundingBoxStart.y + stepz * matrix->yIndexSize,
         proc.stepy, stepz);
  infof("  extended slice bounding box after matrix creation (%.2f,%.2f) to (%.2f,%.2f) and steps %.2f and %.2f",
        sliceVerticalBoundingBoxExtendedStart.x, sliceVerticalBoundingBoxExtendedStart.y,
        sliceVerticalBoundingBoxExtendedEnd.x, sliceVerticalBoundingBoxExtendedEnd.y,
        proc.stepy, stepz);
  
  // Create an image base of the bounding size
  svg = proc.createSvg(fileName,sliceVerticalBoundingBoxExtendedStart,sliceVerticalBoundingBoxExtendedEnd,sliceDirection);

  debugf("  slice bounding box after svg creation (%.2f,%.2f) to (%.2f,%.2f) and steps %.2f and %.2f",
         sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
         sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y,
         proc.stepy, stepz);
  
  // Process the actual cross section
  drawCrossSection(scene);  
  debugf("  slice bounding box after drawing (%.2f,%.2f) to (%.2f,%.2f) and steps %.2f and %.2f",
         sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
         sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y,
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
           sliceVerticalBoundingBoxEnd.y, outlinertitlespaceempty, svg->getPixelYSize(),
           sliceVerticalBoundingBoxEnd.y + (outlinertitlespaceempty * svg->getPixelYSize()));
    svg->text(sliceVerticalBoundingBoxStart.x,
              sliceVerticalBoundingBoxEnd.y + (outlinertitlespaceempty * svg->getPixelYSize()),
              labelText);
  }
  
  // Now there's a matrix filled with a flag for each coordinate,
  // whether there was material or not. And small holes have been filled per above.
  // Draw the output based on all this.
  proc.matrixToSvg(matrix,svg,
                   sliceVerticalBoundingBoxStart.x,
                   sliceVerticalBoundingBoxStart.y,
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
  outlinerhighprecisionreal totalDifference = xDifference + yDifference;
  outlinerhighprecisionreal xDifferenceFraction = xDifference / totalDifference;
  outlinerhighprecisionreal yDifferenceFraction = yDifference / totalDifference;
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

outlinerhighprecisionreal
ProcessorCrossSection::calculateLineXBasedOnY(outlinerhighprecisionreal y) {
  
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
  outlinerhighprecisionreal n = (y - lineStart.y) / lineStepY;
  outlinerhighprecisionreal x = lineStart.x + n*lineStepX;
  return(x);
}

void
ProcessorCrossSection::getLineActualEndPoints(HighPrecisionVector2D& actualLineStart,
                                              HighPrecisionVector2D& actualLineEnd,
                                              outlinerhighprecisionreal extralineatends) {
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
  
  outlinerhighprecisionreal bottomY = sliceVerticalBoundingBoxStart.x - extralineatends;
  outlinerhighprecisionreal bottomX = calculateLineXBasedOnY(bottomY);
  
  outlinerhighprecisionreal topY = sliceVerticalBoundingBoxEnd.x + extralineatends;
  outlinerhighprecisionreal topX = calculateLineXBasedOnY(topY);

  actualLineStart.x = bottomX;
  actualLineStart.y = bottomY;
  
  actualLineEnd.x = topX;
  actualLineEnd.y = topY;
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
    if (!outlinerbetweenanyorder(sliceVerticalBoundingBoxStart.x,
                                 iter.point.y,
                                 sliceVerticalBoundingBoxEnd.x)) {
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
    proc.indexed.getFaces(mesh,iter.point.x,iter.point.y,&nFaces,&faces);
    if (nFaces > 0) {
      deepdebugf("  drawCrossSectionMesh: got %u cross section faces from (%.2f,%.2f)",
                 nFaces, iter.point.x, iter.point.y);
      for  (outlinerhighprecisionreal z = sliceVerticalBoundingBoxStart.y;
            z <= sliceVerticalBoundingBoxEnd.y;
            z += stepz) {
        deepdebugf("  drawCrossSectionMesh: z iterator step (%.2f,%.2f,%.2f)",
                   iter.point.x, iter.point.y, z);
        for (unsigned int f = 0; f < nFaces; f++) {
          drawCrossSectionFace(scene,mesh,faces[f],
                               firstStepInBoundingBox,
                               iter.step,
                               iter.point.x, iter.point.y, z);
        }
      }
    }
  }
}

void
ProcessorCrossSection::drawCrossSectionFace(const aiScene* scene,
                                            const aiMesh* mesh,
                                            const aiFace* face,
                                            unsigned int firstStepInBoundingBox,
                                            unsigned int currentStep,
                                            outlinerhighprecisionreal x,
                                            outlinerhighprecisionreal y,
                                            outlinerhighprecisionreal z) {
  assert(scene != 0);
  assert(mesh != 0);
  assert(face != 0);
  assert(matrix != 0);
  aiVector3D a3;
  aiVector3D b3;
  aiVector3D c3;
  proc.faceGetVertices3D(mesh,face,a3,b3,c3);
  HighPrecisionVector3D point(x,y,z);
  HighPrecisionVector3D stepboundingbox(x+lineStepX,y+lineStepY,z+stepz);
  char buf[80];
  OutlinerMath::triangleDescribe(a3,b3,c3,buf,sizeof(buf));
  deepdeepdebugf("describe done, result = %s", buf);
  if (OutlinerMath::boundingBoxIntersectsTriangle3D(a3,b3,c3,point,stepboundingbox)) {
    unsigned int xIndex = coordinateLineStepToImageXIndex(firstStepInBoundingBox,currentStep);
    unsigned int yIndex = coordinateZToImageYIndex(z);
    deepdebugf("    cross section draw face xyz match     at (%5.2f..%5.2f,%5.2f..%5.2f,%5.2f..%5.2f) steps %u (%u) triangle %s => matrix %u,%u",
               x, stepboundingbox.x, y, stepboundingbox.y, z, stepboundingbox.z,
               currentStep, firstStepInBoundingBox,
               buf,
               xIndex, yIndex);
    matrix->setMaterialMatrix(xIndex,yIndex);
  } else {
    deepdeepdebugf("      going to debug");
    deepdebugf("    cross section draw face xyz non-match at (%5.2f..%5.2f,%5.2f..%5.2f,%5.2f..%5.2f) "
               "face (%5.2f,%5.2f,%5.2f) x (%5.2f,%5.2f,%5.2f) x (%5.2f,%5.2f,%5.2f) triangle %s",
               x, stepboundingbox.x, y, stepboundingbox.y, z, stepboundingbox.z,
               a3.x, a3.y, a3.z,
               b3.x, b3.y, b3.z,
               c3.x, c3.y, c3.z,
               buf);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Cross section coordinates calculation //////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

unsigned int
ProcessorCrossSection::coordinateLineStepToImageXIndex(unsigned int firstStepInBoundingBox,
                                                       unsigned int currentStep) {
  assert(matrix != 0);
  unsigned int result = (currentStep - firstStepInBoundingBox);
  //outlinerhighprecisionreal thisXDifference = x - lineStart.x;
  //outlinerhighprecisionreal thisYDifference = y - lineStart.y;
  //outlinerhighprecisionreal thisLineLength = sqrt(thisXDifference*thisXDifference + thisYDifference*thisYDifference);
  //unsigned int result = (unsigned int)((thisLineLength * (matrix->xIndexSize + 1)) / lineLength);
  assert(result < matrix->xIndexSize);
  return(result);
}
 
unsigned int
ProcessorCrossSection::coordinateZToImageYIndex(outlinerhighprecisionreal z) {
  assert(matrix != 0);
  assert(z >= sliceVerticalBoundingBoxStart.y);
  assert(z <= sliceVerticalBoundingBoxEnd.y);
  outlinerhighprecisionreal diff =
    (z - sliceVerticalBoundingBoxStart.y) /
    (sliceVerticalBoundingBoxEnd.y - sliceVerticalBoundingBoxStart.y);
  unsigned int result = (unsigned int)(diff * (matrix->yIndexSize-1));
  deepdeepdebugf("      cross section coordinate z %.2f (diff %.4f) to %u (yIndexSize %u)",
                 z, diff, result, matrix->yIndexSize);
  assert(result < matrix->yIndexSize);
  return(result);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Cross section bounding box calculation /////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
ProcessorCrossSection::sliceVerticalBoundingBox(const aiScene* scene,
                                                HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                                HighPrecisionVector2D& sliceVerticalBoundingBoxEnd) {
  assert(scene != 0);
  deepdebugf("process cross section bounding box");
  bool set = 0;
  sliceVerticalBoundingBoxNode(scene,scene->mRootNode,
                               set,
                               sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
}

void
ProcessorCrossSection::sliceVerticalBoundingBoxNode(const aiScene* scene,
                                                    const aiNode* node,
                                                    bool& set,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxEnd) {
  assert(scene != 0);
  assert(node != 0);
  debugf("process node cross section");
  if (!node->mTransformation.IsIdentity()) {
    errf("Cannot handle transformations yet");
    exit(1);
  }
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    sliceVerticalBoundingBoxMesh(scene,scene->mMeshes[node->mMeshes[j]],
                                 set,
                                 sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    sliceVerticalBoundingBoxNode(scene,node->mChildren[i],
                                 set,
                                 sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
  }
}

void
ProcessorCrossSection::sliceVerticalBoundingBoxMesh(const aiScene* scene,
                                                    const aiMesh* mesh,
                                                    bool& set,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxEnd) {
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
        sliceVerticalBoundingBoxFace(scene,mesh,faces[f],
                                     iter.point.x, iter.point.y,
                                     set,
                                     sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd);
      }
    }
  }
}

void
ProcessorCrossSection::sliceVerticalBoundingBoxFace(const aiScene* scene,
                                                    const aiMesh* mesh,
                                                    const aiFace* face,
                                                    outlinerhighprecisionreal x,
                                                    outlinerhighprecisionreal y,
                                                    bool& set,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                                    HighPrecisionVector2D& sliceVerticalBoundingBoxEnd) {
  aiVector3D a3;
  aiVector3D b3;
  aiVector3D c3;
  proc.faceGetVertices3D(mesh,face,a3,b3,c3);
  aiVector2D a(DirectionOperations::outputx(proc.direction,a3),DirectionOperations::outputy(proc.direction,a3));
  aiVector2D b(DirectionOperations::outputx(proc.direction,b3),DirectionOperations::outputy(proc.direction,b3));
  aiVector2D c(DirectionOperations::outputx(proc.direction,c3),DirectionOperations::outputy(proc.direction,c3));
  HighPrecisionVector2D point(x,y);
  HighPrecisionVector2D stepboundingbox(x+lineStepX,y+lineStepY);
  if (OutlinerMath::boundingBoxIntersectsTriangle2D(a,b,c,point,stepboundingbox)) {
    deepdeepdebugf("cross section face (%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f) hits step bounding box (%.2f,%.2f)-(%.2f,%.2f)",
                   mesh->mVertices[face->mIndices[0]].x,mesh->mVertices[face->mIndices[0]].y,mesh->mVertices[face->mIndices[0]].z,
                   mesh->mVertices[face->mIndices[1]].x,mesh->mVertices[face->mIndices[1]].y,mesh->mVertices[face->mIndices[1]].z,
                   mesh->mVertices[face->mIndices[2]].x,mesh->mVertices[face->mIndices[2]].y,mesh->mVertices[face->mIndices[2]].z,
                   point.x, point.y,
                   stepboundingbox.x,
                   stepboundingbox.y);
    deepdeepdebugf("cross section direction %s and %s",
                   DirectionOperations::toString(proc.direction),
                   DirectionOperations::toString(sliceDirection));
    aiVector2D aSlice(DirectionOperations::outputy(proc.direction,a3),DirectionOperations::outputz(proc.direction,a3));
    aiVector2D bSlice(DirectionOperations::outputy(proc.direction,b3),DirectionOperations::outputz(proc.direction,b3));
    aiVector2D cSlice(DirectionOperations::outputy(proc.direction,c3),DirectionOperations::outputz(proc.direction,c3));
    HighPrecisionVector2D faceBoundingBoxStart;
    HighPrecisionVector2D faceBoundingBoxEnd;
    OutlinerMath::triangleBoundingBox2D(aSlice,bSlice,cSlice,
                                        faceBoundingBoxStart,
                                        faceBoundingBoxEnd);
    deepdebugf("    adding a face to cross section bounding box, triangle box (%.2f,%.2f) - (%.2f,%.2f)",
               faceBoundingBoxStart.x, faceBoundingBoxStart.y,
               faceBoundingBoxEnd.x, faceBoundingBoxEnd.y);
    //HighPrecisionVector2D resultBoundingBoxStart;
    //HighPrecisionVector2D resultBoundingBoxEnd;
    //OutlinerMath::boundingBoxIntersection(point,
    //                                      stepboundingbox,
    //                                      faceBoundingBoxStart,
    //                                      faceBoundingBoxEnd,
    //                                      resultBoundingBoxStart,
    //                                      resultBoundingBoxEnd);
    if (!set) {
      sliceVerticalBoundingBoxStart = faceBoundingBoxStart;
      sliceVerticalBoundingBoxEnd = faceBoundingBoxEnd;
      deepdebugf("setting initial cross section bounding box at (%.2f,%.2f) "
                 "to (%.2f,%.2f)-(%.2f,%.2f)",
                 x, y,
                 sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
                 sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y);
      set = 1;
    } else {
      HighPrecisionVector2D newBoundingBoxStart;
      HighPrecisionVector2D newBoundingBoxEnd;
      OutlinerMath::boundingBoxUnion(faceBoundingBoxStart,
                                     faceBoundingBoxEnd,
                                     sliceVerticalBoundingBoxStart,
                                     sliceVerticalBoundingBoxEnd,
                                     newBoundingBoxStart,
                                     newBoundingBoxEnd);
      deepdebugf("testing new cross section bounding box "
                 "for face (%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f)-(%.2f,%.2f,%.2f) "
                 "at (%.2f,%.2f) as (%.2f,%.2f)-(%.2f,%.2f)",
                 mesh->mVertices[face->mIndices[0]].x,mesh->mVertices[face->mIndices[0]].y,mesh->mVertices[face->mIndices[0]].z,
                 mesh->mVertices[face->mIndices[1]].x,mesh->mVertices[face->mIndices[1]].y,mesh->mVertices[face->mIndices[1]].z,
                 mesh->mVertices[face->mIndices[2]].x,mesh->mVertices[face->mIndices[2]].y,mesh->mVertices[face->mIndices[2]].z,
                 x, y,
                 newBoundingBoxStart.x, newBoundingBoxStart.y,
                 newBoundingBoxEnd.x, newBoundingBoxEnd.y);
      if (!OutlinerMath::boundingBoxEqual(sliceVerticalBoundingBoxStart,sliceVerticalBoundingBoxEnd,
                                          newBoundingBoxStart,newBoundingBoxEnd)) {
        sliceVerticalBoundingBoxStart = newBoundingBoxStart;
        sliceVerticalBoundingBoxEnd = newBoundingBoxEnd;
        deepdebugf("setting new cross section bounding box to (%.2f,%.2f)-(%.2f,%.2f)",
                   sliceVerticalBoundingBoxStart.x, sliceVerticalBoundingBoxStart.y,
                   sliceVerticalBoundingBoxEnd.x, sliceVerticalBoundingBoxEnd.y);
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
                   a.x, a.y, b.x, b.y, c.x, c.y,
                   point.x, point.y,
                   stepboundingbox.x,
                   stepboundingbox.y);
  }
}

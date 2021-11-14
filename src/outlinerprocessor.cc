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
// Model processing -- Constructors ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

Processor::Processor(const char* fileNameIn,
                     const unsigned int multiplierIn,
                     const bool smoothIn,
                     const bool mergedLinesIn,
                     const float linewidthIn,
                     const OutlinerBox3D& originalBoundingBoxIn,
                     const OutlinerBox3D& boundingBoxIn,
                     const outlinerreal stepxIn,
                     const outlinerreal stepyIn,
                     const outlinerreal stepzIn,
                     const enum outlinerdirection directionIn,
                     const enum outlineralgorithm algorithmIn,
                     const unsigned int holethresholdIn,
                     const unsigned int lineHolethresholdIn,
                     const bool labelsIn,
                     const bool formAnalysisIn,
                     const outlinerreal formCondenseIn,
                     const bool dimensionsIn,
                     unsigned int nCrossSectionsIn,
                     struct ProcessorCrossSectionInfo* crossSectionsIn,
                     IndexedMesh& indexedIn) :
  fileName(fileNameIn),
  multiplier(multiplierIn),
  smooth(smoothIn),
  mergedLines(mergedLinesIn),
  linewidth(linewidthIn),
  svg(0),
  originalBoundingBox(originalBoundingBoxIn),
  boundingBox(boundingBoxIn),
  stepx(stepxIn),
  stepy(stepyIn),
  stepz(stepzIn),
  direction(directionIn),
  algorithm(algorithmIn),
  holethreshold(holethresholdIn),
  lineHolethreshold(lineHolethresholdIn),
  labels(labelsIn),
  formAnalysis(formAnalysisIn),
  formCondense(formCondenseIn),
  stepxCondensed(stepx*formCondense),
  stepyCondensed(stepy*formCondense),
  stepzCondensed(stepz*formCondense),
  dimensions(dimensionsIn),
  originalPlanviewBoundingBox(DirectionOperations::outputx(direction,originalBoundingBox.start),
                              DirectionOperations::outputy(direction,originalBoundingBox.start),
                              DirectionOperations::outputx(direction,originalBoundingBox.end),
                              DirectionOperations::outputy(direction,originalBoundingBox.end)),
  planviewBoundingBox(DirectionOperations::outputx(direction,boundingBox.start),
                      DirectionOperations::outputy(direction,boundingBox.start),
                      DirectionOperations::outputx(direction,boundingBox.end),
                      DirectionOperations::outputy(direction,boundingBox.end)),
  matrix2(planviewBoundingBox,
          stepx,
          stepy),
  matrix3(boundingBox,
          stepxCondensed,
          stepyCondensed,
          stepzCondensed),
  forms(matrix2.xIndexSize,
        matrix2.yIndexSize),
  nCrossSections(nCrossSectionsIn),
  crossSections(crossSectionsIn),
  indexed(indexedIn) {
  debugf("algorithm %u=%u", algorithm, algorithmIn);
  if (holethreshold > outlinermaxholethreshold) {
    errf("Cannot compute hole thresholds larger than %u (%u given)", outlinermaxholethreshold, holethreshold);
  }
  if (lineHolethreshold > outlinermaxlineholethreshold) {
    errf("Cannot compute line hole thresholds larger than %u (%u given)", outlinermaxlineholethreshold, lineHolethreshold);
  }
  boundingBox2D = planviewBoundingBox;
  OutlinerBox2D boundingBox2DExtended(boundingBox2D);

  // Add spacce for labels
  if (labels) {
    addSpaceForLabels(boundingBox2DExtended,
                      hasNonHorizontalCrossSections(),
                      hasHorizontalCrossSections(),
                      stepx,
                      stepy);
  }
  
  // Add space for the line and text underneath
  if (dimensions) {
    addSpaceForDimensions(originalPlanviewBoundingBox,
                          boundingBox2DExtended,
                          dimensionBottomLabelingSpaceStartY,
                          dimensionRightLabelingSpaceStartX,
                          stepx,
                          stepy);
  }
  
  // Create the actual SVG object (but contents to be added later)
  svg = createSvg(fileName,boundingBox2DExtended,stepx,stepy,direction);
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

///////////////////////////////////////////////////////////////////////////////////////////////
// Model processing -- main program ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
Processor::processScene(const aiScene* scene) {
  
  debugf("processScene");
  assert(scene != 0);

  // Construct a plan-view matrix
  if (!preprocessSceneAlgorithmDraw(scene)) {
    return(0);
  }
  
  // Figure out if we need to analyse cave forms
  if (formAnalysis) {
    performFormAnalysis(scene);
  }
  
  // Draw the basic plan view
  if (!processSceneAlgorithmDraw(scene)) {
    return(0);
  }
  
  // Add dimension lines, if any
  if (dimensions) {
    addDimensionLines(svg,
                      originalPlanviewBoundingBox,
                      dimensionBottomLabelingSpaceStartY,
                      dimensionRightLabelingSpaceStartX,
                      stepx,
                      stepy);
  }

  // Process cross sections
  if (!processSceneCrossSections(scene,nCrossSections,crossSections)) {
    return(0);
  }
  
  // Main result (plan view) is also done, flush the image output
  svgDone();
  
  // Done, all good
  return(1);
}

bool
Processor::performFormAnalysis(const aiScene* scene) {
  infof("Slicing cave in %u slices...", matrix3.xIndexSize - 2);
  if (!performFormAnalysisSlicing(scene)) {
    return(0);
  }
  infof("Performing form analysis...");
  if (!performFormAnalysisAnalyze()) {
    return(0);
  }
  return(1);
}

bool
Processor::preprocessSceneAlgorithmDraw(const aiScene* scene) {
  if (algorithm != alg_triangle) {
    
    // First, go through each part of the picture, and determine if
    // there's material in it. Construct a matrix representing the
    // results.
    if (!sceneToMaterialMatrix(scene)) {
      return(0);
    }
    
    // Now there's a matrix filled with a flag for each coordinate,
    // whether there was material or not. Determine if we want to fill
    // small imperfections, removing small holes.
    if (holethreshold > 0) {
      unsigned int holeMinSize;
      unsigned int holeMaxSize;
      unsigned int holes = objectHoleRemoval(holeMinSize,holeMaxSize);
      if (holes > 0) {
        infof("  removed %u holes of size %u..%u pixels", holes, holeMinSize, holeMaxSize);
      }
    }
    
    // Similarly, see if we can fill holes in lines
    if (lineHolethreshold > 0) {
      unsigned int holeMinSize;
      unsigned int holeMaxSize;
      unsigned int holes = lineHoleRemoval(holeMinSize,holeMaxSize);
      if (holes > 0) {
        infof("  removed %u line holes of size %u..%u pixels", holes, holeMinSize, holeMaxSize);
      }
    }
    
  }

  // Done
  return(1);
}

bool
Processor::processSceneAlgorithmDraw(const aiScene* scene) {
  // Decide what to do, based on the algorithm specified
  switch (algorithm) {

  case alg_pixel:
  case alg_pixelform:
  case alg_borderpixel:
  case alg_borderline:
  case alg_borderactual:
    
    // Now there's a matrix filled with a flag for each coordinate,
    // whether there was material or not. And small holes have been filled per above.
    // Draw the output based on all this.
    if (!matrixToSvg(&matrix2,svg,algorithm,
                     DirectionOperations::outputx(direction,boundingBox.start),
                     DirectionOperations::outputy(direction,boundingBox.start),
                     stepx,
                     stepy)) {
      return(0);
    }
    break;

  case alg_triangle:

    if (!sceneToTrianglesSvg(scene,svg,
                             DirectionOperations::outputx(direction,boundingBox.start),
                             DirectionOperations::outputy(direction,boundingBox.start),
                             stepx,
                             stepy)) {
      return(0);
    }
    break;
    
  default:
    errf("Invalid algorithm %u", algorithm);
    return(0);
      
  }

  // Ok
  return(1);
}

bool
Processor::sceneToMaterialMatrix(const aiScene* scene) {
  
  // First, go through each part of the picture, and determine if
  // there's material in it. Construct a matrix representing the
  // results.
  unsigned int xIndex = 0;
  
  infof("computing material matrix...");
  for (outlinerreal x = DirectionOperations::outputx(direction,boundingBox.start);
       outlinerleepsilon(x,DirectionOperations::outputx(direction,boundingBox.end));
       x += stepx) {
    
    debugf("processor main loop x = %.2f (%u)", x, xIndex);
    assert(outlinergeepsilon(x,DirectionOperations::outputx(direction,boundingBox.start)));
    assert(outlinerleepsilon(x,DirectionOperations::outputx(direction,boundingBox.end)));
    unsigned int yIndex = 0;
    if (xIndex >= matrix2.xIndexSize) {
      debugf("processScene %u/%u", xIndex, matrix2.xIndexSize);
    }
    assert(xIndex < matrix2.xIndexSize);
    
    for (outlinerreal y = DirectionOperations::outputy(direction,boundingBox.start);
         outlinerleepsilon(y,DirectionOperations::outputy(direction,boundingBox.end));
         y += stepy) {
      
      assert(outlinergeepsilon(x,DirectionOperations::outputx(direction,boundingBox.start)));
      assert(outlinerleepsilon(x,DirectionOperations::outputx(direction,boundingBox.end)));
      assert(outlinergeepsilon(y,DirectionOperations::outputy(direction,boundingBox.start)));
      assert(outlinerleepsilon(y,DirectionOperations::outputy(direction,boundingBox.end)));
      if (yIndex >= matrix2.yIndexSize) {
        debugf("processScene %u,%u/%u,%u", xIndex, yIndex, matrix2.xIndexSize, matrix2.yIndexSize);
      }
      assert(yIndex < matrix2.yIndexSize);
      if (x > -0.605 && x < -0.595) {
        debugf("checking (%.2f,%.2f) or (%u,%u)",x,y,xIndex,yIndex);
      } else {
        deepdebugf("checking (%.2f,%.2f)",x,y);
      }
      if (sceneHasMaterial(scene,indexed,x,y)) {
        debugf("  material at (%.2f,%.2f) ie. %u,%u",x,y,xIndex,yIndex);
        matrix2.setMaterialMatrix(xIndex,yIndex);
      }
      
      yIndex++;
        
    }
      
    xIndex++;
      
  }
  
  return(1);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Form analysis //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
Processor::performFormAnalysisSlicing(const aiScene* scene) {
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    performFormAnalysisOneSlice(scene,xIndex);
  }
  unsigned int memory;
  unsigned int theoretical;
  outlinerreal percentage = matrix3.filledPercentage(memory,theoretical);
  infof("3D slice matrix has %u set pixels (uses %.2f MB, %.2f%% filled from theoretical %.2f MB)",
        matrix3.count(),
        memory / (1024.0 * 1024.0),
        percentage,
        theoretical / (1024.0 * 1024.0));
  return(1);
}

bool
Processor::performFormAnalysisOneSlice(const aiScene* scene,
                                       unsigned int xIndex) {
  unsigned int yIndexFrom;
  unsigned int yIndexTo;
  outlinerreal x = planviewBoundingBox.start.x + xIndex * stepxCondensed;
  unsigned int xIndexMatrix2 = matrix2.coordinateXToIndex(x);
  if (matrix2.getMaterialYBounds(xIndexMatrix2,yIndexFrom,yIndexTo)) {
    outlinerreal yFrom = matrix2.indexToCoordinateY(yIndexFrom);
    outlinerreal yTo = matrix2.indexToCoordinateY(yIndexTo);
    OutlinerLine2D sliceLine(x,yFrom,x,yTo);
    infof("  slice %u: x = %.2f, y = %.2f..%.2f (%u..%u, in a matrix of %ux%u)",
          xIndex, x,
          yFrom, yTo,
          yIndexFrom,
          yIndexTo,
          matrix2.xIndexSize, matrix2.yIndexSize);
    ProcessorCrossSection csproc(0, // no image
                                 0, // no labels
                                 DirectionOperations::screenx(direction),
                                 sliceLine,
                                 stepxCondensed,
                                 stepyCondensed,
                                 stepzCondensed,
                                 1.0,
                                 *this);
    csproc.processSceneCrossSection(scene);
    OutlinerBox2D verticalBoundingBox;
    csproc.getCrossSectionBoundingBox(verticalBoundingBox);
    MaterialMatrix2D* verticalMatrix = 0;
    csproc.getVerticalMatrix(verticalMatrix);
    assert(verticalMatrix != 0);
    matrix3.setMaterialMatrixSlice(xIndex,verticalBoundingBox,verticalMatrix);
    infof("  slice size %u x %u", verticalMatrix->xIndexSize, verticalMatrix->yIndexSize);
  }
  return(1);
}

bool
Processor::performFormAnalysisAnalyze(void) {
  infof("  form analysis matrix2 %u x %u matrix3 %u x %u",
        matrix2.xIndexSize, matrix2.yIndexSize,
        matrix3.xIndexSize, matrix3.yIndexSize);
  char buf[50];
  OutlinerMath::boxDescribe(matrix2.boundingBox,buf,sizeof(buf),1);
  infof("  form matrix2 box %s", buf);
  OutlinerMath::boxDescribe(matrix3.boundingBox,buf,sizeof(buf),1);
  infof("  form matrix3 box %s", buf);
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    outlinerreal x = matrix3.indexToCoordinateX(xIndex);
    unsigned int matrix2xIndexStart = matrix2.coordinateXToIndex(x);
    unsigned int matrix2xIndexEnd = matrix2xIndexStart + (unsigned int)ceil(formCondense);
    for (unsigned int yIndex = 0; yIndex < matrix3.yIndexSize - 2; yIndex++) {
      outlinerreal y = matrix3.indexToCoordinateY(yIndex);
      infof("    analyze %u,%u: %.2f,%.2f", xIndex, yIndex, x, y);
      unsigned int matrix2yIndexStart = matrix2.coordinateYToIndex(y);
      unsigned int matrix2yIndexEnd = matrix2yIndexStart + (unsigned int)ceil(formCondense);
      performFormAnalysisAnalyzeOnePixel(xIndex,yIndex,
                                         matrix2xIndexStart,matrix2yIndexStart,
                                         matrix2xIndexEnd,matrix2yIndexEnd);
    }
  }
  return(1);
}

bool
Processor::performFormAnalysisAnalyzeOnePixel(const unsigned int matrix3xIndex,
                                              const unsigned int matrix3yIndex,
                                              const unsigned int matrix2xIndexStart,
                                              const unsigned int matrix2yIndexStart,
                                              const unsigned int matrix2xIndexEnd,
                                              const unsigned int matrix2yIndexEnd) {
  if (!matrix2.getMaterialMatrix(matrix2xIndexStart,
                                 matrix2yIndexStart,
                                 matrix2xIndexEnd,
                                 matrix2yIndexEnd)) {
    forms.setForm(matrix3xIndex,matrix3yIndex,outlinerform_mainform_empty);
  } else {
    unsigned int materialLayers = formAnalysisCountLayers(matrix3xIndex,matrix3yIndex);
    infof("  layers = %u", materialLayers);
    switch (materialLayers) {
    case 0:
      forms.setForm(matrix3xIndex,matrix3yIndex,outlinerform_mainform_empty);
      break;
    case 1:
      forms.setForm(matrix3xIndex,matrix3yIndex,outlinerform_mainform_degenerate);
      break;
    case 2:
      forms.setForm(matrix3xIndex,matrix3yIndex,outlinerform_mainform_tunnel);
      break;
    default:
      forms.setForm(matrix3xIndex,matrix3yIndex,outlinerform_mainform_complex);
      break;
    }
  }
  return(1);
}

unsigned int
Processor::formAnalysisCountLayers(const unsigned int matrix3xIndex,
                                   const unsigned int matrix3yIndex) const {
  unsigned int count = 0;
  bool layerOn = 0;
  infof("  count layers %u,%u", matrix3xIndex, matrix3yIndex);
  for (unsigned int matrix3zIndex = 0; matrix3zIndex < matrix3.zIndexSize -2; matrix3zIndex++) {
    infof("  iter count %u,%u,%u", matrix3xIndex, matrix3yIndex, matrix3zIndex);
    bool material = matrix3.getMaterialMatrix(matrix3xIndex,matrix3yIndex,matrix3zIndex);
    if (layerOn && material) continue;
    else if (!layerOn && !material) continue;
    else if (!layerOn && material) { layerOn = 1; count++; continue; }
    else if (layerOn && !material) { layerOn = 0; continue; }
  }
  infof("  count done %u", count);
  return(count);
}

OutlinerSvgStyle
Processor::formToColor(const unsigned int xIndex,
                       const unsigned int yIndex) const {
  outlinerform form = forms.getForm(xIndex,yIndex);
  uint8_t mainform = (form & outlinerform_mainform);
  deepdebugf("    mainform = %u", mainform);
  switch (mainform) {
  case outlinerform_mainform_empty:
    return(outlinersvgstyle_grey);
  case outlinerform_mainform_tunnel:
    return(outlinersvgstyle_none);
  case outlinerform_mainform_tunnel_stalac:
    return(outlinersvgstyle_red);
  case outlinerform_mainform_degenerate:
    return(outlinersvgstyle_blue);
  case outlinerform_mainform_complex:
    return(outlinersvgstyle_green);
  default:
    errf("Invalid form %x", form);
    exit(1);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Model processing -- labels /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
Processor::addSpaceForLabels(OutlinerBox2D& pictureBoundingBox,
                             bool vertical,
                             bool horizontal,
                             const outlinerreal thisStepX,
                             const outlinerreal thisStepY) {
  if (vertical) {
    pictureBoundingBox.start.y -= (outlinercrosssectionextraline * thisStepY);
    pictureBoundingBox.end.y += (outlinercrosssectionextraline * thisStepY);
    pictureBoundingBox.end.y += (outlinertitlespaceempty * thisStepY);
    pictureBoundingBox.end.y += ((outlinerdefaultfontysize * thisStepY) / multiplier);
  }
  if (horizontal) {
    pictureBoundingBox.start.x -= (outlinercrosssectionextraline * thisStepX);
    pictureBoundingBox.end.x += (outlinercrosssectionextraline * thisStepX);
    pictureBoundingBox.end.x += (outlinertitlespaceempty * thisStepX);
    pictureBoundingBox.end.x += ((2 * outlinerdefaultfontysizelarge * thisStepX) / multiplier);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Model processing -- dimension lines ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
Processor::addSpaceForDimensions(const OutlinerBox2D& objectBoundingBox,
                                 OutlinerBox2D& pictureBoundingBox,
                                 outlinerreal& bottomDimensionLabelingStartY,
                                 outlinerreal& rightDimensionLabelingStartX,
                                 const outlinerreal thisStepX,
                                 const outlinerreal thisStepY) {

  // Add space underneath to fit in the length line and text
  bottomDimensionLabelingStartY =  pictureBoundingBox.start.y;
  rightDimensionLabelingStartX = pictureBoundingBox.end.x;
  pictureBoundingBox.start.y -= (outlinerdimensionlinespace * thisStepY);
  pictureBoundingBox.start.y -= (outlinersmallfontysize * thisStepY) / multiplier;
  
  // Is the picture in general big enough to take the string length
  // for the label underneath?
  outlinerreal dimensionHorizontalSpaceNeeded = outlinerdimensionspacex / multiplier;
  outlinerreal horizontalSpaceAvailable = (objectBoundingBox.end.x - objectBoundingBox.start.x)/thisStepX;
  if (horizontalSpaceAvailable < dimensionHorizontalSpaceNeeded) {
    outlinerreal incr = (dimensionHorizontalSpaceNeeded - horizontalSpaceAvailable) * thisStepX + 2*thisStepX;
    infof("increasing image horizontal size by %.2f to accommodate dimension text",
          incr);
    pictureBoundingBox.start.x = outlinermin(pictureBoundingBox.start.x,objectBoundingBox.start.x - incr/2);
    pictureBoundingBox.end.x = outlinermax(pictureBoundingBox.end.x,objectBoundingBox.end.x + incr/2);
  }

  // Add space to the right side to fit the length line and text
  pictureBoundingBox.end.x += (outlinerdimensionlinespace * thisStepX);
  pictureBoundingBox.end.x += (outlinersmallfontysize * thisStepX) / multiplier;
  
  // Is the picture in general big enough to take the string length
  // for the label on the right side?
  outlinerreal dimensionVerticalSpaceNeeded = outlinerdimensionspacex / multiplier;
  outlinerreal verticalSpaceAvailable = (objectBoundingBox.end.y - objectBoundingBox.start.y)/thisStepY;
  if (verticalSpaceAvailable < dimensionVerticalSpaceNeeded) {
    outlinerreal incr = (dimensionVerticalSpaceNeeded - verticalSpaceAvailable) * thisStepY + 2*thisStepY;
    infof("increasing image vertical size by %.2f to accommodate dimension text",
          incr);
    pictureBoundingBox.start.y = outlinermin(pictureBoundingBox.start.y,objectBoundingBox.start.y - incr/2);
    pictureBoundingBox.end.y = outlinermax(pictureBoundingBox.end.y,objectBoundingBox.end.y + incr/2);
  }
}

void
Processor::addDimensionLines(SvgCreator* theSvg,
                             const OutlinerBox2D& objectBoundingBox,
                             outlinerreal bottomDimensionLabelingStartY,
                             outlinerreal rightDimensionLabelingStartX,
                             const outlinerreal thisStepX,
                             const outlinerreal thisStepY) {

  // Bottom line
  outlinerreal bottomLineStart =  bottomDimensionLabelingStartY - outlinerdimensionlinespacelinestart * thisStepY;
  theSvg->line(objectBoundingBox.start.x,
               bottomLineStart,
               objectBoundingBox.end.x + thisStepX,
               bottomLineStart,
               outlinersvgstyle_ends);

  // Bottom text
  outlinerreal bottomTextStartX =  (objectBoundingBox.end.x + objectBoundingBox.start.x) / 2.0;
  outlinerreal bottomTextStartY =
    bottomDimensionLabelingStartY -
    outlinerdimensionlinespace * thisStepY -
    (outlinersmallfontysize * thisStepY) / multiplier;
  outlinerreal diffX = objectBoundingBox.end.x - objectBoundingBox.start.x;
  char bufX[20];
  memset(bufX,0,sizeof(bufX));
  snprintf(bufX,sizeof(bufX)-1,"%.2fm",diffX);
  bottomTextStartX -= (((strlen(bufX) / 2.0) * outlinersmallfontxsize) * thisStepX) / multiplier;
  infof("addDimensionLines bottom text %s at %.2f %.2f", bufX, bottomTextStartX, bottomTextStartY);
  theSvg->text(bottomTextStartX,bottomTextStartY,bufX,outlinersmallfont);
  
  // Right side line
  outlinerreal rightLineStart =  rightDimensionLabelingStartX + outlinerdimensionlinespacelinestart * thisStepX;
  theSvg->line(rightLineStart,
               objectBoundingBox.start.y - thisStepY,
               rightLineStart,
               objectBoundingBox.end.y,
               outlinersvgstyle_ends);
  
  // Right side text
  outlinerreal rightTextStartX =  
    rightDimensionLabelingStartX +
    outlinerdimensionlinespace * thisStepX;
  outlinerreal rightTextStartY = (objectBoundingBox.end.y + objectBoundingBox.start.y) / 2.0;
  infof("right text y %.2f from %.2f+%.2f/2",
        rightTextStartY,
        objectBoundingBox.end.y, objectBoundingBox.start.y);
  outlinerreal diffY = objectBoundingBox.end.y - objectBoundingBox.start.y;
  char bufY[20];
  memset(bufY,0,sizeof(bufY));
  snprintf(bufY,sizeof(bufY)-1,"%.2fm",diffY);
  infof("addDimensionLines right text now at %.2f %.2f", rightTextStartX, rightTextStartY);
  rightTextStartY += (((strlen(bufY) / 2.0) * outlinersmallfontxsize) * thisStepY) / multiplier;
  infof("addDimensionLines right text %s at %.2f %.2f (string %u fontxsize %.2f thisstepy %.2f)",
        bufY, rightTextStartX, rightTextStartY,
        strlen(bufY), outlinersmallfontxsize, thisStepY);
  theSvg->text(rightTextStartX,rightTextStartY,bufY,outlinersmallfont,90);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Model proecssing -- 3D model conversion to SVGs of triangles ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
Processor::sceneToTrianglesSvg(const aiScene* scene,
                               SvgCreator* theSvg,
                               outlinerreal xStart,
                               outlinerreal yStart,
                               outlinerreal xStep,
                               outlinerreal yStep) {
  
  // Go through each part of the picture, and draw a triangle to the
  // output image for each of them.
  
  infof("drawing triangles...");
  OutlinerBox2D trianglesBoundingBox(DirectionOperations::outputx(direction,boundingBox.start),
                                     DirectionOperations::outputy(direction,boundingBox.start),
                                     DirectionOperations::outputx(direction,boundingBox.end),
                                     DirectionOperations::outputy(direction,boundingBox.end));
  nodeToTrianglesSvg(scene,scene->mRootNode,theSvg,indexed,trianglesBoundingBox);
      
  return(1);
}

void
Processor::nodeToTrianglesSvg(const aiScene* scene,
                              const aiNode* node,
                              SvgCreator* theSvg,
                              IndexedMesh& indexed,
                              OutlinerBox2D& trianglesBoundingBox) {
  assert(scene != 0);
  assert(node != 0);
  if (!node->mTransformation.IsIdentity()) {
    errf("Cannot handle transformations yet");
    exit(1);
  }
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    meshToTrianglesSvg(scene,scene->mMeshes[node->mMeshes[j]],theSvg,indexed,trianglesBoundingBox);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    nodeToTrianglesSvg(scene,node->mChildren[i],theSvg,indexed,trianglesBoundingBox);
  }
}

void
Processor::meshToTrianglesSvg(const aiScene* scene,
                              const aiMesh* mesh,
                              SvgCreator* theSvg,
                              IndexedMesh& indexed,
                              OutlinerBox2D& trianglesBoundingBox) {
  assert(scene != 0);
  assert(mesh != 0);
  for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
    faceToTrianglesSvg(scene,mesh,&mesh->mFaces[f],theSvg,trianglesBoundingBox);
  }
}

void
Processor::faceToTrianglesSvg(const aiScene* scene,
                              const aiMesh* mesh,
                              const aiFace* face,
                              SvgCreator* theSvg,
                              OutlinerBox2D& trianglesBoundingBox) {
  assert(scene != 0);
  assert(mesh != 0);
  OutlinerTriangle2D t;
  faceGetVertices2D(mesh,face,direction,t);
  if (OutlinerMath::boundingBoxIntersectsTriangle2D(t,trianglesBoundingBox)) {
    theSvg->triangle(t);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Drawing an SVG based on a material matrix //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
Processor::matrixToSvg(MaterialMatrix2D* theMatrix,
                       SvgCreator* theSvg,
                       enum outlineralgorithm theAlgorithm,
                       outlinerreal xStart,
                       outlinerreal yStart,
                       outlinerreal xStep,
                       outlinerreal yStep) {
  infof("constructing output core of %ux%u pixels...",
        theMatrix->xIndexSize, theMatrix->yIndexSize);
  deepdebugf("algorithm %u", theAlgorithm);
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
        debugf("algorithm %u", theAlgorithm);
        switch (theAlgorithm) {
        case alg_pixel:
          debugf("pixel alg %u,%u from %.2f,%.2f", xIndex, yIndex, x, y);
          theSvg->pixel(x,y);
          break;
        case alg_pixelform:
          debugf("pixelform alg %u,%u from %.2f,%.2f", xIndex, yIndex, x, y);
          theSvg->pixel(x,y,formToColor(xIndex,yIndex));
          break;
        case alg_triangle:
          errf("Invalid algorithm for matrix-based operation");
          exit(1);
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
                  outlinerreal otherX = matrix2.indexToCoordinateX(borderTableX[b]);
                  outlinerreal otherY = matrix2.indexToCoordinateY(borderTableY[b]);
                  deepdeepdebugf("calling theSvg->line");
                  theSvg->line(otherX,otherY,x,y);
                }
              }
            }
          }
          break;
        case alg_borderactual:
          errf("Borderactual algorithm is not yet implemented");
          return(0);
        default:
          errf("Invalid algorithm %u", theAlgorithm);
          return(0);
        }
      }
    }
  }
  
  return(1);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Model processing -- inspecting 3D elements /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

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
  OutlinerVector2D stepBoundingBox(x+stepx-2*outlinerepsilon,y+stepy-2*outlinerepsilon);
  OutlinerBox2D thisBox(point,stepBoundingBox);
  if (OutlinerMath::boundingBoxIntersectsTriangle2D(t,thisBox)) {
    char buf[150];
    OutlinerMath::triangleDescribe(t,buf,sizeof(buf));
    debugf("    found out that (%.2f..%.2f,%.2f..%.2f) is hitting a face %s",
           thisBox.start.x,thisBox.end.x,thisBox.start.y,thisBox.end.y,buf);
    return(1);
  }
  return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Model processing -- Neighbor pixel operations //////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

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
  if (xIndex < matrix2.xIndexSize-1)                                   { tableX[n] = xIndex+1; tableY[n] = yIndex;   n++; }
  
  // Top and bottom neighbours
  if (yIndex > 0)                                                     { tableX[n] = xIndex;   tableY[n] = yIndex-1; n++; }
  if (yIndex < matrix2.yIndexSize-1)                                   { tableX[n] = xIndex;   tableY[n] = yIndex+1; n++; }
  
  // Left side corner neighbours
  if (xIndex > 0 && yIndex > 0)                                       { tableX[n] = xIndex-1; tableY[n] = yIndex-1; n++; }
  if (xIndex > 0 && yIndex < matrix2.yIndexSize-1)                     { tableX[n] = xIndex-1; tableY[n] = yIndex+1; n++; }

  // Right side corner neighbours
  if (xIndex < matrix2.xIndexSize-1 && yIndex > 0)                     { tableX[n] = xIndex+1; tableY[n] = yIndex-1; n++; }
  if (xIndex < matrix2.xIndexSize-1 && yIndex < matrix2.yIndexSize-1)   { tableX[n] = xIndex+1; tableY[n] = yIndex+1; n++; }
  
  // Done
  assert(n <= tableSize);
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
                    MaterialMatrix2D* theMatrix,
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

///////////////////////////////////////////////////////////////////////////////////////////////
// SVG Object Creation ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

SvgCreator*
Processor::createSvg(const char* svgFileName,
                     const OutlinerBox2D& svgBoundingBox,
                     outlinerreal svgStepX,
                     outlinerreal svgStepY,
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
                          svgStepX,svgStepY,
                          svgDirection,
                          xOutputStart,xOutputEnd,
                          yOutputStart,yOutputEnd,
                          xSize,ySize,
                          xSizeInt,ySizeInt,
                          xFactor,yFactor);
  infof("SVG %s size x %.2f..%.2f step %.2f xsize %.2f xsizeint %u",
        svgFileName,
        xOutputStart, xOutputEnd, svgStepX, xSize, xSizeInt);
  infof("  size y %.2f..%.2f step %.2f ysize %.2f ysizeint %u",
        yOutputStart, yOutputEnd, svgStepY, ySize, ySizeInt);
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
                                   const outlinerreal svgStepX,
                                   const outlinerreal svgStepY,
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
  xSize = (xOutputEnd - xOutputStart) / svgStepX;
  ySize = (yOutputEnd - yOutputStart) / svgStepY;
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
                                     const unsigned int nCrossSections,
                                     const struct ProcessorCrossSectionInfo* crossSections) {
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
                                    const unsigned int c,
                                    const struct ProcessorCrossSectionInfo* crossSection) {
  infof("cross section %u at (%.2f,%.2f)-(%.2f,%.2f) to file %s",
        c,
        crossSection->line.start.x,
        crossSection->line.start.y,
        crossSection->line.end.x,
        crossSection->line.end.y,
        crossSection->filename);
  assert(crossSection->filename != 0);
  assert(crossSection->width >= 0.0);
  ProcessorCrossSection csproc(crossSection->filename,
                               crossSection->label,
                               DirectionOperations::screenx(direction),
                               crossSection->line,
                               stepx,
                               stepy,
                               stepz,
                               crossSection->width,
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
  debugf("addCrossSectionLine (%.2f,%.2f) to (%.2f,%.2f)",
         actualLine.start.x,actualLine.start.y,
         actualLine.end.x,actualLine.end.y);
  svg->line(actualLine.start.x,actualLine.start.y,
            actualLine.end.x,actualLine.end.y,
            outlinersvgstyle_dashed);
  infof("    process cross-section line text %.2f - font %.2f * 0.5 * pixelXSize %.2f = %.2f",
        actualLine.end.x, outlinerdefaultfontxsizelarge, svg->getPixelXSize(),
        actualLine.end.x - outlinerdefaultfontxsizelarge * 0.5 * svg->getPixelXSize());
  if (actualLine.horizontal()) {
    svg->text(actualLine.end.x + outlinerdefaultfontxsize * 0.2 * svg->getPixelXSize(),
              actualLine.end.y - outlinerdefaultfontysizelarge * 0.3 * svg->getPixelYSize(),
              label);
  } else {
    svg->text(actualLine.end.x - outlinerdefaultfontxsizelarge * 0.5 * svg->getPixelXSize(),
              actualLine.end.y + outlinerdefaultfontysize * 0.1 * svg->getPixelYSize(),
              label);
  }
}

bool
Processor::hasNonHorizontalCrossSections(void) {
  for (unsigned int i = 0; i < nCrossSections; i++) {
    if (!crossSections[i].line.horizontal()) {
      return(1);
    }
  }
  return(0);
}

bool
Processor::hasHorizontalCrossSections(void) {
  for (unsigned int i = 0; i < nCrossSections; i++) {
    if (crossSections[i].line.horizontal()) {
      return(1);
    }
  }
  return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// Hole removal algorithms ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

unsigned int
Processor::objectHoleRemoval(unsigned int& holeMinSize,
                             unsigned int& holeMaxSize) {
  unsigned int holes = 0;
  holeMinSize = 9999;
  holeMaxSize = 0;
  if (holethreshold > 0) {
    infof("filtering holes...");
     for (unsigned int xIndex = 1; xIndex < matrix2.xIndexSize; xIndex++) {
      for (unsigned int yIndex = 0; yIndex < matrix2.yIndexSize; yIndex++) {
        if (!matrix2.getMaterialMatrix(xIndex-1,yIndex)) continue;
        if (!matrix2.getMaterialMatrix(xIndex,yIndex)) {
          unsigned int n;
          unsigned int holeXtable[outlinermaxholethreshold];
          unsigned int holeYtable[outlinermaxholethreshold];
          if (objectHoleIsEqualOrSmallerThan(xIndex,yIndex,holethreshold,n,outlinermaxholethreshold,holeXtable,holeYtable)) {
            debugf("  correcting a hole of %u pixels at (%u,%u)", n, xIndex, yIndex);
            holes++;
            if (holeMinSize > n) holeMinSize = n;
            if (holeMaxSize < n) holeMaxSize = n;
            for (unsigned int i = 0; i < n; i++) {
              matrix2.setMaterialMatrix(holeXtable[i],holeYtable[i]);
            }
          }
        }
      }
    }
  }
  
  return(holes);
}

unsigned int
Processor::lineHoleRemoval(unsigned int& holeMinSize,
                           unsigned int& holeMaxSize) {
  return(0);
}

bool
Processor::objectHoleIsEqualOrSmallerThan(unsigned int xIndex,
                                          unsigned int yIndex,
                                          unsigned int holethreshold,
                                          unsigned int& n,
                                          unsigned int tableSize,
                                          unsigned int* holeXtable,
                                          unsigned int* holeYtable) {
  assert(tableSize <= outlinermaxholethreshold);
  assert(holethreshold <= outlinermaxholethreshold);
  assert(!matrix2.getMaterialMatrix(xIndex,yIndex));
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
    if (matrix2.getMaterialMatrix(xIndex,yIndex)) continue;

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


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
#include "outlinerprocessorforms.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class Processor;

///////////////////////////////////////////////////////////////////////////////////////////////
// Form analysis -- Construction //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

ProcessorForms::ProcessorForms(const OutlinerBox3D& boundingBoxIn,
                               const enum outlinerdirection directionIn,
                               const OutlinerBox2D& planviewBoundingBoxIn,
                               const outlinerreal stepxIn,
                               const outlinerreal stepyIn,
                               const outlinerreal stepzIn,
                               const outlinerreal formCondenseIn,
                               const MaterialMatrix2D& matrix2In,
                               class Processor& procIn) :
  boundingBox(boundingBoxIn),
  direction(directionIn),
  planviewBoundingBox(planviewBoundingBoxIn),
  stepx(stepxIn),
  stepy(stepyIn),
  stepz(stepzIn),
  formCondense(formCondenseIn),
  stepxCondensed(stepx*formCondense),
  stepyCondensed(stepy*formCondense),
  stepzCondensed(stepz*formCondense),
  matrix2(matrix2In),
  matrix3(boundingBox,
          stepxCondensed,
          stepyCondensed,
          stepzCondensed),
  forms(matrix2.xIndexSize,
        matrix2.yIndexSize),
  proc(procIn) {
}

ProcessorForms::~ProcessorForms() {
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Form analysis -- External interface ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
ProcessorForms::performFormAnalysis(const aiScene* scene) {
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

OutlinerSvgStyle
ProcessorForms::formToColor(const unsigned int xIndex,
                            const unsigned int yIndex) const {
  assert(forms.xIndexSize == matrix2.xIndexSize);
  assert(forms.yIndexSize == matrix2.yIndexSize);
  outlinerform form = forms.getForm(xIndex,yIndex);
  uint8_t mainform = (form & outlinerform_mainform);
  deepdebugf("    mainform = %u", mainform);
  switch (mainform) {
  case outlinerform_mainform_empty:
    return(outlinersvgstyle_greyval(128));
  case outlinerform_mainform_tunnel:
    return(outlinersvgstyle_none);
  case outlinerform_mainform_tunnel_stalac:
    return(outlinersvgstyle_red);
  case outlinerform_mainform_degenerate:
    return(outlinersvgstyle_blue);
  case outlinerform_mainform_dripline:
    return(outlinersvgstyle_yellow);
  case outlinerform_mainform_complex:
    return(outlinersvgstyle_green);
  default:
    errf("Invalid form %x", form);
    exit(1);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Form analysis -- Internal functions ////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
ProcessorForms::performFormAnalysisSlicing(const aiScene* scene) {
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
ProcessorForms::performFormAnalysisOneSlice(const aiScene* scene,
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
                                 proc);
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
ProcessorForms::performFormAnalysisAnalyze(void) {
  infof("  form analysis matrix2 %u x %u matrix3 %u x %u",
        matrix2.xIndexSize, matrix2.yIndexSize,
        matrix3.xIndexSize, matrix3.yIndexSize);
  char buf[50];
  OutlinerMath::boxDescribe(matrix2.boundingBox,buf,sizeof(buf),1);
  infof("  form matrix2 box %s", buf);
  OutlinerMath::boxDescribe(matrix3.boundingBox,buf,sizeof(buf),1);
  infof("  form matrix3 box %s", buf);

  // Phase 1
  infof("Form analysis phase 1...");
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    outlinerreal x = matrix3.indexToCoordinateX(xIndex);
    unsigned int matrix2xIndexStart = matrix2.coordinateXToIndex(x);
    unsigned int matrix2xIndexEnd = matrix2xIndexStart + ((unsigned int)ceil(formCondense)) - 1;
    for (unsigned int yIndex = 0; yIndex < matrix3.yIndexSize - 2; yIndex++) {
      outlinerreal y = matrix3.indexToCoordinateY(yIndex);
      unsigned int matrix2yIndexStart = matrix2.coordinateYToIndex(y);
      unsigned int matrix2yIndexEnd = matrix2yIndexStart + ((unsigned int)ceil(formCondense)) - 1;
      infof("    analyze phase 1 %u,%u: %.2f,%.2f (matrix2 %u,%u .. %u,%u)",
            xIndex, yIndex, x, y,
            matrix2xIndexStart, matrix2yIndexStart,
            matrix2xIndexEnd, matrix2yIndexEnd);
      assert(matrix2xIndexStart <= matrix2xIndexEnd);
      assert(matrix2yIndexStart <= matrix2yIndexEnd);
      performFormAnalysisAnalyzeOnePixelPhase1(xIndex,yIndex,
                                               matrix2xIndexStart,matrix2yIndexStart,
                                               matrix2xIndexEnd,matrix2yIndexEnd);
    }
  }

  // Phase2
  infof("Form analysis phase 2...");
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    outlinerreal x = matrix3.indexToCoordinateX(xIndex);
    unsigned int matrix2xIndexStart = matrix2.coordinateXToIndex(x);
    unsigned int matrix2xIndexEnd = matrix2xIndexStart + ((unsigned int)ceil(formCondense)) - 1;
    for (unsigned int yIndex = 0; yIndex < matrix3.yIndexSize - 2; yIndex++) {
      outlinerreal y = matrix3.indexToCoordinateY(yIndex);
      unsigned int matrix2yIndexStart = matrix2.coordinateYToIndex(y);
      unsigned int matrix2yIndexEnd = matrix2yIndexStart + ((unsigned int)ceil(formCondense)) - 1;
      infof("    analyze phase 2 %u,%u: %.2f,%.2f (matrix2 %u,%u .. %u,%u)",
            xIndex, yIndex, x, y,
            matrix2xIndexStart, matrix2yIndexStart,
            matrix2xIndexEnd, matrix2yIndexEnd);
      assert(matrix2xIndexStart <= matrix2xIndexEnd);
      assert(matrix2yIndexStart <= matrix2yIndexEnd);
      performFormAnalysisAnalyzeOnePixelPhase2(xIndex,yIndex,
                                               matrix2xIndexStart,matrix2yIndexStart,
                                               matrix2xIndexEnd,matrix2yIndexEnd);
    }
  }

  // Done
  return(1);
}

bool
ProcessorForms::performFormAnalysisAnalyzeOnePixelPhase1(const unsigned int matrix3xIndex,
                                                         const unsigned int matrix3yIndex,
                                                         const unsigned int matrix2xIndexStart,
                                                         const unsigned int matrix2yIndexStart,
                                                         const unsigned int matrix2xIndexEnd,
                                                         const unsigned int matrix2yIndexEnd) {
  outlinerform form = outlinerform_mainform_empty;

  // Find out if there's any material.
  if (!matrix2.getMaterialMatrix(matrix2xIndexStart,
                                 matrix2yIndexStart,
                                 matrix2xIndexEnd,
                                 matrix2yIndexEnd)) {

    //  If not, just the form as no material (empty).
    infof("    no material");
    form = outlinerform_mainform_empty;
    
  } else {

    // Otherwise, make a decision based on how many layers of material
    // there are
    unsigned int materialLayers = formAnalysisCountLayers(matrix3xIndex,matrix3yIndex);
    infof("   layers = %u", materialLayers);
    switch (materialLayers) {
    case 0:
      form = outlinerform_mainform_empty;
      break;
    case 1:
      form = outlinerform_mainform_degenerate;
      break;
    case 2:
      form = outlinerform_mainform_tunnel;
      break;
    default:
      form = outlinerform_mainform_complex;
      break;
    }
  }

  // Found out what form we should use. Update the matrix.
  forms.setForm(matrix2xIndexStart,
                matrix2yIndexStart,
                matrix2xIndexEnd,
                matrix2yIndexEnd,
                form);
  return(1);
}

bool
ProcessorForms::performFormAnalysisAnalyzeOnePixelPhase2(const unsigned int matrix3xIndex,
                                                         const unsigned int matrix3yIndex,
                                                         const unsigned int matrix2xIndexStart,
                                                         const unsigned int matrix2yIndexStart,
                                                         const unsigned int matrix2xIndexEnd,
                                                         const unsigned int matrix2yIndexEnd) {
  // If there's no material or only one layer, check to see if we are
  // potentially at an entrance. An entrance is defined as a place
  // where the model stops, i.e., there is no material for a sideways
  // or from the top entry, and you can get underneath a form space
  // that is marked as a proper tunnel.

  unsigned int matrix2xStep = matrix2xIndexEnd - matrix2xIndexStart + 1;
  unsigned int matrix2yStep = matrix2yIndexEnd - matrix2yIndexStart + 1;
  outlinerform form = forms.getForm(matrix2xIndexStart,matrix2yIndexStart);
  
  switch (form) {
  case outlinerform_mainform_empty:
  case outlinerform_mainform_degenerate:
    if (entranceAnalysis(matrix3xIndex,matrix3yIndex, 1, 0, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep) ||
        entranceAnalysis(matrix3xIndex,matrix3yIndex, 0, 1, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep) ||
        entranceAnalysis(matrix3xIndex,matrix3yIndex,-1, 0, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep) ||
        entranceAnalysis(matrix3xIndex,matrix3yIndex, 0,-1, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep)) {
      
      infof("found an entrance hit at %u,%u", matrix3xIndex,matrix3yIndex);
      forms.setForm(matrix2xIndexStart,
                    matrix2yIndexStart,
                    matrix2xIndexEnd,
                    matrix2yIndexEnd,
                    outlinerform_mainform_dripline);
      
    }
    break;
  default:
    break;
  }

  // Done
  return(1);
}

bool
ProcessorForms::entranceAnalysis(const unsigned int matrix3xIndex,
                                 const unsigned int matrix3yIndex,
                                 const int xDirection,
                                 const int yDirection,
                                 const unsigned int matrix2xIndexStart,
                                 const unsigned int matrix2xStep,
                                 const unsigned int matrix2yIndexStart,
                                 const unsigned int matrix2yStep) const {
  
  const outlinerreal minimumTunnelPath = 0.5;
  const outlinerreal minimumTunnelHeight = 0.4;
  outlinerreal pathSoFar = 0.0;
  outlinerreal pathHeightSoFar = 0.0;
  unsigned int matrix3zIndexSearch = matrix3.zIndexSize - 2;
  unsigned int matrix3zIndexSearchStart = matrix3zIndexSearch;
  
  do {

    unsigned int matrix3xIndexSearch = matrix3xIndex;
    unsigned int matrix3yIndexSearch = matrix3yIndex;
    unsigned int matrix2xIndex = matrix2xIndexStart;
    unsigned int matrix2yIndex = matrix2yIndexStart;

    // Check if our starting point has material at the given z
    // height. If so, this isn't an entrance.
    bool materialAtPointIn3D = matrix3.getMaterialMatrix(matrix3xIndex,matrix3yIndex,matrix3zIndexSearch);
    if (materialAtPointIn3D && matrix3zIndexSearch > 0) {
      matrix3zIndexSearch--;
      continue;
    }
    
    do {
      
      // Did we reach an border in the image? If so, bail out and
      // conclude the result is inconclusive :-)
      if (matrix3zIndexSearch == 0) return(0);
      if (xDirection < 0 && matrix3xIndexSearch == 0) return(0);
      if (yDirection < 0 && matrix3yIndexSearch == 0) return(0);
      if (xDirection > 0 && matrix3xIndexSearch >= matrix3.xIndexSize - 2) return(0);
      if (yDirection > 0 && matrix3yIndexSearch >= matrix3.yIndexSize - 2) return(0);
      
      // Otherwise, advance the search
      matrix3xIndexSearch += xDirection;
      matrix3yIndexSearch += yDirection;
      matrix3zIndexSearch--;
      matrix2xIndex += xDirection * matrix2xStep;
      matrix2yIndex += yDirection * matrix2yStep;
      
      // And then see if we can find the right type of material at
      // this xy-position and empty space at level z in the
      // xyz-matrix.
      outlinerform neighborForm = forms.getForm(matrix2xIndex,matrix2yIndex);
      bool materialIn3D = matrix3.getMaterialMatrix(matrix3xIndexSearch,matrix3yIndexSearch,matrix3zIndexSearch);
      if ((neighborForm != outlinerform_mainform_tunnel &&
           neighborForm != outlinerform_mainform_tunnel_stalac) ||
          materialIn3D) {
        pathSoFar = pathHeightSoFar = 0.0;
        matrix3xIndexSearch = matrix3xIndex;
        matrix3yIndexSearch = matrix3yIndex;
        matrix2xIndex = matrix2xIndexStart;
        matrix2yIndex = matrix2yIndexStart;
        matrix3zIndexSearchStart = matrix3zIndexSearch - 1;
        continue;
      }

      // So far so good, lets increase the length we have searched to
      // see if this is enough
      if (xDirection != 0) pathSoFar += stepxCondensed;
      else if (yDirection != 0) pathSoFar += stepyCondensed;
      pathHeightSoFar += stepzCondensed;

      // See if we can declare a win
      if (pathSoFar >= minimumTunnelPath && pathHeightSoFar >= minimumTunnelHeight) {
        infof("Found entrance at %u,%u => %u,%u (level %u from %u, path length %.2f height %.2f)",
              matrix3xIndex,matrix3yIndex,
              matrix3xIndexSearch,matrix3yIndexSearch,
              matrix3zIndexSearch,matrix3zIndexSearch,
              pathSoFar, pathHeightSoFar);
        return(1);
      }
      
    } while (1);
    
  } while (matrix3zIndexSearch > 0);
  
  // Nothing found
  return(0);
}

unsigned int
ProcessorForms::formAnalysisCountLayers(const unsigned int matrix3xIndex,
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



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
ProcessorForms::performFormAnalysisAnalyzeOnePixel(const unsigned int matrix3xIndex,
                                                   const unsigned int matrix3yIndex,
                                                   const unsigned int matrix2xIndexStart,
                                                   const unsigned int matrix2yIndexStart,
                                                   const unsigned int matrix2xIndexEnd,
                                                   const unsigned int matrix2yIndexEnd) {
  if (!matrix2.getMaterialMatrix(matrix2xIndexStart,
                                 matrix2yIndexStart,
                                 matrix2xIndexEnd,
                                 matrix2yIndexEnd)) {
    forms.setForm(matrix2xIndexStart,matrix2yIndexStart,
                  matrix2xIndexEnd,matrix2yIndexEnd,
                  outlinerform_mainform_empty);
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



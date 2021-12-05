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
                               const unsigned int formCondenseIn,
                               MaterialMatrix2D& matrix2In,
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
  proc(procIn),
  nClearedMaterial(0) {
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

outlinerform
ProcessorForms::getForm(const unsigned int xIndex,
                        const unsigned int yIndex) const {
  assert(forms.xIndexSize == matrix2.xIndexSize);
  assert(forms.yIndexSize == matrix2.yIndexSize);
  outlinerform form = forms.getForm(xIndex,yIndex);
  return(form);
}

bool
ProcessorForms::formIsEntrance(const unsigned int xIndex,
                               const unsigned int yIndex) const {
  assert(forms.xIndexSize == matrix2.xIndexSize);
  assert(forms.yIndexSize == matrix2.yIndexSize);
  assert(xIndex < forms.xIndexSize);
  assert(yIndex < forms.yIndexSize);
  if (forms.getForm(xIndex,yIndex) == outlinerform_mainform_dripline) return(1);
  else if (checkFormNearby(isEntrance,xIndex,yIndex,formCondense,1)) return(1);
  else return(0);
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
    debugf("  slice %u: x = %.2f, y = %.2f..%.2f (%u..%u, in a matrix of %ux%u)",
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
    debugf("  slice size %u x %u", verticalMatrix->xIndexSize, verticalMatrix->yIndexSize);
  }
  return(1);
}

void
ProcessorForms::condensedXIndexToIndex(const unsigned int matrix3xIndex,
                                       unsigned int& matrix2xIndexStart,
                                       unsigned int& matrix2xIndexEnd) const {
  assert(matrix3xIndex < matrix3.xIndexSize);
  outlinerreal x = matrix3.indexToCoordinateX(matrix3xIndex);
  matrix2xIndexStart = matrix2.coordinateXToIndex(x);
  matrix2xIndexEnd = matrix2xIndexStart + formCondense - 1;
  assert(matrix2xIndexStart <= matrix2xIndexEnd);
}

void
ProcessorForms::condensedYIndexToIndex(const unsigned int matrix3yIndex,
                                       unsigned int& matrix2yIndexStart,
                                       unsigned int& matrix2yIndexEnd) const {
  assert(matrix3yIndex < matrix3.yIndexSize);
  outlinerreal y = matrix3.indexToCoordinateY(matrix3yIndex);
  matrix2yIndexStart = matrix2.coordinateYToIndex(y);
  matrix2yIndexEnd = matrix2yIndexStart + formCondense - 1;
  assert(matrix2yIndexStart <= matrix2yIndexEnd);
}

bool
ProcessorForms::performFormAnalysisAnalyze(void) {
  infof("  form analysis matrix2 %u x %u matrix3 %u x %u",
        matrix2.xIndexSize, matrix2.yIndexSize,
        matrix3.xIndexSize, matrix3.yIndexSize);
  char buf[50];
  OutlinerMath::boxDescribe(matrix2.boundingBox,buf,sizeof(buf),1);
  debugf("  form matrix2 box %s", buf);
  OutlinerMath::boxDescribe(matrix3.boundingBox,buf,sizeof(buf),1);
  debugf("  form matrix3 box %s", buf);

  // Phase 1
  infof("Form analysis phase 1...");
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    unsigned int matrix2xIndexStart;
    unsigned int matrix2xIndexEnd;
    condensedXIndexToIndex(xIndex,matrix2xIndexStart,matrix2xIndexEnd);
    for (unsigned int yIndex = 0; yIndex < matrix3.yIndexSize - 2; yIndex++) {
      unsigned int matrix2yIndexStart;
      unsigned int matrix2yIndexEnd;
      condensedYIndexToIndex(yIndex,matrix2yIndexStart,matrix2yIndexEnd);
      debugf("    analyze phase 1 %u,%u: (matrix2 %u,%u .. %u,%u)",
             xIndex, yIndex, 
             matrix2xIndexStart, matrix2yIndexStart,
             matrix2xIndexEnd, matrix2yIndexEnd);
      assert(matrix2xIndexStart <= matrix2xIndexEnd);
      assert(matrix2yIndexStart <= matrix2yIndexEnd);
      performFormAnalysisAnalyzeOnePixelPhase1(xIndex,yIndex,
                                               matrix2xIndexStart,matrix2yIndexStart,
                                               matrix2xIndexEnd,matrix2yIndexEnd);
    }
  }

  // Debugs
  debugf("Form matrix after phase 1 (all)");
  forms.print();
  debugf("Form matrix after phase 1 (condensed)");
  forms.print(formCondense);
  for (unsigned int i = 0; i < 150 && i < matrix3.xIndexSize - 2; i++) {
    debugf("Slice %u:", i);
    char buf[120];
    memset(buf,0,sizeof(buf));
    for (unsigned int c = 0; c < matrix3.yIndexSize - 2 && c < 80; c++) {
      snprintf(buf+strlen(buf),sizeof(buf)-1-strlen(buf),"%u", c%10);
    }
    debugf(buf);
    memset(buf,0,sizeof(buf));
    unsigned int matrix2xIndexStart;
    unsigned int matrix2xIndexEnd;
    condensedXIndexToIndex(i,matrix2xIndexStart,matrix2xIndexEnd);
    for (unsigned int f = 0; f < matrix3.yIndexSize -2 && f < 80; f++) {
      unsigned int matrix2yIndexStart;
      unsigned int matrix2yIndexEnd;
      condensedYIndexToIndex(f,matrix2yIndexStart,matrix2yIndexEnd);
      outlinerform form = forms.getForm(matrix2xIndexStart,matrix2yIndexStart);
      char representativeChar = forms.getFormChar(form);
      snprintf(buf+strlen(buf),sizeof(buf)-1-strlen(buf),"%c", representativeChar);
    }
    debugf(buf);
    for (unsigned int zIndex = matrix3.zIndexSize - 2; zIndex >= 0 && zIndex < matrix3.zIndexSize; zIndex--) {
      memset(buf,0,sizeof(buf));
      for (unsigned int yIndex = 0; yIndex < matrix3.yIndexSize && yIndex < 80; yIndex++) {
        if (matrix3.getMaterialMatrix(i,yIndex,zIndex)) {
          strncat(buf,"X",sizeof(buf)-1);
        } else {
          strncat(buf," ",sizeof(buf)-1);
        }
      }
      snprintf(buf+strlen(buf),sizeof(buf)-1-strlen(buf)," %2u", zIndex);
      debugf(buf);
    }
    debugf("");
  }
  
  // Phase 2
  infof("Form analysis phase 2...");
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    outlinerreal x = matrix3.indexToCoordinateX(xIndex);
    unsigned int matrix2xIndexStart = matrix2.coordinateXToIndex(x);
    unsigned int matrix2xIndexEnd = matrix2xIndexStart + formCondense - 1;
    for (unsigned int yIndex = 0; yIndex < matrix3.yIndexSize - 2; yIndex++) {
      outlinerreal y = matrix3.indexToCoordinateY(yIndex);
      unsigned int matrix2yIndexStart = matrix2.coordinateYToIndex(y);
      unsigned int matrix2yIndexEnd = matrix2yIndexStart + formCondense - 1;
      debugf("  analyze phase 2 %u,%u: %.2f,%.2f (matrix2 %u,%u .. %u,%u)",
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

  // Phase 3
  infof("Form analysis phase 3...");
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    outlinerreal x = matrix3.indexToCoordinateX(xIndex);
    unsigned int matrix2xIndexStart = matrix2.coordinateXToIndex(x);
    unsigned int matrix2xIndexEnd = matrix2xIndexStart + formCondense - 1;
    for (unsigned int yIndex = 0; yIndex < matrix3.yIndexSize - 2; yIndex++) {
      outlinerreal y = matrix3.indexToCoordinateY(yIndex);
      unsigned int matrix2yIndexStart = matrix2.coordinateYToIndex(y);
      unsigned int matrix2yIndexEnd = matrix2yIndexStart + formCondense - 1;
      debugf("  analyze phase 3 %u,%u: %.2f,%.2f (matrix2 %u,%u .. %u,%u)",
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
  
  infof("Form analysis phase 4...");
  if (nClearedMaterial > 0) {
    debugf("Clearing %u elements...", nClearedMaterial);
    clearMaterial(nClearedMaterial,
                  clearedMaterialX,
                  clearedMaterialY);
    nClearedMaterial = 0;
  }
  
  infof("Form analysis phase 5...");
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    outlinerreal x = matrix3.indexToCoordinateX(xIndex);
    unsigned int matrix2xIndexStart = matrix2.coordinateXToIndex(x);
    unsigned int matrix2xIndexEnd = matrix2xIndexStart + formCondense - 1;
    for (unsigned int yIndex = 0; yIndex < matrix3.yIndexSize - 2; yIndex++) {
      outlinerreal y = matrix3.indexToCoordinateY(yIndex);
      unsigned int matrix2yIndexStart = matrix2.coordinateYToIndex(y);
      unsigned int matrix2yIndexEnd = matrix2yIndexStart + formCondense - 1;
      infof("  analyze phase 5 %u,%u: %.2f,%.2f (matrix2 %u,%u .. %u,%u)",
            xIndex, yIndex, x, y,
            matrix2xIndexStart, matrix2yIndexStart,
            matrix2xIndexEnd, matrix2yIndexEnd);
      assert(matrix2xIndexStart <= matrix2xIndexEnd);
      assert(matrix2yIndexStart <= matrix2yIndexEnd);
      performFormAnalysisAnalyzeOnePixelPhase5(xIndex,yIndex,
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
  //
  // Are we near another entrance form? If yes, relax the limits
  //
  
  bool entranceNearby = checkFormNearby(isEntrance,
                                        matrix2xIndexStart,matrix2yIndexStart,
                                        formCondense,
                                        1);
  
  // If there's no material or only one layer, check to see if we are
  // potentially at an entrance. An entrance is defined as a place
  // where the model stops, i.e., there is no material for a sideways
  // or from the top entry, and you can get underneath a form space
  // that is marked as a proper tunnel.

  unsigned int matrix2xStep = matrix2xIndexEnd - matrix2xIndexStart + 1;
  unsigned int matrix2yStep = matrix2yIndexEnd - matrix2yIndexStart + 1;
  
  if (
      entranceAnalysis(matrix3xIndex,matrix3yIndex, 1, 1, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep,entranceNearby) ||
      entranceAnalysis(matrix3xIndex,matrix3yIndex, 1, 0, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep,entranceNearby) ||
      entranceAnalysis(matrix3xIndex,matrix3yIndex, 1,-1, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep,entranceNearby) ||
      entranceAnalysis(matrix3xIndex,matrix3yIndex, 0, 1, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep,entranceNearby) ||
      entranceAnalysis(matrix3xIndex,matrix3yIndex, 0,-1, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep,entranceNearby) ||
      entranceAnalysis(matrix3xIndex,matrix3yIndex,-1, 1, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep,entranceNearby) ||
      entranceAnalysis(matrix3xIndex,matrix3yIndex,-1, 0, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep,entranceNearby) ||
      entranceAnalysis(matrix3xIndex,matrix3yIndex,-1,-1, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep,entranceNearby)
      ) {
    
    infof("  found an entrance hit at %u,%u", matrix3xIndex,matrix3yIndex);
    forms.setForm(matrix2xIndexStart,
                  matrix2yIndexStart,
                  matrix2xIndexEnd,
                  matrix2yIndexEnd,
                  outlinerform_mainform_dripline);
    
  }

  // Done
  return(1);
}

bool
ProcessorForms::performFormAnalysisAnalyzeOnePixelPhase5(const unsigned int matrix3xIndex,
                                                         const unsigned int matrix3yIndex,
                                                         const unsigned int matrix2xIndexStart,
                                                         const unsigned int matrix2yIndexStart,
                                                         const unsigned int matrix2xIndexEnd,
                                                         const unsigned int matrix2yIndexEnd) {

  //
  // Do we have an entrance form here?
  //

  if (getForm(matrix2xIndexStart,matrix2yIndexStart) != outlinerform_mainform_dripline) return(1);
  
  //
  // We do. Are there other entrance forms nearby? 
  //
  
  if (checkFormNearby(isEntrance,
                      matrix2xIndexStart,matrix2yIndexStart,
                      formCondense,
                      1)) return(1);

  //
  // If not, remove this one too as spurious.
  //

  infof("Removing spurious entrance form marking...");
  forms.setForm(matrix2xIndexStart,
                matrix2yIndexStart,
                matrix2xIndexEnd,
                matrix2yIndexEnd,
                outlinerform_mainform_degenerate);
  
  //
  // Done
  //

  return(1);
}


#define dirstring(d)    (((d) == 0) ? "" : ((d) < 0 ? "--" : "++"))
#define debugreturn(w,v) { bool val = (v); debugf("%s: returning %u", w, val); return(val); }
#define debugreturn1(w,a,v) { bool val = (v); debugf(w ": returning %u", a, val); return(val); }
#define debugreturn2(w,a,b,v) { bool val = (v); debugf(w ": returning %u", a, b, val); return(val); }
#define debugreturn3(w,a,b,c,v) { bool val = (v); debugf(w ": returning %u", a, b, c, val); return(val); }
#define debugreturn4(w,a,b,c,d,v) { bool val = (v); debugf(w ": returning %u", a, b, c, d, val); return(val); }
#define debugreturn5(w,a,b,c,d,e,v) { bool val = (v); debugf(w ": returning %u", a, b, c, d, e, val); return(val); }

bool
ProcessorForms::isEmpty(outlinerform form) {
  return(form  == outlinerform_mainform_empty);
}

bool
ProcessorForms::isDegenerate(outlinerform form) {
  return(form  == outlinerform_mainform_degenerate);
}

bool
ProcessorForms::isEmptyOrDegenerate(outlinerform form) {
  switch (form) {
  case outlinerform_mainform_empty:
  case outlinerform_mainform_degenerate:
    return(1);
  default:
    return(0);
  }
}

bool
ProcessorForms::isTunnel(outlinerform form) {
  return(form == outlinerform_mainform_tunnel);
}

bool
ProcessorForms::isEntrance(outlinerform form) {
  return(form == outlinerform_mainform_dripline);
}

bool
ProcessorForms::checkForm(ProcessorFormChecker checkFunction,
                          const unsigned int matrix2xIndex,
                          const unsigned int matrix2yIndex) const {
  
  assert(matrix2xIndex < forms.xIndexSize);
  assert(matrix2yIndex < forms.yIndexSize);
  outlinerform form = forms.getForm(matrix2xIndex,matrix2yIndex);
  return((*checkFunction)(form));
}

bool
ProcessorForms::canIncreaseIndex(const unsigned int matrix2xIndex,
                                 const unsigned int matrix2yIndex,
                                 const int xDirection,
                                 const int yDirection) const {
  const unsigned int xAbs = outlinerabs(xDirection);
  const unsigned int yAbs = outlinerabs(yDirection);
  if (matrix2xIndex < xAbs && xDirection < 0) debugreturn("      can increase runs to matrix x start",0);
  if (matrix2yIndex < yAbs && yDirection < 0) debugreturn("      can increase runs to matrix y start",0);
  if (matrix2xIndex >= forms.xIndexSize - 1 - xAbs && xDirection > 0) debugreturn("      can increase runs to matrix x end",0);
  if (matrix2yIndex >= forms.yIndexSize - 1 - yAbs && yDirection > 0) debugreturn("      can increase runs to matrix y end",0);
  return(1);
}

bool
ProcessorForms::checkFormRange(ProcessorFormChecker checkFunction,
                               const unsigned int matrix2xIndex,
                               const unsigned int matrix2yIndex,
                               const int xDirection,
                               const int yDirection,
                               const unsigned int steps,
                               const bool okToRunToModelEnd) const {

  assert(matrix2xIndex < forms.xIndexSize);
  assert(matrix2yIndex < forms.yIndexSize);
  unsigned int matrix2xIndexIter = matrix2xIndex;
  unsigned int matrix2yIndexIter = matrix2yIndex;
  debugf("    check form range %u,%u %d,%d and %u steps",
        matrix2xIndex, matrix2yIndex,
        xDirection,
        yDirection,
        steps);
  for (unsigned int i = 0; i < steps; i++) {
    outlinerform form = forms.getForm(matrix2xIndexIter,matrix2yIndexIter);
    bool val = (*checkFunction)(form);
    if (!val) debugreturn5("    check range fails at %u,%u (%u,%u) for form %u",
                           matrix2xIndexIter,matrix2yIndexIter,
                           matrix2xIndexIter/formCondense,matrix2yIndexIter/formCondense,
                           form,
                           0);
    if (i+1 < steps) {
      if (canIncreaseIndex(matrix2xIndexIter,matrix2yIndexIter,xDirection,yDirection)) {
        matrix2xIndexIter += xDirection;
        matrix2yIndexIter += yDirection;
        debugf("      increased check form range to %u,%u", matrix2xIndexIter,matrix2yIndexIter);
      } else {
        debugreturn("    check range runs to matrix ends",okToRunToModelEnd);
      }
    }
  }
  debugreturn("    check range succeeds",1);
}

bool
ProcessorForms::checkFormNearby(ProcessorFormChecker checkFunction,
                                const unsigned int matrix2xIndex,
                                const unsigned int matrix2yIndex,
                                const unsigned int oneStep,
                                const unsigned int steps) const {

  assert(matrix2xIndex < forms.xIndexSize);
  assert(matrix2yIndex < forms.yIndexSize);
  assert(steps <= 1);
  unsigned int n = 0;
  const unsigned int maxTable = 20;
  unsigned int xTable[maxTable];
  unsigned int yTable[maxTable];

  //
  // Look up the neighbors
  //
  
  if (steps > 0)  {
    proc.getNeighbours(matrix2xIndex,
                       matrix2yIndex,
                       n,
                       maxTable,
                       xTable,
                       yTable,
                       oneStep,
                       forms.xIndexSize,
                       forms.yIndexSize);
  }

  //
  // Iterate over the neighbors, and check the form in each neighbor
  //
  
  for (unsigned int i = 0; i < n; i++) {
    debugf("      neighbor %u/%u: %u,%u", i, n, xTable[i],yTable[i]);
    outlinerform form = forms.getForm(xTable[i],yTable[i]);
    bool val = (*checkFunction)(form);
    if (val) debugreturn("    found neighbor with matching form",1);
  }
  
  //
  // Fail, no neighbor with match found
  //
  
  debugreturn("    did not find a neighbor with matching form",0);
}

void
ProcessorForms::clearMaterial(const unsigned int n,
                              unsigned int* tableX,
                              unsigned int* tableY) {
  for (unsigned int i = 0;  i < n; i++) {
    unsigned int xIndex = tableX[i];
    unsigned int yIndex = tableY[i];
    for (unsigned int xIndexExtra = xIndex; xIndexExtra < xIndex + formCondense; xIndexExtra++) {
      for (unsigned int yIndexExtra = yIndex; yIndexExtra < yIndex + formCondense; yIndexExtra++) {
        debugf("        clearing %u,%", xIndexExtra,yIndexExtra);
        forms.setForm(xIndexExtra,yIndexExtra,outlinerform_mainform_empty);
        matrix2.unsetMaterialMatrix(xIndexExtra,yIndexExtra);
      }
    }
  }
}

void
ProcessorForms::addToTable(const unsigned int x,
                           const unsigned int y,
                           unsigned int& n,
                           const unsigned int tableSize,
                           unsigned int* tableX,
                           unsigned int* tableY) {
  if (n == tableSize) {
    errf("Entrance blocking material table full (%u entries)", tableSize);
    exit(1);
  }
  tableX[n] = x;
  tableY[n] = y;
  n++;
}

bool
ProcessorForms::hasSideNeighbor(const unsigned int xIndex,
                                const unsigned int yIndex,
                                const int xDirection,
                                const int yDirection,
                                const unsigned int sideCounter, // 0..5
                                unsigned int& neighX,
                                unsigned int& neighY) const {
  assert(xDirection != 0 || yDirection != 0);
  assert(sideCounter < 6);
  int sideXDirection;
  int sideYDirection;
  if (xDirection != 0 && yDirection == 0) {
    if (sideCounter > 1) return(0);
    sideXDirection = 0;
    sideYDirection = sideCounter == 0 ? xDirection : -xDirection;
  } else if (xDirection == 0 && yDirection != 0) {
    if (sideCounter > 1) return(0);
    sideXDirection = sideCounter == 0 ? yDirection : -yDirection;
    sideYDirection = 0;
  } else {
    switch (sideCounter) {
    case 0:
      sideXDirection = -xDirection;
      sideYDirection = yDirection;
      break;
    case 1:
      sideXDirection = xDirection;
      sideYDirection = -yDirection;
      break;
    case 2:
      sideXDirection = -xDirection;
      sideYDirection = 0;
      break;
    case 3:
      sideXDirection = xDirection;
      sideYDirection = 0;
      break;
    case 4:
      sideXDirection = 0;
      sideYDirection = yDirection;
      break;
    case 5:
      sideXDirection = 0;
      sideYDirection = -yDirection;
      break;
    default:
      errf("invalid side counter");
      exit(1);
    }
  }
  
  //
  // Ok, now we have a direction, but is the model large enough for us
  // to go to that direction?
  //
  
  if (!canIncreaseIndex(xIndex,yIndex,sideXDirection,sideYDirection)) {
    return(0);
  } else {
    neighX = xIndex + sideXDirection;
    neighY = yIndex + sideYDirection;
    return(1);
  }
}

void
ProcessorForms::collectMaterialToClear(ProcessorFormChecker checkFunction,
                                       ProcessorFormChecker terminationCheckFunction,
                                       const unsigned int matrix2xIndex,
                                       const unsigned int matrix2yIndex,
                                       const int xDirection,
                                       const int yDirection,
                                       const bool neighbors,
                                       unsigned int& n,
                                       const unsigned int tableSize,
                                       unsigned int* tableX,
                                       unsigned int* tableY) {
  
  assert(matrix2xIndex < forms.xIndexSize);
  assert(matrix2yIndex < forms.yIndexSize);
  assert(tableSize > 0);
  assert(tableX != 0);
  assert(tableY != 0);

  unsigned int beginN = n;
  unsigned int matrix2xIndexIter = matrix2xIndex;
  unsigned int matrix2yIndexIter = matrix2yIndex;
  debugf("    clear material %u,%u %d,%d",
        matrix2xIndex, matrix2yIndex,
        xDirection,
        yDirection);
  for (;;) {

    //
    // Check if we still have the material that needs to be cleared
    //
    
    outlinerform form = forms.getForm(matrix2xIndexIter,matrix2yIndexIter);
    bool val = (*checkFunction)(form);
    if (!val) {
      debugf("    clear material ends %u,%u (%u,%u) for form %u",
            matrix2xIndexIter,matrix2yIndexIter,
            matrix2xIndexIter/formCondense,matrix2yIndexIter/formCondense,
            form);
      if ((*terminationCheckFunction)(form)) {
        debugf("    clearing colletion ended well, keeping %u", n - beginN);
      } else {
        debugf("    clearing colletion ended badly, not keeping %u", n - beginN);
        n = beginN;
      }
      return;
    }

    //
    // We do. Add to the list of material to clear.
    //

    addToTable(matrix2xIndexIter,matrix2yIndexIter,n,tableSize,tableX,tableY);
    
    //
    // Should we also add neighbouring material?
    //

    if (neighbors) {
      unsigned int neighX;
      unsigned int neighY;
      for (unsigned int side = 0; side < 6; side++) {
        if (hasSideNeighbor(matrix2xIndexIter,matrix2yIndexIter,xDirection,yDirection,side,neighX,neighY)) {
          if ((*checkFunction)(getForm(neighX,neighY))) addToTable(neighX,neighY,n,tableSize,tableX,tableY);
        }
      }
    }
    
    //
    // Can we continue, or are we hitting model borders?
    //
    
    if (canIncreaseIndex(matrix2xIndexIter,matrix2yIndexIter,xDirection,yDirection)) {
      matrix2xIndexIter += xDirection;
      matrix2yIndexIter += yDirection;
      debugf("      increased clear material to %u,%u", matrix2xIndexIter,matrix2yIndexIter);
    } else {
      debugf("    clear material runs to matrix ends");
      return;
    }
  }
}

static const outlinerreal minimumTunnelPathMeters = 0.5;
static const outlinerreal minimumTunnelHeightMeters = 0.3;
static const outlinerreal minimumTunnelPathMetersNearOtherEntrance = 0.3;
static const outlinerreal minimumTunnelHeightMetersNearOtherEntrance = 0.2;

bool
ProcessorForms::entranceAnalysis(const unsigned int matrix3xIndex,
                                 const unsigned int matrix3yIndex,
                                 const int xDirection,
                                 const int yDirection,
                                 const unsigned int matrix2xIndexStart,
                                 const unsigned int matrix2xStep,
                                 const unsigned int matrix2yIndexStart,
                                 const unsigned int matrix2yStep,
                                 const bool entranceNearby) {

  //
  // Debugs
  //
  
  debugf("  entrance analysis from %u%s,%u%s",
         matrix3xIndex, dirstring(xDirection),
         matrix3yIndex, dirstring(yDirection));

  //
  // Preliminary variables
  //
  
  outlinerreal directionStepCondensed;
  if (xDirection == 0.0) {
    directionStepCondensed = stepyCondensed;
  } else if (yDirection != 0.0 && xDirection != 0.0) {
    directionStepCondensed = (stepxCondensed+stepyCondensed)/2;
  } else {
    directionStepCondensed = stepxCondensed;
  }

  //
  // Path lengths
  //
  
  unsigned int minimumTunnelPath =
    (unsigned int)ceil((entranceNearby?minimumTunnelPathMetersNearOtherEntrance:minimumTunnelPathMeters) /
                       directionStepCondensed);
  
  //
  // Check the place looks ok -- we are on empty/degenerate spot, and
  // towards the chosen inside direction there's a tunnel.
  //
  
  if (!checkForm(isEmptyOrDegenerate,
                 matrix2xIndexStart,matrix2yIndexStart)) {
    debugreturn("    here is not empty/degenerate",0);
  }
  if (!canIncreaseIndex(matrix2xIndexStart,matrix2yIndexStart,
                        xDirection*formCondense, yDirection*formCondense)) {
    debugreturn("    can not search far enough inside",0);
  }
  if (!checkFormRange(isTunnel,
                      matrix2xIndexStart + xDirection*formCondense,matrix2yIndexStart + yDirection*formCondense,
                      xDirection*formCondense, yDirection*formCondense,
                      minimumTunnelPath,
                      0)) {
    debugreturn("    to inside direction is not a tunnel",0);
  }
  if (!canIncreaseIndex(matrix2xIndexStart,matrix2yIndexStart,
                        -xDirection*formCondense, -yDirection*formCondense)) {
    debugreturn("can not search far enough outside",0);
  }
  if (!checkFormRange(isEmptyOrDegenerate,
                      matrix2xIndexStart - xDirection*formCondense,matrix2yIndexStart - yDirection*formCondense,
                      -xDirection*formCondense, -yDirection*formCondense,
                      minimumTunnelPath,
                      1)) {
    debugreturn("    to outside direction is not empty",0);
  }

  //
  // Find the right z level to start from, searching from the top
  // until we have a place where there's actually material towards the
  // tunnel (ceiling).
  //

  debugf("  find right z level at %u,%u", matrix3xIndex, matrix3yIndex);
  for (unsigned int matrix3zIndexSearch = matrix3.zIndexSize - 2; 1; matrix3zIndexSearch--) {

    debugf("  find right z level at %u,%u now level %u", matrix3xIndex, matrix3yIndex, matrix3zIndexSearch);
    if (matrix3zIndexSearch > 0 &&
        check3DMaterial(matrix3xIndex + xDirection,matrix3yIndex + yDirection,matrix3zIndexSearch)) {
      if (potentialEntranceAnalysis(matrix3xIndex, matrix3yIndex, matrix3zIndexSearch - 1,
                                    xDirection, yDirection,
                                    entranceNearby)) {
        debugf("  found an entrance at %u,%u, clearing degenerates from nearby", matrix3xIndex, matrix3yIndex);
        collectMaterialToClear(isDegenerate,
                               isEmpty,
                               matrix2xIndexStart - xDirection*formCondense,matrix2yIndexStart - yDirection*formCondense,
                               -xDirection*formCondense, -yDirection*formCondense,
                               1,
                               nClearedMaterial,
                               maxClearedMaterial,
                               clearedMaterialX,
                               clearedMaterialY);
#if 0
        debugf("  setting one unit inside entrance also to be an entrance");
        forms.setForm(matrix2xIndexStart + xDirection*formCondense,
                      matrix2yIndexStart + yDirection*formCondense,
                      matrix2xIndexStart + matrix2xStep + xDirection*formCondense,
                      matrix2yIndexStart + matrix2yStep + yDirection*formCondense,
                      outlinerform_mainform_dripline);
#endif
        
        debugreturn("  found an entrance point",1);
      }
    }

    if (matrix3zIndexSearch == 0) {
      break;
    }
  }
  
  // Nothing found
  debugreturn("  z limit hit, bailing",0);
  return(0);
}

bool
ProcessorForms::potentialEntranceAnalysis(const unsigned int matrix3xIndex,
                                          const unsigned int matrix3yIndex,
                                          const unsigned int matrix3zIndex,
                                          const int xDirection,
                                          const int yDirection,
                                          const bool entranceNearby) const {

  //
  // Sanity checks
  //

  assert(matrix3xIndex < matrix3.xIndexSize);
  assert(matrix3yIndex < matrix3.yIndexSize);
  assert(matrix3zIndex < matrix3.zIndexSize);

  //
  // Figure out how far into the cave we need to go to convince
  // ourselves that this is really an entrance.
  //
  
  outlinerreal directionStepCondensed;
  if (xDirection == 0.0) {
    directionStepCondensed = stepyCondensed;
  } else if (yDirection != 0.0 && xDirection != 0.0) {
    directionStepCondensed = (stepxCondensed+stepyCondensed)/2;
  } else {
    directionStepCondensed = stepxCondensed;
  }
  const unsigned int minimumTunnelPathCondensed =
    (unsigned int)ceil((entranceNearby?minimumTunnelPathMetersNearOtherEntrance:minimumTunnelPathMeters) /
                       directionStepCondensed);

  //
  // Figure out how high the entrance must be to fit a human
  //
  
  const unsigned int minimumTunnelHeightCondensed =
    (unsigned int)ceil((entranceNearby?minimumTunnelHeightMetersNearOtherEntrance:minimumTunnelHeightMeters) /
                       stepzCondensed);
  unsigned int pathSoFar = 0;
  unsigned int heightSoFar = 0;

  //
  // Loop through lower and lower levels in z-coordinate, looking at
  // the cave entrance at that level to ensure there's free access to
  // the cave. Stop when we've found high and long enough entrance.
  //

  if (matrix3zIndex == 0) debugreturn("too close to bottom",0);
  unsigned int matrix3zIndexSearch = matrix3zIndex - 1;
  unsigned int furthestPointX = 0;
  unsigned int furthestPointY = 0;
  unsigned int furthestPointZ = 0;
  while (heightSoFar < minimumTunnelHeightCondensed || pathSoFar < minimumTunnelPathCondensed) {
    
    //
    // Is there material in this exact spot? If yes, we are not in an
    // entrance.
    //
    
    debugf("  tunnel check at %u,%u level %u", matrix3xIndex, matrix3yIndex, matrix3zIndexSearch);
    if (check3DMaterial(matrix3xIndex,matrix3yIndex,matrix3zIndexSearch)) {
      debugreturn("  hit material here on z search",0);
    }

    //
    // How far inside can we go at this level?
    //
    
    unsigned int thisFurthestPointX = 0;
    unsigned int thisFurthestPointY = 0;
    unsigned int thisFurthestPointZ = 0;
    unsigned int far = check3DMaterialRangeHorizontal(0,
                                                      matrix3xIndex,matrix3yIndex,matrix3zIndexSearch,
                                                      xDirection,yDirection,
                                                      minimumTunnelPathCondensed,
                                                      thisFurthestPointX,
                                                      thisFurthestPointY,
                                                      thisFurthestPointZ);
    if (far > 0 && far >= pathSoFar) {
      pathSoFar = far;
      furthestPointX = thisFurthestPointX;
      furthestPointY = thisFurthestPointY;
      furthestPointZ = thisFurthestPointZ;
    }
    
    //
    // If not at all, the original place was not  the entrance. Bail out.
    //
    
    if (far == 0) {
      debugreturn("    hit again neighbor material on z search",0);
    }

    //
    // Advance one level down in z-direction.
    //
    
    if (matrix3zIndexSearch == 0) debugreturn("  run out of z space in search of entrance",0);
    matrix3zIndexSearch--;
    heightSoFar++;
  }
  assert(heightSoFar >= minimumTunnelHeightCondensed);
  assert(pathSoFar >= minimumTunnelPathCondensed);
  assert(furthestPointX < matrix3.xIndexSize);
  assert(furthestPointY < matrix3.yIndexSize);
  assert(furthestPointZ < matrix3.zIndexSize);

  //
  // So now we have enough of a hole into the cave. Now we still need
  // to ensure that the hole is within the cave, and not e.g.,
  // underneath. Scan further down to see if there's material under
  // the entrance.
  //

  debugf("  underneath check at %u,%u furthest %u,%u,%u",
         matrix3xIndex, matrix3yIndex,
         furthestPointX,furthestPointY,furthestPointZ);
  bool materialUnder = 0;
  while (!materialUnder) {
    if (check3DMaterialDown(furthestPointX,furthestPointY,furthestPointZ)) {
      materialUnder = 1;
    } else {
      if (matrix3zIndexSearch == 0) debugreturn("  run out of z space in search of material under the entrance",0);
      matrix3zIndexSearch--;
    }
  }
  
  if (!materialUnder) {
    debugreturn("  couldn't find material under",0);
  }
  
  //
  // Success!
  //

  debugf("  entrance at %u,%u",
         matrix3xIndex,
         matrix3yIndex);
  debugreturn("  found an entrance",1);
}

bool
ProcessorForms::check3DMaterial(const unsigned int matrix3xIndex,
                                const unsigned int matrix3yIndex,
                                const unsigned int matrix3zIndex) const {
  assert(matrix3xIndex < matrix3.xIndexSize);
  assert(matrix3yIndex < matrix3.yIndexSize);
  assert(matrix3zIndex < matrix3.zIndexSize);
  return(matrix3.getMaterialMatrix(matrix3xIndex,matrix3yIndex,matrix3zIndex));
}

bool
ProcessorForms::check3DMaterialDown(const unsigned int matrix3xIndex,
                                    const unsigned int matrix3yIndex,
                                    const unsigned int matrix3zIndex) const {
  assert(matrix3xIndex < matrix3.xIndexSize);
  assert(matrix3yIndex < matrix3.yIndexSize);
  assert(matrix3zIndex < matrix3.zIndexSize);
  unsigned int matrix3zIndexSearch = matrix3zIndex;

  while (!matrix3.getMaterialMatrix(matrix3xIndex,matrix3yIndex,matrix3zIndexSearch)) {
    if (matrix3zIndexSearch == 0) return(0);
    matrix3zIndexSearch--;
  }

  return(1);
}

unsigned int
ProcessorForms::check3DMaterialRangeHorizontal(const bool expectedMaterial,
                                               const unsigned int matrix3xIndex,
                                               const unsigned int matrix3yIndex,
                                               const unsigned int matrix3zIndex,
                                               const int xDirection,
                                               const int yDirection,
                                               const unsigned int steps,
                                               unsigned int& furthestPointX,
                                               unsigned int& furthestPointY,
                                               unsigned int& furthestPointZ) const {
  unsigned int matrix3xIndexSearch = matrix3xIndex;
  unsigned int matrix3yIndexSearch = matrix3yIndex;

  unsigned int n = 0;
  furthestPointX = matrix3xIndexSearch;
  furthestPointY = matrix3yIndexSearch;
  furthestPointZ = matrix3zIndex;
  while (n < steps) {
    if (xDirection < 0 && matrix3xIndexSearch == 0) return(n);
    if (yDirection < 0 && matrix3yIndexSearch == 0) return(n);
    if (xDirection > 0 && matrix3xIndexSearch >= matrix3.xIndexSize - 2) return(n);
    if (yDirection > 0 && matrix3yIndexSearch >= matrix3.yIndexSize - 2) return(n);
    matrix3xIndexSearch += xDirection;
    matrix3yIndexSearch += yDirection;
    bool mat = check3DMaterial(matrix3xIndexSearch,matrix3yIndexSearch,matrix3zIndex);
    if (mat != expectedMaterial) return(n);
    furthestPointX = matrix3xIndexSearch;
    furthestPointY = matrix3yIndexSearch;
    furthestPointZ = matrix3zIndex;
    n++;
  }
  
  return(n);
}

unsigned int
ProcessorForms::formAnalysisCountLayers(const unsigned int matrix3xIndex,
                                        const unsigned int matrix3yIndex) const {
  unsigned int count = 0;
  bool layerOn = 0;
  debugf("  count layers %u,%u", matrix3xIndex, matrix3yIndex);
  for (unsigned int matrix3zIndex = 0; matrix3zIndex < matrix3.zIndexSize -2; matrix3zIndex++) {
    debugf("  iter count %u,%u,%u", matrix3xIndex, matrix3yIndex, matrix3zIndex);
    bool material = matrix3.getMaterialMatrix(matrix3xIndex,matrix3yIndex,matrix3zIndex);
    if (layerOn && material) continue;
    else if (!layerOn && !material) continue;
    else if (!layerOn && material) { layerOn = 1; count++; continue; }
    else if (layerOn && !material) { layerOn = 0; continue; }
  }
  debugf("  count done %u", count);
  return(count);
}



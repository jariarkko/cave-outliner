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
  infof("  form matrix2 box %s", buf);
  OutlinerMath::boxDescribe(matrix3.boundingBox,buf,sizeof(buf),1);
  infof("  form matrix3 box %s", buf);

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
      infof("    analyze phase 1 %u,%u: (matrix2 %u,%u .. %u,%u)",
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
  for (unsigned int i = 0; i < 150 && i < matrix3.xIndexSize - 2; i++) {
    infof("Slice %u:", i);
    char buf[120];
    memset(buf,0,sizeof(buf));
    for (unsigned int c = 0; c < matrix3.yIndexSize - 2 && c < 80; c++) {
      snprintf(buf+strlen(buf),sizeof(buf)-1-strlen(buf),"%u", c%10);
    }
    infof(buf);
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
    infof(buf);
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
      infof(buf);
    }
    infof("");
  }
  
  // Phase2
  infof("Form analysis phase 2...");
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    outlinerreal x = matrix3.indexToCoordinateX(xIndex);
    unsigned int matrix2xIndexStart = matrix2.coordinateXToIndex(x);
    unsigned int matrix2xIndexEnd = matrix2xIndexStart + formCondense - 1;
    for (unsigned int yIndex = 0; yIndex < matrix3.yIndexSize - 2; yIndex++) {
      outlinerreal y = matrix3.indexToCoordinateY(yIndex);
      unsigned int matrix2yIndexStart = matrix2.coordinateYToIndex(y);
      unsigned int matrix2yIndexEnd = matrix2yIndexStart + formCondense - 1;
      infof("  analyze phase 2 %u,%u: %.2f,%.2f (matrix2 %u,%u .. %u,%u)",
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
  if (entranceAnalysis(matrix3xIndex,matrix3yIndex, 1, 0, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep) ||
      entranceAnalysis(matrix3xIndex,matrix3yIndex, 0, 1, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep) ||
      entranceAnalysis(matrix3xIndex,matrix3yIndex,-1, 0, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep) ||
      entranceAnalysis(matrix3xIndex,matrix3yIndex, 0,-1, matrix2xIndexStart,matrix2xStep,matrix2yIndexStart,matrix2yStep)) {
    
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

#define dirstring(d)    (((d) == 0) ? "" : ((d) < 0 ? "--" : "++"))
#define debugreturn(w,v) { bool val = (v); infof("%s: returning %u", w, val); return(val); }

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
  if (matrix2xIndex == 0 && xDirection < 0) debugreturn("can increase runs to matrix x start",0);
  if (matrix2yIndex == 0 && yDirection < 0) debugreturn("can increase runs to matrix y start",0);
  if (matrix2xIndex >= forms.xIndexSize - 2 && xDirection > 0) debugreturn("can increase runs to matrix x end",0);
  if (matrix2yIndex >= forms.yIndexSize - 2 && yDirection > 0) debugreturn("can increase runs to matrix y end",0);
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
  for (unsigned int i = 0; i < steps; i++) {
    outlinerform form = forms.getForm(matrix2xIndexIter,matrix2yIndexIter);
    bool val = (*checkFunction)(form);
    if (!val) debugreturn("check range fails",0);
    if (i+1 < steps) {
      if (!canIncreaseIndex(matrix2xIndex,matrix2yIndex,xDirection,yDirection)) {
        debugreturn("check range runs to matrix ends",okToRunToModelEnd);
      }
    }
  }
  debugreturn("check range succeeds",1);
}

static const outlinerreal minimumTunnelPathMeters = 0.5;
static const outlinerreal minimumTunnelHeightMeters = 0.4;

bool
ProcessorForms::entranceAnalysis(const unsigned int matrix3xIndex,
                                 const unsigned int matrix3yIndex,
                                 const int xDirection,
                                 const int yDirection,
                                 const unsigned int matrix2xIndexStart,
                                 const unsigned int matrix2xStep,
                                 const unsigned int matrix2yIndexStart,
                                 const unsigned int matrix2yStep) const {

  //
  // Debugs
  //
  
  infof("  entrance analysis from %u%s,%u%s",
        matrix3xIndex, dirstring(xDirection),
        matrix3yIndex, dirstring(yDirection));

  //
  // Preliminary variables
  //
  
  outlinerreal directionStep;
  if (xDirection == 0.0) {
    directionStep = stepy;
  } else if (yDirection != 0.0 && xDirection != 0.0) {
    directionStep = (stepx+stepy)/2;
  } else {
    directionStep = stepx;
  }
  unsigned int minimumTunnelPath = (unsigned int)ceil(minimumTunnelPathMeters / directionStep);
  
  //
  // Check the place looks ok -- we are on empty/degenerate spot, and
  // towards the chosen inside direction there's a tunnel.
  //
  
  if (!checkForm(isEmptyOrDegenerate,
                 matrix2xIndexStart,matrix2yIndexStart)) {
    debugreturn("here is not empty/degenerate",0);
  }
  if (!canIncreaseIndex(matrix2xIndexStart,matrix2yIndexStart,
                        xDirection, yDirection)) {
    debugreturn("can not search far enough inside",0);
  }
  if (!checkFormRange(isTunnel,
                      matrix2xIndexStart + xDirection,matrix2yIndexStart + yDirection,
                      xDirection, yDirection,
                      minimumTunnelPath,
                      0)) {
    debugreturn("to inside direction is not a tunnel",0);
  }
  if (!canIncreaseIndex(matrix2xIndexStart,matrix2yIndexStart,
                        -xDirection*formCondense, -yDirection*formCondense)) {
    debugreturn("can not search far enough outside",0);
  }
  if (!checkFormRange(isEmptyOrDegenerate,
                      matrix2xIndexStart - xDirection*formCondense,matrix2yIndexStart - yDirection*formCondense,
                      -xDirection, -yDirection,
                      minimumTunnelPath,
                      1)) {
    debugreturn("to inside direction is not a tunnel",0);
  }

  //
  // Find the right z level to start from, searching from the top
  // until we have a place where there's actually material towards the
  // tunnel (ceiling).
  //

  infof("  find right z level at %u,%u", matrix3xIndex, matrix3yIndex);
  for (unsigned int matrix3zIndexSearch = matrix3.zIndexSize - 2; 1; matrix3zIndexSearch--) {

    infof("  find right z level at %u,%u now level %u", matrix3xIndex, matrix3yIndex, matrix3zIndexSearch);
    if (matrix3zIndexSearch > 0 &&
        check3DMaterial(matrix3xIndex + xDirection,matrix3yIndex + yDirection,matrix3zIndexSearch)) {
      if (potentialEntranceAnalysis(matrix3xIndex, matrix3yIndex, matrix3zIndexSearch - 1,
                                    xDirection, yDirection)) {
        debugreturn("found an entrance point",1);
      }
    }

    if (matrix3zIndexSearch == 0) {
      break;
    }
  }
  
  // Nothing found
  infof("  z limit");
  return(0);
}

bool
ProcessorForms::potentialEntranceAnalysis(const unsigned int matrix3xIndex,
                                          const unsigned int matrix3yIndex,
                                          const unsigned int matrix3zIndex,
                                          const int xDirection,
                                          const int yDirection) const {

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
  const unsigned int minimumTunnelPathCondensed = (unsigned int)ceil(minimumTunnelPathMeters / directionStepCondensed);

  //
  // Figure out how high the entrance must be to fit a human
  //
  
  const unsigned int minimumTunnelHeightCondensed = (unsigned int)ceil(minimumTunnelHeightMeters / stepzCondensed);
  unsigned int pathSoFar = 0;
  unsigned int heightSoFar = 0;

  //
  // Loop through lower and lower levels in z-coordinate, looking at
  // the cave entrance at that level to ensure there's free access to
  // the cave. Stop when we've found high and long enough entrance.
  //

  if (matrix3zIndex == 0) debugreturn("too close to bottom",0);
  unsigned int matrix3zIndexSearch = matrix3zIndex - 1;
  unsigned int furthestPointX;
  unsigned int furthestPointY;
  unsigned int furthestPointZ;
  while (heightSoFar < minimumTunnelHeightCondensed || pathSoFar < minimumTunnelPathCondensed) {
    
    //
    // Is there material in this exact spot? If yes, we are not in an
    // entrance.
    //
    
    infof("  tunnel check at %u,%u level %u", matrix3xIndex, matrix3yIndex, matrix3zIndexSearch);
    if (check3DMaterial(matrix3xIndex,matrix3yIndex,matrix3zIndexSearch)) {
      debugreturn("hit material here on z search",0);
    }

    //
    // How far inside can we go at this level?
    //
    
    unsigned int thisFurthestPointX;
    unsigned int thisFurthestPointY;
    unsigned int thisFurthestPointZ;
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
      debugreturn("hit again neighbor material on z search",0);
    }

    //
    // Advance one level down in z-direction.
    //
    
    if (matrix3zIndexSearch == 0) debugreturn("run out of z space in search of entrance",0);
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

  infof("  underneath check at %u,%u furthest %u,%u,%u",
        matrix3xIndex, matrix3yIndex,
        furthestPointX,furthestPointY,furthestPointZ);
  bool materialUnder = 0;
  while (!materialUnder) {
    if (check3DMaterialDown(furthestPointX,furthestPointY,furthestPointZ)) {
      materialUnder = 1;
    } else {
      if (matrix3zIndexSearch == 0) debugreturn("run out of z space in search of material under the entrance",0);
      matrix3zIndexSearch--;
    }
  }
  
  if (!materialUnder) {
    debugreturn("couldn't find material under",0);
  }
  
  //
  // Success!
  //
  
  debugreturn("found an entrance",1);
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

#if 0

bool
ProcessorForms::entranceAnalysis(const unsigned int matrix3xIndex,
                                 const unsigned int matrix3yIndex,
                                 const int xDirection,
                                 const int yDirection,
                                 const unsigned int matrix2xIndexStart,
                                 const unsigned int matrix2xStep,
                                 const unsigned int matrix2yIndexStart,
                                 const unsigned int matrix2yStep) const {
  
  infof("  entrance analysis from %u%s,%u%s",
        matrix3xIndex, dirstring(xDirection),
        matrix3yIndex, dirstring(yDirection));
  const outlinerreal minimumTunnelPath = 0.5;
  const outlinerreal minimumTunnelHeight = 0.4;
  outlinerreal pathSoFar = 0.0;
  outlinerreal pathHeightSoFar = 0.0;
  unsigned int matrix3zIndexSearch = matrix3.zIndexSize - 2;
  unsigned int matrix3zIndexSearchStart = matrix3zIndexSearch;
  
  do {

    unsigned int matrix3xIndexSearch = matrix3xIndex;
    unsigned int matrix3yIndexSearch = matrix3yIndex;
    unsigned int matrix3xIndexSearchReverse = matrix3xIndex;
    unsigned int matrix3yIndexSearchReverse = matrix3yIndex;
    unsigned int matrix2xIndex = matrix2xIndexStart;
    unsigned int matrix2yIndex = matrix2yIndexStart;

    // Check if our starting point has material at the given z
    // height. If so, this isn't an entrance.
    bool materialAtPointIn3D = matrix3.getMaterialMatrix(matrix3xIndex,matrix3yIndex,matrix3zIndexSearch);
    infof("    outer loop %u,%u level %u (3d material = %u)", 
          matrix3xIndexSearch, matrix3yIndexSearch, matrix3zIndexSearch,
          materialAtPointIn3D);
    if (materialAtPointIn3D) {
      if (matrix3zIndexSearch == 0) { infof("    z bottom limit"); return(0); }
      pathSoFar = pathHeightSoFar = 0.0;
      matrix3zIndexSearch--;
      matrix3zIndexSearchStart = matrix3zIndexSearch;
      infof("    continue main loop due to material at level %u", matrix3zIndexSearch);
      continue;
    }
    
    do {
      
      infof("      inner loop %u,%u level %u (reverse %u,%u, path so far %.2f, %.2f)",
            matrix3xIndexSearch, matrix3yIndexSearch, matrix3zIndexSearch,
            matrix3xIndexSearchReverse, matrix3yIndexSearchReverse,
            pathSoFar, pathHeightSoFar);
      
      // Did we reach an border in the image? If so, bail out and
      // conclude the result is inconclusive :-)
      if (matrix3zIndexSearch == 0) { infof("    z limit"); return(0); }
      if (xDirection < 0 && matrix3xIndexSearch == 0) { infof("    x limit"); return(0); }
      if (yDirection < 0 && matrix3yIndexSearch == 0) { infof("    y limit"); return(0); }
      if (xDirection > 0 && matrix3xIndexSearch >= matrix3.xIndexSize - 2) { infof("    x limit"); return(0); }
      if (yDirection > 0 && matrix3yIndexSearch >= matrix3.yIndexSize - 2) { infof("    y limit"); return(0); }
      if (xDirection > 0 && matrix3xIndexSearchReverse == 0) matrix3xIndexSearchReverse += xDirection;
      if (yDirection > 0 && matrix3yIndexSearchReverse == 0) matrix3yIndexSearchReverse += yDirection;
      if (xDirection < 0 && matrix3xIndexSearchReverse >= matrix3.xIndexSize - 2) matrix3xIndexSearchReverse -= xDirection;
      if (yDirection < 0 && matrix3yIndexSearchReverse >= matrix3.yIndexSize - 2) matrix3yIndexSearchReverse -= yDirection;
      
      // Otherwise, advance the search
      matrix3xIndexSearch += xDirection;
      matrix3yIndexSearch += yDirection;
      matrix3xIndexSearchReverse -= xDirection;
      matrix3yIndexSearchReverse -= yDirection;
      matrix2xIndex += xDirection * matrix2xStep;
      matrix2yIndex += yDirection * matrix2yStep;
      
      // And then see if we can find the right type of material at
      // this xy-position and empty space at level z in the
      // xyz-matrix. There's two cases:
      //
      //   1) We are first level (path height = 0): then we MUST have
      //      empty here, material in next slot to the direction we're
      //      looking at (inside the cave), empty in the other
      //      direction. Otherwise we might be looking for an entrance
      //      above the cave.
      //
      //   2) Otherwise, we must have empty in the next slot(s)
      //   towards the inside of the cave and empty on the other
      //   direction. And still empty in the position we are in.
      //
      outlinerform neighborForm = forms.getForm(matrix2xIndex,matrix2yIndex);
      bool materialIn3D = matrix3.getMaterialMatrix(matrix3xIndexSearch,matrix3yIndexSearch,matrix3zIndexSearch);
      bool materialIn3Dreverse = matrix3.getMaterialMatrix(matrix3xIndexSearchReverse,matrix3yIndexSearchReverse,matrix3zIndexSearch);
      infof("      inner investigation center %u,%u search %u,%u (reverse %u,%u): nf %u mat %u revmat %u level %u path height %.2f",
            matrix3xIndex, matrix3yIndex,
            matrix3xIndexSearch, matrix3yIndexSearch,
            matrix3xIndexSearchReverse, matrix3yIndexSearchReverse,
            neighborForm,
            materialIn3D,
            materialIn3Dreverse,
            matrix3zIndexSearch,
            pathHeightSoFar);
      
      if (pathHeightSoFar == 0.0 &&
          (!materialIn3D || materialIn3Dreverse)) {
        infof("      inner loop must go down, not hitting material at level %u", matrix3zIndexSearch);
        pathSoFar = pathHeightSoFar = 0.0;
        matrix3xIndexSearch = matrix3xIndexSearchReverse = matrix3xIndex;
        matrix3yIndexSearch = matrix3yIndexSearchReverse = matrix3yIndex;
        matrix2xIndex = matrix2xIndexStart;
        matrix2yIndex = matrix2yIndexStart;
        if (matrix3zIndexSearch == 0) return(0);
        matrix3zIndexSearch--;
        matrix3zIndexSearchStart = matrix3zIndexSearch;
        goto continueOuterLoop;
      }
      
      if ((neighborForm != outlinerform_mainform_tunnel &&
           neighborForm != outlinerform_mainform_tunnel_stalac) ||
          materialIn3D ||
          materialIn3Dreverse) {
        infof("      inner loop continue due to neighbor or material content %u %u %u",
              neighborForm, materialIn3D, materialIn3Dreverse);
        pathSoFar = pathHeightSoFar = 0.0;
        matrix3xIndexSearch = matrix3xIndexSearchReverse = matrix3xIndex;
        matrix3yIndexSearch = matrix3yIndexSearchReverse = matrix3yIndex;
        matrix2xIndex = matrix2xIndexStart;
        matrix2yIndex = matrix2yIndexStart;
        if (matrix3zIndexSearch == 0) return(0);
        matrix3zIndexSearch--;
        matrix3zIndexSearchStart = matrix3zIndexSearch;
        goto continueOuterLoop;
      }

      // So far so good, lets increase the length we have searched to
      // see if this is enough
      if (xDirection != 0) pathSoFar += stepxCondensed;
      else if (yDirection != 0) pathSoFar += stepyCondensed;
      
      // See if we can declare a tentative win at this level
      if (pathSoFar >= minimumTunnelPath) {
        infof("    level looks like an entrance at %u,%u => %u,%u (level %u from %u, path length %.2f height %.2f)",
              matrix3xIndex,matrix3yIndex,
              matrix3xIndexSearch,matrix3yIndexSearch,
              matrix3zIndexSearch,matrix3zIndexSearch,
              pathSoFar, pathHeightSoFar);
        break;
      }
      
      infof("      inner loop next cycle");
      
    } while (1);

    // See if we can declare a win
    if (pathHeightSoFar >= minimumTunnelHeight) {
      infof("    found entrance at %u,%u (level %u from %u, path height %.2f)",
            matrix3xIndex,matrix3yIndex,
            matrix3zIndexSearch,matrix3zIndexSearchStart,
            pathHeightSoFar);
      return(1);
    }
    
    matrix3zIndexSearch--;
    pathHeightSoFar += stepzCondensed;
    infof("    outer loop next cycle");

  continueOuterLoop:
    infof("    continuing outer loop");
    
  } while (matrix3zIndexSearch > 0);
  
  // Nothing found
  infof("  z limit");
  return(0);
}

#endif

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



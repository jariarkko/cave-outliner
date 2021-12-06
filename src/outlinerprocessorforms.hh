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

#ifndef PROCESSORFORMS_HH
#define PROCESSORFORMS_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"
#include "outlinersvg.hh"
#include "outlinerindexedmesh.hh"
#include "outlinermaterialmatrix2d.hh"
#include "outlinermaterialmatrix3d.hh"
#include "outlinerformmatrix2d.hh"
#include "outlineroutlineanalyzer.hh"
#include "outlinershaperecognizer.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

typedef bool (*ProcessorFormChecker)(outlinerform form);
class DepthMap;

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class ProcessorForms {

public:
  
  ProcessorForms(const OutlinerBox3D& boundingBoxIn,
                 const enum outlinerdirection directionIn,
                 const OutlinerBox2D& planviewBoundingBoxIn,
                 const outlinerreal stepxIn,
                 const outlinerreal stepyIn,
                 const outlinerreal stepzIn,
                 const unsigned int formCondenseIn,
                 MaterialMatrix2D& matrix2In,
                 class Processor& procIn);
  ~ProcessorForms();

  //
  // Form analysis main functions
  //
  
  bool performFormAnalysis(const aiScene* scene);
  OutlinerSvgStyle formToColor(const unsigned int xIndex,
                               const unsigned int yIndex) const;
  bool formIsEntrance(const unsigned int xIndex,
                      const unsigned int yIndex) const;
  outlinerform getForm(const unsigned int xIndex,
                       const unsigned int yIndex) const;
  void drawSpines(SvgCreator& svg) const;

  //
  // Access to data
  //
  
  const DepthMap& getFloorDepthMap(void);
  const DepthMap& getRoofDepthMap(void);
  
  //
  // Coordinate transformations
  //

  void condensedXIndexToIndex(const unsigned int matrix3xIndex,
                              unsigned int& matrix2xIndexStart,
                              unsigned int& matrix2xIndexEnd) const;
  void condensedYIndexToIndex(const unsigned int matrix3yIndex,
                              unsigned int& matrix2yIndexStart,
                              unsigned int& matrix2yIndexEnd) const;
  void condensedIndexesToIndexes(const unsigned int matrix3xIndex,
                                 const unsigned int matrix3yIndex,
                                 unsigned int& matrix2xIndexStart,
                                 unsigned int& matrix2yIndexStart,
                                 unsigned int& matrix2xIndexEnd,
                                 unsigned int& matrix2yIndexEnd) const;
  void condensedIndexIncrease(unsigned int& matrix2Index) const;
  
private:
  
  friend class OutlineAnalyzer;
  
  const OutlinerBox3D boundingBox;
  const enum outlinerdirection direction;
  const OutlinerBox2D planviewBoundingBox;
  const outlinerreal stepx;
  const outlinerreal stepy;
  const outlinerreal stepz;
  const unsigned int formCondense;
  const outlinerreal stepxCondensed;
  const outlinerreal stepyCondensed;
  const outlinerreal stepzCondensed;
  MaterialMatrix2D& matrix2;           // Processor's main plan-view matrix
  MaterialMatrix3D matrix3;            // Form analysis 3D view matrix
  FormMatrix2D forms;                  // Form analysis results
  OutlineAnalyzer outlineAnalyzer;     // Outline analyzer
  ShapeRecognizer recognizer;          // Recognize stalactites etc.
  class Processor& proc;               // Reference to the main processor object
  static const unsigned int maxClearedMaterial = 20 * 1024;
  unsigned int nClearedMaterial;
  unsigned int clearedMaterialX[maxClearedMaterial];
  unsigned int clearedMaterialY[maxClearedMaterial];
  
  //
  // Internal functions, main functions
  //
  
  bool performFormAnalysisAnalyze(void);
  bool performFormAnalysisOutline(void);
  bool performFormAnalysisInsideAnalyze(void);

  //
  // Internal functions, basic form analysis
  //
  
  bool performFormAnalysisSlicing(const aiScene* scene);
  bool performFormAnalysisOneSlice(const aiScene* scene,
                                   unsigned int xIndex);
  bool performFormAnalysisAnalyzeOnePixelPhase1(const unsigned int matrix3xIndex,
                                                const unsigned int matrix3yIndex,
                                                const unsigned int matrix2xIndexStart,
                                                const unsigned int matrix2yIndexStart,
                                                const unsigned int matrix2xIndexEnd,
                                                const unsigned int matrix2yIndexEnd);
  bool performFormAnalysisAnalyzeOnePixelPhase2(const unsigned int matrix3xIndex,
                                                const unsigned int matrix3yIndex,
                                                const unsigned int matrix2xIndexStart,
                                                const unsigned int matrix2yIndexStart,
                                                const unsigned int matrix2xIndexEnd,
                                                const unsigned int matrix2yIndexEnd);
  bool performFormAnalysisAnalyzeOnePixelPhase5(const unsigned int matrix3xIndex,
                                                const unsigned int matrix3yIndex,
                                                const unsigned int matrix2xIndexStart,
                                                const unsigned int matrix2yIndexStart,
                                                const unsigned int matrix2xIndexEnd,
                                                const unsigned int matrix2yIndexEnd);
  unsigned int formAnalysisCountLayers(const unsigned int matrix3xIndex,
                                       const unsigned int matrix3yIndex) const;
  bool entranceAnalysis(const unsigned int matrix3xIndex,
                        const unsigned int matrix3yIndex,
                        const int xDirection,
                        const int yDirection,
                        const unsigned int matrix2xIndexStart,
                        const unsigned int matrix2xStep,
                        const unsigned int matrix2yIndexStart,
                        const unsigned int matrix2yStep,
                        const bool entranceNearby);
  bool potentialEntranceAnalysis(const unsigned int matrix3xIndex,
                                 const unsigned int matrix3yIndex,
                                 const unsigned int matrix3zIndex,
                                 const int xDirection,
                                 const int yDirection,
                                 const bool entranceNearby) const;
  static bool isEmpty(outlinerform form);
  static bool isDegenerate(outlinerform form);
  static bool isEmptyOrDegenerate(outlinerform form);
  static bool isTunnel(outlinerform form);
  static bool isEntrance(outlinerform form);
  bool checkForm(ProcessorFormChecker checkFunction,
                 const unsigned int matrix2xIndex,
                 const unsigned int matrix2yIndex) const;
  bool checkFormRange(ProcessorFormChecker checkFunction,
                      const unsigned int matrix2xIndex,
                      const unsigned int matrix2yIndex,
                      const int xDirection,
                      const int yDirection,
                      const unsigned int steps,
                      const bool okToRunToModelEnd) const;
  void collectMaterialToClear(ProcessorFormChecker checkFunction,
                              ProcessorFormChecker terminationCheckFunction,
                              const unsigned int matrix2xIndex,
                              const unsigned int matrix2yIndex,
                              const int xDirection,
                              const int yDirection,
                              const bool neighbors,
                              unsigned int& n,
                              const unsigned int tableSize,
                              unsigned int* tableX,
                              unsigned int* tableY);
  bool hasSideNeighbor(const unsigned int xIndex,
                       const unsigned int yIndex,
                       const int xDirection,
                       const int yDirection,
                       const unsigned int sideCounter, // 0..5
                       unsigned int& neighX,
                       unsigned int& neighY) const;
  void addToTable(const unsigned int x,
                  const unsigned int y,
                  unsigned int& n,
                  const unsigned int tableSize,
                  unsigned int* tableX,
                  unsigned int* tableY);
  void clearMaterial(const unsigned int n,
                     unsigned int* tableX,
                     unsigned int* tableY);
  bool checkFormNearby(ProcessorFormChecker checkFunction,
                       const unsigned int matrix2xIndex,
                       const unsigned int matrix2yIndex,
                       const unsigned int oneStep,
                       const unsigned int steps) const;
  bool check3DMaterial(const unsigned int matrix3xIndex,
                       const unsigned int matrix3yIndex,
                       const unsigned int matrix3zIndex) const;
  bool check3DMaterialDown(const unsigned int matrix3xIndex,
                           const unsigned int matrix3yIndex,
                           const unsigned int matrix3zIndex) const;
  unsigned int check3DMaterialRangeHorizontal(const bool expectedMaterial,
                                              const unsigned int matrix3xIndex,
                                              const unsigned int matrix3yIndex,
                                              const unsigned int matrix3zIndex,
                                              const int xDirection,
                                              const int yDirection,
                                              const unsigned int steps,
                                              unsigned int& furthestPointX,
                                              unsigned int& furthestPointY,
                                              unsigned int& furthestPointZ) const;
  bool canIncreaseIndex(const unsigned int matrix2xIndex,
                        const unsigned int matrix2yIndex,
                        const int xDirection,
                        const int yDirection) const;

};

#endif // PROCESSORFORMS_HH

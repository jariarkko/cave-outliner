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
                 const outlinerreal formCondenseIn,
                 const MaterialMatrix2D& matrix2In,
                 class Processor& procIn);
  ~ProcessorForms();

  //
  // Form analysis main functions
  //
  
  bool performFormAnalysis(const aiScene* scene);
  OutlinerSvgStyle formToColor(const unsigned int xIndex,
                               const unsigned int yIndex) const;
  
private:
  
  const OutlinerBox3D boundingBox;
  const enum outlinerdirection direction;
  const OutlinerBox2D planviewBoundingBox;
  const outlinerreal stepx;
  const outlinerreal stepy;
  const outlinerreal stepz;
  const outlinerreal formCondense;
  const outlinerreal stepxCondensed;
  const outlinerreal stepyCondensed;
  const outlinerreal stepzCondensed;
  const MaterialMatrix2D& matrix2; // Processor's main plan-view matrix
  MaterialMatrix3D matrix3; // Form analysis 3D view matrix
  FormMatrix2D forms;
  class Processor& proc;
  
  //
  // Internal functions
  //
  
  bool performFormAnalysisSlicing(const aiScene* scene);
  bool performFormAnalysisOneSlice(const aiScene* scene,
                                   unsigned int xIndex);
  bool performFormAnalysisAnalyze(void);
  bool performFormAnalysisAnalyzeOnePixel(const unsigned int matrix3xIndex,
                                          const unsigned int matrix3yIndex,
                                          const unsigned int matrix2xIndexStart,
                                          const unsigned int matrix2yIndexStart,
                                          const unsigned int matrix2xIndexEnd,
                                          const unsigned int matrix2yIndexEnd);
  unsigned int formAnalysisCountLayers(const unsigned int matrix3xIndex,
                                       const unsigned int matrix3yIndex) const;
};

#endif // PROCESSORFORMS_HH

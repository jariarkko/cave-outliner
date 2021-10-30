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

#ifndef PROCESSORCROSSSECTION_HH
#define PROCESSORCROSSSECTION_HH

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
#include "outlinermaterialmatrix.hh"
#include "outlinerprocessor.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

struct ProcessorCrossSectionLineIterator {
  OutlinerVector2D point;
  unsigned int step;
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class ProcessorCrossSection {

public:
  ProcessorCrossSection(const char* fileNameIn,
                        const char* labelIn, // 0 if no label desired
                        enum outlinerdirection sliceDirectionIn,
                        const OutlinerVector2D& lineStartIn,
                        const OutlinerVector2D& lineEndIn,
                        outlinerreal stepzIn,
                        Processor& procIn);
  ~ProcessorCrossSection();
  bool processSceneCrossSection(const aiScene* scene);
  void getLineActualEndPoints(OutlinerLine2D& actualLine,
                              outlinerreal extralineatends);
  
private:

  const unsigned int freespacearound = 2;
  const char* fileName;
  const char* label; // 0 if no label desired
  const enum outlinerdirection sliceDirection;
  const OutlinerVector2D lineStart;
  const OutlinerVector2D lineEnd;
  outlinerreal stepz;
  outlinerreal xDifference;
  outlinerreal yDifference;
  outlinerreal lineLength;
  outlinerreal lineStep;
  outlinerreal lineSteps;
  outlinerreal lineStepX;
  outlinerreal lineStepY;
  OutlinerBox2D sliceVerticalBoundingBox;
  MaterialMatrix* matrix;
  Processor& proc;
  SvgCreator* svg;
  
  //
  // Internal state management
  //
  
  void deleteSvg(void);
  void deleteMatrix(void);

  //
  // Finding out the cross section bounding box
  //
  
  void getSliceVerticalBoundingBox(const aiScene* scene,
                                   OutlinerBox2D& sliceBoundingBox);
  void getSliceVerticalBoundingBoxNode(const aiScene* scene,
                                       const aiNode* node,
                                       bool& set,
                                       OutlinerBox2D& sliceVerticalBoundingBox);
  void getSliceVerticalBoundingBoxMesh(const aiScene* scene,
                                       const aiMesh* mesh,
                                       bool& set,
                                       OutlinerBox2D& sliceVerticalBoundingBox);
  void getSliceVerticalBoundingBoxFace(const aiScene* scene,
                                       const aiMesh* mesh,
                                       const aiFace* face,
                                       outlinerreal x,
                                       outlinerreal y,
                                       bool& set,
                                       OutlinerBox2D& sliceVerticalBoundingBox);
  
  //
  // Drawing cross sections
  //
  
  void drawCrossSection(const aiScene* scene);
  void drawCrossSectionNode(const aiScene* scene,
                            const aiNode* node);
  void drawCrossSectionMesh(const aiScene* scene,
                            const aiMesh* mesh);
  void drawCrossSectionFace(const aiScene* scene,
                            const aiMesh* mesh,
                            const aiFace* face,
                            unsigned int firstStepInBoundingBox,
                            unsigned int currentStep,
                            outlinerreal x,
                            outlinerreal y,
                            outlinerreal z);

  //
  // Coordinate operations
  //

  unsigned int coordinateLineStepToImageXIndex(unsigned int firstStepInBoundingBox,
                                               unsigned int currentStep);
  unsigned int coordinateZToImageYIndex(outlinerreal z);
  
  //
  // Line operations
  //
  
  void calculateLineEquation(void);
  outlinerreal calculateLineXBasedOnY(outlinerreal y);
  void lineIteratorInit(struct ProcessorCrossSectionLineIterator& iter);
  bool lineIteratorDone(struct ProcessorCrossSectionLineIterator& iter);
  void lineIteratorNext(struct ProcessorCrossSectionLineIterator& iter);

};

#endif // PROCESSORCROSSECTION_HH

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

///
/// This is the module inside of the Core module that performs the the drawing of cross sections.
///

class ProcessorCrossSection {

public:

  /// Create an object to prepare for the drawing of a cross section
  /// view of a cave tunnel.
  ProcessorCrossSection(const char* fileNameIn,
                        const char* labelIn, // 0 if no label desired
                        enum outlinerdirection sliceDirectionIn,
                        const OutlinerVector2D& lineStartIn,
                        const OutlinerVector2D& lineEndIn,
                        outlinerreal stepzIn,
                        Processor& procIn);

  /// Release all resources associated with this cross section view,
  /// close the image file, etc.
  ~ProcessorCrossSection();

  /// Draw an actual cross section of the given scene. The coordinates
  /// associated with the line representing the cross section were
  /// specified at the time the ProcessorCrossSection object was
  /// created.
  bool processSceneCrossSection(const aiScene* scene);

  /// Once a cross section has been draw, we have information exactly
  /// how wide/high the cross section is. On the plan view this can be
  /// represented by a line drawn across the cave tunnel, with each
  /// end of the line ending exactly at the tunnel walls. This
  /// function returns the coordinates for this line.
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
                            unsigned int xyStep,
                            unsigned int zStep,
                            outlinerreal x,
                            outlinerreal y,
                            outlinerreal z);
  
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

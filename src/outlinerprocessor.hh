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

#ifndef PROCESSOR_HH
#define PROCESSOR_HH

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

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class IndexedMesh;

struct ProcessorCrossSectionInfo {
  const char* filename;
  OutlinerLine2D line;
  outlinerreal width; // in units of one step
  const char* label; // 0 if no label desired
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This is the main program of the Core module, it performs the
/// actual mapping from a given model and indexed mesh to an SVG
/// image. It creates a material matrix as part of the process.
///

class Processor {

public:

  /// Create a Processor.
  Processor(const char* fileNameIn,
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
            const bool formanalysisIn,
            const bool dimensionsIn,
            unsigned int nCrossSectionsIn,
            struct ProcessorCrossSectionInfo* crossSectionsIn,
            IndexedMesh& indexedIn);

  /// Destruct a Processor.
  ~Processor();

  /// Process a plan fiew fora given scene in the processor, with a
  /// given set of (optional) cross sections.
  bool processScene(const aiScene* scene);
  
private:

  friend class ProcessorCrossSection;
  const char* fileName;
  const unsigned int multiplier;
  const bool smooth;
  const bool mergedLines;
  const float linewidth;
  SvgCreator* svg;
  const unsigned int maxNeighbors = 8;
  const OutlinerBox3D originalBoundingBox;
  const OutlinerBox3D boundingBox;
  OutlinerBox2D boundingBox2D;
  const outlinerreal stepx;
  const outlinerreal stepy;
  const outlinerreal stepz;
  const enum outlinerdirection direction;
  const enum outlineralgorithm algorithm;
  const unsigned int holethreshold;
  const unsigned int lineHolethreshold;
  const bool labels;
  const bool formanalysis;
  const bool dimensions;
  const OutlinerBox2D originalPlanviewBoundingBox;
  const OutlinerBox2D planviewBoundingBox;
  MaterialMatrix2D matrix;
  const unsigned int nCrossSections;
  const struct ProcessorCrossSectionInfo* crossSections;
  IndexedMesh& indexed;
  outlinerreal dimensionBottomLabelingSpaceStartY;
  outlinerreal dimensionRightLabelingSpaceStartX;
  
  //
  // Matrix operations
  //

  bool sceneToMaterialMatrix(const aiScene* scene);
  bool coordinatesInTable(const unsigned int xIndex,
                          const unsigned int yIndex,
                          const unsigned int n,
                          const unsigned int* tableX,
                          const unsigned int* tableY);

  //
  // Neighbor operations in a matrix
  //
  
  void getNeighbours(unsigned int xIndex,
                     unsigned int yIndex,
                     unsigned int& n,
                     unsigned int tableSize,
                     unsigned int* tableX,
                     unsigned int* tableY);
  bool closerNeighborExists(const unsigned int thisX,
                            const unsigned int thisY,
                            const unsigned int xIndex,
                            const unsigned int yIndex,
                            const unsigned int nNeighbors,
                            const unsigned int* neighborTableX,
                            const unsigned int* neighborTableY);
  bool isBorder(unsigned int xIndex,
                unsigned int yIndex,
                MaterialMatrix2D* theMatrix,
                unsigned int& nBorderTo,
                unsigned int borderTableSize,
                bool* borderTablePrev,
                unsigned int* boderTableX,
                unsigned int* borderTableY);

  //
  // Hole removal
  //

  unsigned int objectHoleRemoval(unsigned int& holeMinSize,
                                 unsigned int& holeMaxSize);
  bool objectHoleIsEqualOrSmallerThan(unsigned int xIndex,
                                      unsigned int yIndex,
                                      unsigned int holethreshold,
                                      unsigned int& n,
                                      unsigned int tableSize,
                                      unsigned int* holeXtable,
                                      unsigned int* holeYtable);
  unsigned int lineHoleRemoval(unsigned int& holeMinSize,
                               unsigned int& holeMaxSize);

  //
  // 3D model queries
  //
  
  bool sceneHasMaterial(const aiScene* scene,
                        IndexedMesh& indexed,
                        outlinerreal x,
                        outlinerreal y);
  bool nodeHasMaterial(const aiScene* scene,
                       const aiNode* node,
                       IndexedMesh& indexed,
                       outlinerreal x,
                       outlinerreal y);
  bool meshHasMaterial(const aiScene* scene,
                       const aiMesh* mesh,
                       IndexedMesh& indexed,
                       outlinerreal x,
                       outlinerreal y);
  bool faceHasMaterial(const aiScene* scene,
                       const aiMesh* mesh,
                       const aiFace* face,
                       outlinerreal x,
                       outlinerreal y);
  void faceGetVertices2D(const aiMesh* mesh,
                         const aiFace* face,
                         enum outlinerdirection thisDirection,
                         OutlinerTriangle2D& t);
  void faceGetVertices3D(const aiMesh* mesh,
                         const aiFace* face,
                         OutlinerTriangle3D& t);
  
  //
  // Coordinate management
  //
  
  unsigned int coordinateXToIndex(outlinerreal x);
  unsigned int coordinateYToIndex(outlinerreal y);
  outlinerreal indexToCoordinateX(unsigned int xIndex);
  outlinerreal indexToCoordinateY(unsigned int yIndex);

  //
  // Labels
  //
  
  void addSpaceForLabels(OutlinerBox2D& pictureBoundingBox,
                         bool vertical,
                         bool horizontal,
                         const outlinerreal thisStepX,
                         const outlinerreal thisStepY);
  
  //
  // Dimension lines
  //

  void addSpaceForDimensions(const OutlinerBox2D& objectBoundingBox,
                             OutlinerBox2D& pictureBoundingBox,
                             outlinerreal& bottomDimensionLabelingStartY,
                             outlinerreal& rightDimensionLabelingStartX,
                             const outlinerreal thisStepX,
                             const outlinerreal thisStepY);
  void addDimensionLines(SvgCreator* theSvg,
                         const OutlinerBox2D& objectBoundingBox,
                         outlinerreal bottomDimensionLabelingStartY,
                         outlinerreal rightDimensionLabelingStartX,
                         const outlinerreal thisStepX,
                         const outlinerreal thisStepY);
  bool hasNonHorizontalCrossSections(void);
  bool hasHorizontalCrossSections(void);
  
  //
  // Cross sections
  //
  
  bool processSceneCrossSections(const aiScene* scene,
                                 const unsigned int nCrossSections,
                                 const struct ProcessorCrossSectionInfo* crossSections);
  bool processSceneCrossSection(const aiScene* scene,
                                const unsigned int c,
                                const struct ProcessorCrossSectionInfo* crossSection);
  void addCrossSectionLine(const char* label,
                           OutlinerLine2D& actualLine);
  
  //
  // Image drawing
  //
  
  bool matrixToSvg(MaterialMatrix2D* theMatrix,
                   SvgCreator* theSvg,
                   enum outlineralgorithm theAlgorithm,
                   outlinerreal xStart,
                   outlinerreal yStart,
                   outlinerreal xStep,
                   outlinerreal yStep);
  bool sceneToTrianglesSvg(const aiScene* scene,
                           SvgCreator* theSvg,
                           outlinerreal xStart,
                           outlinerreal yStart,
                           outlinerreal xStep,
                           outlinerreal yStep);
  void nodeToTrianglesSvg(const aiScene* scene,
                          const aiNode* node,
                          SvgCreator* theSvg,
                          IndexedMesh& indexed,
                          OutlinerBox2D& boundingBox);
  void meshToTrianglesSvg(const aiScene* scene,
                          const aiMesh* mesh,
                          SvgCreator* theSvg,
                          IndexedMesh& indexed,
                          OutlinerBox2D& boundingBox);
  void faceToTrianglesSvg(const aiScene* scene,
                          const aiMesh* mesh,
                          const aiFace* face,
                          SvgCreator* theSvg,
                          OutlinerBox2D& boundingBox);
  
  //
  // SVG image management
  //
  
  SvgCreator* createSvg(const char* svgFileName,
                        const OutlinerBox2D& svgBoundingBox,
                        outlinerreal svgStepX,
                        outlinerreal svgStepY,
                        enum outlinerdirection svgDirection);
  void svgDone();
  void createSvgCalculateSizes(const OutlinerBox2D& svgBoundingBox,
                               const outlinerreal stepx,
                               const outlinerreal stepy,
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
                               outlinerreal& yFactor);
};

#endif // PROCESSOR_HH

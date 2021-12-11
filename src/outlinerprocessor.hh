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
#include "outlinermaterialmatrix3d.hh"
#include "outlinerformmatrix2d.hh"
#include "outlinerdepthmap.hh"
#include "outlinersvgoptions.hh"
#include "outlinerprocessoroptions.hh"
#include "outlinerprocessorforms.hh"

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

struct ProcessorRangeInfo {
  bool needed;
  bool set;
  OutlinerBox1D zRange;
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
            const ProcessorOptions& optionsIn,
            const SvgOptions& svgOptionsIn,
            const OutlinerBox3D& originalBoundingBoxIn,
            const OutlinerBox3D& boundingBoxIn,
            const outlinerreal stepxIn,
            const outlinerreal stepyIn,
            const outlinerreal stepzIn,
            const enum outlinerdirection directionIn,
            const enum outlineralgorithm algorithmIn,
            unsigned int nCrossSectionsIn,
            struct ProcessorCrossSectionInfo* crossSectionsIn,
            IndexedMesh& indexedIn);

  /// Destruct a Processor.
  ~Processor();

  /// Process a plan fiew fora given scene in the processor, with a
  /// given set of (optional) cross sections.
  bool processScene(const aiScene* scene);

  /// Access the built model.
  bool getMaterialMatrix(const unsigned int xIndex,
                         const unsigned int yIndex) const;
  
private:

  friend class ProcessorCrossSection;
  friend class ProcessorForms;
  friend class DepthMap;

  const char* fileName;
  const ProcessorOptions options;
  const SvgOptions svgOptions;
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
  const OutlinerBox2D originalPlanviewBoundingBox;
  const OutlinerBox2D planviewBoundingBox;
  MaterialMatrix2D matrix2; // Plan view matrix
  DepthMap* depthMap;
  ProcessorForms formAnalyzer;
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
                     unsigned int* tableY,
                     unsigned int step = 1,
                     unsigned int xSize = 0,
                     unsigned int ySize = 0) const;
  bool closerNeighborExists(const unsigned int thisX,
                            const unsigned int thisY,
                            const unsigned int xIndex,
                            const unsigned int yIndex,
                            const unsigned int nNeighbors,
                            const unsigned int* neighborTableX,
                            const unsigned int* neighborTableY) const;
  bool isBorder(unsigned int xIndex,
                unsigned int yIndex,
                MaterialMatrix2D* theMatrix,
                unsigned int& nBorderTo,
                unsigned int borderTableSize,
                bool* borderTablePrev,
                unsigned int* boderTableX,
                unsigned int* borderTableY) const;

  //
  // Hole removal
  //

  void holeFillingPass(void);
  void lineHoleFillingPass(void);
  void dustRemovingPass(void);
  unsigned int objectHoleRemoval(const bool lookForHoles,
                                 unsigned int& holeMinSize,
                                 unsigned int& holeMaxSize);
  bool objectHoleIsEqualOrSmallerThan(const bool lookForHoles,
                                      unsigned int xIndex,
                                      unsigned int yIndex,
                                      unsigned int threshold,
                                      unsigned int& n,
                                      unsigned int tableSize,
                                      unsigned int* holeXtable,
                                      unsigned int* holeYtable,
                                      unsigned int& nonHoleX,
                                      unsigned int& nonHoleY);
  unsigned int lineHoleRemoval(unsigned int& holeMinSize,
                               unsigned int& holeMaxSize);

  //
  // 3D model queries
  //
  
  bool sceneHasMaterial(const aiScene* scene,
                        IndexedMesh& indexed,
                        outlinerreal x,
                        outlinerreal y,
                        struct ProcessorRangeInfo& range);
  bool nodeHasMaterial(const aiScene* scene,
                       const aiNode* node,
                       IndexedMesh& indexed,
                       outlinerreal x,
                       outlinerreal y,
                       struct ProcessorRangeInfo& range);
  bool meshHasMaterial(const aiScene* scene,
                       const aiMesh* mesh,
                       IndexedMesh& indexed,
                       outlinerreal x,
                       outlinerreal y,
                       struct ProcessorRangeInfo& range);
  bool faceHasMaterial(const aiScene* scene,
                       const aiMesh* mesh,
                       const aiFace* face,
                       outlinerreal x,
                       outlinerreal y,
                       struct ProcessorRangeInfo& range);
  void faceGetVertices2D(const aiMesh* mesh,
                         const aiFace* face,
                         enum outlinerdirection thisDirection,
                         OutlinerTriangle2D& t,
                         OutlinerBox1D& depthRange);
  void faceGetVertices3D(const aiMesh* mesh,
                         const aiFace* face,
                         OutlinerTriangle3D& t);
  
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
  // Main processing functions
  //

  bool preprocessSceneAlgorithmDraw(const aiScene* scene);
  bool processSceneAlgorithmDraw(const aiScene* scene);

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

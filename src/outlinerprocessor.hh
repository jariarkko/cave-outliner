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
#include "outlinermaterialmatrix.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class IndexedMesh;

struct ProcessorCrossSectionInfo {
  const char* filename;
  OutlinerVector2D start;
  OutlinerVector2D end;
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
            unsigned int multiplierIn,
            bool smoothIn,
            bool mergedLinesIn,
            float linewidthIn,
            OutlinerBox3D boundingBoxIn,
            outlinerreal stepxIn,
            outlinerreal stepyIn,
            outlinerreal stepzIn,
            enum outlinerdirection directionIn,
            enum outlineralgorithm algorithmIn,
            unsigned int holethresholdIn,
            unsigned int lineHolethresholdIn,
            bool labelsIn,
            IndexedMesh& indexedIn);

  /// Destruct a Processor.
  ~Processor();

  /// Process a plan fiew fora given scene in the processor, with a
  /// given set of (optional) cross sections.
  bool processScene(const aiScene* scene,
                    unsigned int nCrossSections,
                    struct ProcessorCrossSectionInfo* crossSections);
  
private:

  friend class ProcessorCrossSection;
  const char* fileName;
  unsigned int multiplier;
  bool smooth;
  bool mergedLines;
  float linewidth;
  SvgCreator* svg;
  const unsigned int maxNeighbors = 8;
  OutlinerBox3D boundingBox;
  OutlinerBox2D boundingBox2D;
  outlinerreal stepx;
  outlinerreal stepy;
  outlinerreal stepz;
  enum outlinerdirection direction;
  enum outlineralgorithm algorithm;
  unsigned int holethreshold;
  unsigned int lineHolethreshold;
  bool labels;
  OutlinerVector2D planviewBoundingBoxStart;
  OutlinerVector2D planviewBoundingBoxEnd;
  OutlinerBox2D planviewBoundingBox;
  MaterialMatrix matrix;
  IndexedMesh& indexed;

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
                MaterialMatrix* theMatrix,
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
  // Cross sections
  //
  
  bool processSceneCrossSections(const aiScene* scene,
                                 unsigned int nCrossSections,
                                 struct ProcessorCrossSectionInfo* crossSections);
  bool processSceneCrossSection(const aiScene* scene,
                                unsigned int c,
                                const struct ProcessorCrossSectionInfo* crossSection);
  void addCrossSectionLine(const char* label,
                           OutlinerLine2D& actualLine);
  
  //
  // Image drawing
  //
  
  bool matrixToSvg(MaterialMatrix* theMatrix,
                   SvgCreator* theSvg,
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

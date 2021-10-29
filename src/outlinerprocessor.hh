
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
  HighPrecisionVector2D start;
  HighPrecisionVector2D end;
  const char* label; // 0 if no label desired
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class Processor {

public:
  Processor(const char* fileNameIn,
            unsigned int multiplierIn,
            bool smoothIn,
            bool mergedLinesIn,
            float linewidthIn,
            HighPrecisionVector3D boundingBoxStartIn,
            HighPrecisionVector3D boundingBoxEndIn,
            outlinerreal stepxIn,
            outlinerreal stepyIn,
            outlinerreal stepzIn,
            enum outlinerdirection directionIn,
            enum outlineralgorithm algorithmIn,
            unsigned int holethresholdIn,
            bool labelsIn,
            IndexedMesh& indexedIn);
  ~Processor();
  
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
  HighPrecisionVector3D boundingBoxStart;
  HighPrecisionVector3D boundingBoxEnd;
  HighPrecisionVector2D boundingBoxStart2D;
  HighPrecisionVector2D boundingBoxEnd2D;
  outlinerreal stepx;
  outlinerreal stepy;
  outlinerreal stepz;
  enum outlinerdirection direction;
  enum outlineralgorithm algorithm;
  unsigned int holethreshold;
  bool labels;
  HighPrecisionVector2D planviewBoundingBoxStart;
  HighPrecisionVector2D planviewBoundingBoxEnd;
  MaterialMatrix matrix;
  IndexedMesh& indexed;

  //
  // Matrix operations
  //
  
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
  bool holeIsEqualOrSmallerThan(unsigned int xIndex,
                                unsigned int yIndex,
                                unsigned int holethreshold,
                                unsigned int& n,
                                unsigned int tableSize,
                                unsigned int* holeXtable,
                                unsigned int* holeYtable);

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
                         HighPrecisionTriangle2D& t);
  void faceGetVertices3D(const aiMesh* mesh,
                         const aiFace* face,
                         HighPrecisionTriangle3D& t);
  
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
                           HighPrecisionVector2D& actualLineStart,
                           HighPrecisionVector2D& actualLineEnd);
  
  //
  // Image drawing
  //
  
  void matrixToSvg(MaterialMatrix* theMatrix,
                   SvgCreator* theSvg,
                   outlinerreal xStart,
                   outlinerreal yStart,
                   outlinerreal xStep,
                   outlinerreal yStep);
  
  //
  // SVG image management
  //
  
  SvgCreator* createSvg(const char* svgFileName,
                        const HighPrecisionVector2D& svgBoundingBoxStart,
                        const HighPrecisionVector2D& svgBoundingBoxEnd,
                        enum outlinerdirection svgDirection);
  void svgDone();
  void createSvgCalculateSizes(const HighPrecisionVector2D& svgBoundingBoxStart,
                               const HighPrecisionVector2D& svgBoundingBoxEnd,
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

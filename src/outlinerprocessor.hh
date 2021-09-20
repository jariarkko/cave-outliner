
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

class IndexedMesh;

struct ProcessorCrossSection {
  outlinerhighprecisionreal x;
  const char* filename;
};

class Processor {

public:
  Processor(const char* fileNameIn,
            unsigned int multiplierIn,
            bool smoothIn,
            bool mergedLinesIn,
            float linewidthIn,
            HighPrecisionVector3D boundingboxstartIn,
            HighPrecisionVector3D boundingboxendIn,
            outlinerhighprecisionreal stepxIn,
            outlinerhighprecisionreal stepyIn,
            enum outlinerdirection directionIn,
            enum outlineralgorithm algorithmIn,
            unsigned int holethresholdIn,
            IndexedMesh& indexedIn);
  ~Processor();
  
  bool processScene(const aiScene* scene,
                    unsigned int nCrossSections,
                    struct ProcessorCrossSection* crossSections);
  
private:

  const char* fileName;
  unsigned int multiplier;
  bool smooth;
  bool mergedLines;
  float linewidth;
   SvgCreator* svg;
  const unsigned int maxNeighbors = 8;
  HighPrecisionVector3D boundingboxstart;
  HighPrecisionVector3D boundingboxend;
  outlinerhighprecisionreal stepx;
  outlinerhighprecisionreal stepy;
  enum outlinerdirection direction;
  enum outlineralgorithm algorithm;
  unsigned int holethreshold;
  MaterialMatrix matrix;
  IndexedMesh& indexed;
  
  bool sceneHasMaterial(const aiScene* scene,
                        IndexedMesh& indexed,
                        outlinerhighprecisionreal x,
                        outlinerhighprecisionreal y);
  bool nodeHasMaterial(const aiScene* scene,
                       const aiNode* node,
                       IndexedMesh& indexed,
                       outlinerhighprecisionreal x,
                       outlinerhighprecisionreal y);
  bool meshHasMaterial(const aiScene* scene,
                       const aiMesh* mesh,
                       IndexedMesh& indexed,
                       outlinerhighprecisionreal x,
                       outlinerhighprecisionreal y);
  bool faceHasMaterial(const aiScene* scene,
                       const aiMesh* mesh,
                       const aiFace* face,
                       outlinerhighprecisionreal x,
                       outlinerhighprecisionreal y);
   bool isBorder(unsigned int xIndex,
                 unsigned int yIndex,
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
  bool coordinatesInTable(const unsigned int xIndex,
                          const unsigned int yIndex,
                          const unsigned int n,
                          const unsigned int* tableX,
                          const unsigned int* tableY);
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
  outlinerhighprecisionreal indexToCoordinateX(unsigned int xIndex);
  outlinerhighprecisionreal indexToCoordinateY(unsigned int yIndex);
  bool processSceneCrossSections(unsigned int nCrossSections,
                                 struct ProcessorCrossSection* crossSections);
  bool processSceneCrossSection(unsigned int c,
                                const struct ProcessorCrossSection* crossSection);
  SvgCreator* createSvg(const char* svgFileName,
                        const HighPrecisionVector3D& svgBoundingBoxStart,
                        const HighPrecisionVector3D& svgBoundingBoxEnd,
                        enum outlinerdirection svgDirection);
  void createSvgCalculateSizes(const HighPrecisionVector3D& svgBoundingBoxStart,
                               const HighPrecisionVector3D& svgBoundingBoxEnd,
                               const outlinerhighprecisionreal stepx,
                               const outlinerhighprecisionreal stepy,
                               const enum outlinerdirection svgDirection,
                               outlinerhighprecisionreal& xOutputStart,
                               outlinerhighprecisionreal& xOutputEnd,
                               outlinerhighprecisionreal& yOutputStart,
                               outlinerhighprecisionreal& yOutputEnd,
                               outlinerhighprecisionreal& xSize,
                               outlinerhighprecisionreal& ySize,
                               unsigned int& xSizeInt,
                               unsigned int& ySizeInt,
                               outlinerhighprecisionreal& xFactor,
                               outlinerhighprecisionreal& yFactor);
};

#endif // PROCESSOR_HH

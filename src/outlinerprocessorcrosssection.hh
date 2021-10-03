
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
  outlinerhighprecisionreal x;
  outlinerhighprecisionreal y;
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class ProcessorCrossSection {

public:
  ProcessorCrossSection(const char* fileNameIn,
                        enum outlinerdirection sliceDirectionIn,
                        const HighPrecisionVector2D& lineStartIn,
                        const HighPrecisionVector2D& lineEndIn,
                        Processor& procIn);
  ~ProcessorCrossSection();
  bool processSceneCrossSection(const aiScene* scene);
  
private:

  const char* fileName;
  const enum outlinerdirection sliceDirection;
  const HighPrecisionVector2D lineStart;
  const HighPrecisionVector2D lineEnd;
  outlinerhighprecisionreal xDifference;
  outlinerhighprecisionreal yDifference;
  outlinerhighprecisionreal lineLength;
  outlinerhighprecisionreal lineStep;
  outlinerhighprecisionreal lineSteps;
  outlinerhighprecisionreal lineStepX;
  outlinerhighprecisionreal lineStepY;
  HighPrecisionVector2D sliceVerticalBoundingBoxStart;
  HighPrecisionVector2D sliceVerticalBoundingBoxEnd;
  Processor& proc;
  SvgCreator* svg;
  
  void deleteSvg(void);
  void calculateLineEquation(void);
  void sliceVerticalBoundingBox(const aiScene* scene,
                                HighPrecisionVector2D& sliceBoundingBoxStart,
                                HighPrecisionVector2D& sliceBoundingBoxEnd);
  void sliceVerticalBoundingBoxNode(const aiScene* scene,
                                    const aiNode* node,
                                    bool& set,
                                    HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                    HighPrecisionVector2D& sliceVerticalBoundingBoxEnd);
  void sliceVerticalBoundingBoxMesh(const aiScene* scene,
                                    const aiMesh* mesh,
                                    bool& set,
                                    HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                    HighPrecisionVector2D& sliceVerticalBoundingBoxEnd);
  void sliceVerticalBoundingBoxFace(const aiScene* scene,
                                    const aiMesh* mesh,
                                    const aiFace* face,
                                    outlinerhighprecisionreal x,
                                    outlinerhighprecisionreal y,
                                    bool& set,
                                    HighPrecisionVector2D& sliceVerticalBoundingBoxStart,
                                    HighPrecisionVector2D& sliceVerticalBoundingBoxEnd);
  void lineIteratorInit(struct ProcessorCrossSectionLineIterator& iter);
  bool lineIteratorDone(struct ProcessorCrossSectionLineIterator& iter);
  void lineIteratorNext(struct ProcessorCrossSectionLineIterator& iter);

};

#endif // PROCESSORCROSSECTION_HH

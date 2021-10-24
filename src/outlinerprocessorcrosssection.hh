
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
  HighPrecisionVector2D point;
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
                        const HighPrecisionVector2D& lineStartIn,
                        const HighPrecisionVector2D& lineEndIn,
                        outlinerhighprecisionreal stepzIn,
                        Processor& procIn);
  ~ProcessorCrossSection();
  bool processSceneCrossSection(const aiScene* scene);
  void getLineActualEndPoints(HighPrecisionVector2D& actualLineStart,
                              HighPrecisionVector2D& actualLineEnd,
                              outlinerhighprecisionreal extralineatends);
  
private:

  const unsigned int freespacearound = 2;
  const char* fileName;
  const char* label; // 0 if no label desired
  const enum outlinerdirection sliceDirection;
  const HighPrecisionVector2D lineStart;
  const HighPrecisionVector2D lineEnd;
  outlinerhighprecisionreal stepz;
  outlinerhighprecisionreal xDifference;
  outlinerhighprecisionreal yDifference;
  outlinerhighprecisionreal lineLength;
  outlinerhighprecisionreal lineStep;
  outlinerhighprecisionreal lineSteps;
  outlinerhighprecisionreal lineStepX;
  outlinerhighprecisionreal lineStepY;
  HighPrecisionVector2D sliceVerticalBoundingBoxStart;
  HighPrecisionVector2D sliceVerticalBoundingBoxEnd;
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
                            outlinerhighprecisionreal x,
                            outlinerhighprecisionreal y,
                            outlinerhighprecisionreal z);

  //
  // Coordinate operations
  //

  unsigned int coordinateLineStepToImageXIndex(unsigned int firstStepInBoundingBox,
                                               unsigned int currentStep);
  unsigned int coordinateZToImageYIndex(outlinerhighprecisionreal z);
  
  //
  // Line operations
  //
  
  void calculateLineEquation(void);
  outlinerhighprecisionreal calculateLineXBasedOnY(outlinerhighprecisionreal y);
  void lineIteratorInit(struct ProcessorCrossSectionLineIterator& iter);
  bool lineIteratorDone(struct ProcessorCrossSectionLineIterator& iter);
  void lineIteratorNext(struct ProcessorCrossSectionLineIterator& iter);

};

#endif // PROCESSORCROSSECTION_HH

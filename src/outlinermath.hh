
#ifndef OUTLINERMATH_HH
#define OUTLINERMATH_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerhighprecision.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void triangleBoundingBox2D(const aiVector2D& a,
                           const aiVector2D& b,
                           const aiVector2D& c,
                           HighPrecisionVector2D& boundingBoxStart,
                           HighPrecisionVector2D& boundingBoxEnd);
void triangleBoundingBox3D(const aiVector3D& a,
                           const aiVector3D& b,
                           const aiVector3D& c,
                           HighPrecisionVector3D& boundingBoxStart,
                           HighPrecisionVector3D& boundingBoxEnd);
bool pointInsideTriangle2D(const aiVector2D& a,
                           const aiVector2D& b,
                           const aiVector2D& c,
                           const HighPrecisionVector2D& point);
bool pointInsideBoundingBox2D(const HighPrecisionVector2D& boxStart,
                              const HighPrecisionVector2D& boxEnd,
                              const aiVector2D& point);
bool pointInsideBoundingBox2D(const HighPrecisionVector2D& boxStart,
                              const HighPrecisionVector2D& boxEnd,
                              const HighPrecisionVector2D& point);
bool boundingBoxIntersectsTriangle2D(const aiVector2D& a,
                                     const aiVector2D& b,
                                     const aiVector2D& c,
                                     const HighPrecisionVector2D& boxStart,
                                     const HighPrecisionVector2D& boxEnd);
bool boundingBoxesIntersect3D(HighPrecisionVector3D& boundingBox1Start,
                              HighPrecisionVector3D& boundingBox1End,
                              HighPrecisionVector3D& boundingBox2Start,
                              HighPrecisionVector3D& boundingBox2End);
bool pointOnLine2D(const aiVector2D* a,
                   const aiVector2D* b,
                   const HighPrecisionVector2D* point);
bool lineIntersectsVerticalLine2D(const aiVector2D& lineStart,
                                  const aiVector2D& lineEnd,
                                  const aiVector2D& verticalLineStart,
                                  const aiVector2D& verticalLineEnd,
                                  aiVector2D& intersectionPoint);
bool lineIntersectsHorizontalLine2D(const aiVector2D& lineStart,
                                    const aiVector2D& lineEnd,
                                    const aiVector2D& horizontalLineStart,
                                    const aiVector2D& horizontalLineEnd,
                                    aiVector2D& intersectionPoint);
bool vectorEqual(const aiVector2D* a,
                 const aiVector2D* b);
bool vectorEqual(const aiVector2D* a,
                 const HighPrecisionVector2D* b);
void vectorTo(const aiVector2D* from,
              const aiVector2D* to,
              HighPrecisionVector2D* result);
outlinerhighprecisionreal determinant2x2(const HighPrecisionVector2D* u,
                                         const HighPrecisionVector2D* v);
void mathTests(void);

#endif // OUTLINERMATH_HH

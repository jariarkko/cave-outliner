
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
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class OutlinerMath {

public:
  
  static bool boundingBoxEqual(const HighPrecisionVector2D& box1Start,
                               const HighPrecisionVector2D& box1End,
                               const HighPrecisionVector2D& box2Start,
                               const HighPrecisionVector2D& box2End);
  static void boundingBoxIntersection(const HighPrecisionVector2D& box1Start,
                                      const HighPrecisionVector2D& box1End,
                                      const HighPrecisionVector2D& box2Start,
                                      const HighPrecisionVector2D& box2End,
                                      HighPrecisionVector2D& resultBoxStart,
                                      HighPrecisionVector2D& resultBoxEnd);
  static void boundingBoxUnion(const HighPrecisionVector2D& box1Start,
                               const HighPrecisionVector2D& box1End,
                               const HighPrecisionVector2D& box2Start,
                               const HighPrecisionVector2D& box2End,
                               HighPrecisionVector2D& resultBoxStart,
                               HighPrecisionVector2D& resultBoxEnd);
  static void triangleDescribe(const HighPrecisionTriangle3D& triangle,
                               char* buf,
                               unsigned int bufSize);
  static void triangleBoundingBox2D(const HighPrecisionTriangle2D& triangle,
                                    HighPrecisionVector2D& boundingBoxStart,
                                    HighPrecisionVector2D& boundingBoxEnd);
  static void triangleBoundingBox3D(const HighPrecisionTriangle3D& triangle,
                                    HighPrecisionVector3D& boundingBoxStart,
                                    HighPrecisionVector3D& boundingBoxEnd);
  static bool pointInsideTriangle2D(const HighPrecisionTriangle2D& triangle,
                                    const HighPrecisionVector2D& point);
  static bool pointInsideBoundingBox2D(const HighPrecisionVector2D& boxStart,
                                       const HighPrecisionVector2D& boxEnd,
                                       const aiVector2D& point);
  static bool pointInsideBoundingBox2D(const HighPrecisionVector2D& boxStart,
                                       const HighPrecisionVector2D& boxEnd,
                                       const HighPrecisionVector2D& point);
  static bool boundingBoxIntersectsTriangle2D(const HighPrecisionTriangle2D& triangle,
                                              const HighPrecisionVector2D& boxStart,
                                              const HighPrecisionVector2D& boxEnd);
  static bool boundingBoxIntersectsTriangle3D(const HighPrecisionTriangle3D& triangle,
                                              const HighPrecisionVector3D& boxStart,
                                              const HighPrecisionVector3D& boxEnd);
  static bool boundingBoxesIntersect3D(HighPrecisionVector3D& boundingBox1Start,
                                       HighPrecisionVector3D& boundingBox1End,
                                       HighPrecisionVector3D& boundingBox2Start,
                                       HighPrecisionVector3D& boundingBox2End);
  static bool pointOnLine2D(const aiVector2D& a,
                            const aiVector2D& b,
                            const aiVector2D& point);
  static bool pointOnLine2D(const HighPrecisionVector2D& a,
                            const HighPrecisionVector2D& b,
                            const HighPrecisionVector2D& point);
  static bool lineIntersectsVerticalLine2D(const aiVector2D& lineStart,
                                           const aiVector2D& lineEnd,
                                           const aiVector2D& verticalLineStart,
                                           const aiVector2D& verticalLineEnd,
                                           aiVector2D& intersectionPoint);
  static bool lineIntersectsHorizontalLine2D(const aiVector2D& lineStart,
                                             const aiVector2D& lineEnd,
                                             const aiVector2D& horizontalLineStart,
                                             const aiVector2D& horizontalLineEnd,
                                             aiVector2D& intersectionPoint);
  static bool vectorEqual(const aiVector2D& a,
                          const aiVector2D& b);
  static bool vectorEqual(const HighPrecisionVector2D& a,
                          const HighPrecisionVector2D& b);
  static void vectorTo(const HighPrecisionVector2D& from,
                       const HighPrecisionVector2D& to,
                       HighPrecisionVector2D& result);
  static outlinerhighprecisionreal determinant2x2(const HighPrecisionVector2D& u,
                                                  const HighPrecisionVector2D& v);
  static void mathTests(void);

private:

  static void utilityTests(void);
  static void vectorTests(void);
  static void detTests(void);
  static void boundingBoxTests(void);
  static void boundingBoxEqualTests(void);
  static void triangleBoundingBoxTests(void);
  static void boundingBoxIntersectionTests(void);
  static void boundingBoxUnionTests(void);
  static void pointTests(void);
  static void lineTests(void);
  static void lineIntersectionTests(void);
  static void triangleTests(void);
  static void sortVectorsX2D(const HighPrecisionVector2D* a,
                             const HighPrecisionVector2D* b,
                             const HighPrecisionVector2D* c,
                             const HighPrecisionVector2D** nth0,
                             const HighPrecisionVector2D** nth1,
                             const HighPrecisionVector2D** nth2);
  static void sortVectorsY2D(const HighPrecisionVector2D* a,
                             const HighPrecisionVector2D* b,
                             const HighPrecisionVector2D* c,
                             const HighPrecisionVector2D** nth0,
                             const HighPrecisionVector2D** nth1,
                             const HighPrecisionVector2D** nth2);
  static void sortVectorsX3D(const HighPrecisionVector3D* a,
                             const HighPrecisionVector3D* b,
                             const HighPrecisionVector3D* c,
                             const HighPrecisionVector3D** nth0,
                             const HighPrecisionVector3D** nth1,
                             const HighPrecisionVector3D** nth2);
  static void sortVectorsY3D(const HighPrecisionVector3D* a,
                             const HighPrecisionVector3D* b,
                             const HighPrecisionVector3D* c,
                             const HighPrecisionVector3D** nth0,
                             const HighPrecisionVector3D** nth1,
                             const HighPrecisionVector3D** nth2);
  static void sortVectorsZ3D(const HighPrecisionVector3D* a,
                             const HighPrecisionVector3D* b,
                             const HighPrecisionVector3D* c,
                             const HighPrecisionVector3D** nth0,
                             const HighPrecisionVector3D** nth1,
                             const HighPrecisionVector3D** nth2);
  
};

#endif // OUTLINERMATH_HH


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
  
  static bool boundingBoxEqual(const OutlinerVector2D& box1Start,
                               const OutlinerVector2D& box1End,
                               const OutlinerVector2D& box2Start,
                               const OutlinerVector2D& box2End);
  static void boundingBoxIntersection(const OutlinerVector2D& box1Start,
                                      const OutlinerVector2D& box1End,
                                      const OutlinerVector2D& box2Start,
                                      const OutlinerVector2D& box2End,
                                      OutlinerVector2D& resultBoxStart,
                                      OutlinerVector2D& resultBoxEnd);
  static void boundingBoxUnion(const OutlinerVector2D& box1Start,
                               const OutlinerVector2D& box1End,
                               const OutlinerVector2D& box2Start,
                               const OutlinerVector2D& box2End,
                               OutlinerVector2D& resultBoxStart,
                               OutlinerVector2D& resultBoxEnd);
  static void triangleDescribe(const OutlinerTriangle3D& triangle,
                               char* buf,
                               unsigned int bufSize);
  static void triangleBoundingBox2D(const OutlinerTriangle2D& triangle,
                                    OutlinerVector2D& boundingBoxStart,
                                    OutlinerVector2D& boundingBoxEnd);
  static void triangleBoundingBox3D(const OutlinerTriangle3D& triangle,
                                    OutlinerVector3D& boundingBoxStart,
                                    OutlinerVector3D& boundingBoxEnd);
  static bool pointInsideTriangle2D(const OutlinerTriangle2D& triangle,
                                    const OutlinerVector2D& point);
  static bool pointInsideBoundingBox2D(const OutlinerVector2D& boxStart,
                                       const OutlinerVector2D& boxEnd,
                                       const aiVector2D& point);
  static bool pointInsideBoundingBox2D(const OutlinerVector2D& boxStart,
                                       const OutlinerVector2D& boxEnd,
                                       const OutlinerVector2D& point);
  static bool boundingBoxIntersectsTriangle2D(const OutlinerTriangle2D& triangle,
                                              const OutlinerVector2D& boxStart,
                                              const OutlinerVector2D& boxEnd);
  static bool boundingBoxIntersectsTriangle3D(const OutlinerTriangle3D& triangle,
                                              const OutlinerVector3D& boxStart,
                                              const OutlinerVector3D& boxEnd);
  static bool boundingBoxesIntersect3D(OutlinerVector3D& boundingBox1Start,
                                       OutlinerVector3D& boundingBox1End,
                                       OutlinerVector3D& boundingBox2Start,
                                       OutlinerVector3D& boundingBox2End);
  static bool pointOnLine2D(const aiVector2D& a,
                            const aiVector2D& b,
                            const aiVector2D& point);
  static bool pointOnLine2D(const OutlinerVector2D& a,
                            const OutlinerVector2D& b,
                            const OutlinerVector2D& point);
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
  static bool vectorEqual(const OutlinerVector2D& a,
                          const OutlinerVector2D& b);
  static void vectorTo(const OutlinerVector2D& from,
                       const OutlinerVector2D& to,
                       OutlinerVector2D& result);
  static outlinerreal determinant2x2(const OutlinerVector2D& u,
                                     const OutlinerVector2D& v);
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
  static void sortVectorsX2D(const OutlinerVector2D* a,
                             const OutlinerVector2D* b,
                             const OutlinerVector2D* c,
                             const OutlinerVector2D** nth0,
                             const OutlinerVector2D** nth1,
                             const OutlinerVector2D** nth2);
  static void sortVectorsY2D(const OutlinerVector2D* a,
                             const OutlinerVector2D* b,
                             const OutlinerVector2D* c,
                             const OutlinerVector2D** nth0,
                             const OutlinerVector2D** nth1,
                             const OutlinerVector2D** nth2);
  static void sortVectorsX3D(const OutlinerVector3D* a,
                             const OutlinerVector3D* b,
                             const OutlinerVector3D* c,
                             const OutlinerVector3D** nth0,
                             const OutlinerVector3D** nth1,
                             const OutlinerVector3D** nth2);
  static void sortVectorsY3D(const OutlinerVector3D* a,
                             const OutlinerVector3D* b,
                             const OutlinerVector3D* c,
                             const OutlinerVector3D** nth0,
                             const OutlinerVector3D** nth1,
                             const OutlinerVector3D** nth2);
  static void sortVectorsZ3D(const OutlinerVector3D* a,
                             const OutlinerVector3D* b,
                             const OutlinerVector3D* c,
                             const OutlinerVector3D** nth0,
                             const OutlinerVector3D** nth1,
                             const OutlinerVector3D** nth2);
  
};

#endif // OUTLINERMATH_HH

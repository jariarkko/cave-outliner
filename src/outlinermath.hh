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
  
  static bool boundingBoxEqual(const OutlinerBox2D& box1,
                               const OutlinerBox2D& box2);
  static void boundingBoxIntersection(const OutlinerBox2D& box1,
                                      const OutlinerBox2D& box2,
                                      OutlinerBox2D& resultBox);
  static void boundingBoxUnion(const OutlinerBox2D& box1,
                               const OutlinerBox2D& box2,
                               OutlinerBox2D& resultBox );
  static void triangleDescribe(const OutlinerTriangle3D& triangle,
                               char* buf,
                               unsigned int bufSize);
  static void triangleBoundingBox2D(const OutlinerTriangle2D& triangle,
                                    OutlinerBox2D& boundingBox);
  static void triangleBoundingBox3D(const OutlinerTriangle3D& triangle,
                                    OutlinerBox3D& boundingBox);
  static bool pointInsideTriangle2D(const OutlinerTriangle2D& triangle,
                                    const OutlinerVector2D& point);
  static bool pointInsideBoundingBox2D(const OutlinerBox2D& box,
                                       const OutlinerVector2D& point);
  static bool boundingBoxIntersectsTriangle2D(const OutlinerTriangle2D& triangle,
                                              const OutlinerBox2D& box);
  static bool boundingBoxIntersectsTriangle3D(const OutlinerTriangle3D& triangle,
                                              const OutlinerBox3D& box);
  static bool boundingBoxesIntersect3D(OutlinerBox3D& boundingBox1,
                                       OutlinerBox3D& boundingBox2);
  static bool pointOnLine2D(const OutlinerLine2D& line,
                            const OutlinerVector2D& point);
  static bool lineIntersectsVerticalLine2D(const OutlinerLine2D& line,
                                           const OutlinerLine2D& verticalLine,
                                           OutlinerVector2D& intersectionPoint);
  static bool lineIntersectsHorizontalLine2D(const OutlinerLine2D& line,
                                             const OutlinerLine2D& horizontalLine,
                                             OutlinerVector2D& intersectionPoint);
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

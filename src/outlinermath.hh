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

///
/// The OutlinerMath class is a geometry and mathematics module. It
/// hosts a number of static methods that can be used to calculate
/// bounding boxes for triangles, whether points are inside a
/// triangle, etc.
///

class OutlinerMath {

public:

  /// Produce a string that describes a triangle. Used in debugging.
  static void triangleDescribe(const OutlinerTriangle3D& triangle,
                               char* buf,
                               unsigned int bufSize,
                               bool full = 0); 
 
  /// Produce a string that describes a triangle. Used in debugging.
  static void triangleDescribe(const OutlinerTriangle2D& triangle,
                               char* buf,
                               unsigned int bufSize,
                               bool full = 0);

  /// Produce a string that describes a bounding box. Used in debugging.
  static void boxDescribe(const OutlinerBox3D& box,
                          char* buf,
                          unsigned int bufSize,
                          bool full = 0);
   
  /// Produce a string that describes a bounding box. Used in debugging.
  static void boxDescribe(const OutlinerBox2D& box,
                          char* buf,
                          unsigned int bufSize,
                          bool full = 0);
   
  /// Calculate the smallest bounding box that holds a given triangle (in a 2-dimension model).
  static void triangleBoundingBox2D(const OutlinerTriangle2D& triangle,
                                    OutlinerBox2D& boundingBox);

  /// Calculate the smallest bounding box that holds a given triangle (in a 3-dimension model).
  static void triangleBoundingBox3D(const OutlinerTriangle3D& triangle,
                                    OutlinerBox3D& boundingBox);

  /// Is a given point inside a triangle (in a 2-dimension model)?
  static bool pointInsideTriangle2D(const OutlinerTriangle2D& triangle,
                                    const OutlinerVector2D& point);

  /// Does a triangle intersect a bounding box?
  static bool boundingBoxIntersectsTriangle2D(const OutlinerTriangle2D& triangle,
                                              const OutlinerBox2D& box);

  /// Does a triangle intersect a bounding box?
  static bool boundingBoxIntersectsTriangle3D(const OutlinerTriangle3D& triangle,
                                              const OutlinerBox3D& box);

  /// Does a bounding box intersect another one?
  static bool boundingBoxesIntersect2D(const OutlinerBox2D& boundingBox1,
                                       const OutlinerBox2D& boundingBox2);

  /// Does a bounding box intersect another one?
  static bool boundingBoxesIntersect3D(const OutlinerBox3D& boundingBox1,
                                       const OutlinerBox3D& boundingBox2);

  /// Is a given point on a line?
  static bool pointOnLine2D(const OutlinerLine2D& line,
                            const OutlinerVector2D& point);
  
  /// Does a line intersect a vertical line?
  static bool lineIntersectsVerticalLine2D(const OutlinerLine2D& line,
                                           const OutlinerLine2D& verticalLine,
                                           OutlinerVector2D& intersectionPoint);

  /// Does a line intersect a horizontal line?
  static bool lineIntersectsHorizontalLine2D(const OutlinerLine2D& line,
                                             const OutlinerLine2D& horizontalLine,
                                             OutlinerVector2D& intersectionPoint);

  /// Create a vector from "from" to "to", placing the result in "result".
  static void vectorTo(const OutlinerVector2D& from,
                       const OutlinerVector2D& to,
                       OutlinerVector2D& result);

  /// Run module tests.
  static void mathTests(void);

private:

  static outlinerreal determinant2x2(const OutlinerVector2D& u,
                                     const OutlinerVector2D& v);
  static void utilityTests(void);
  static void vectorTests(void);
  static void detTests(void);
  static void boundingBoxTests(void);
  static void boundingBoxIntersectionTests(void);
  static void triangleBoundingBoxTests(void);
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

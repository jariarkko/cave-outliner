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

#ifndef OUTLINERHIGHPRECISION_HH
#define OUTLINERHIGHPRECISION_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "outlinertypes.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Vectors ////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This object represents a "vector" to a point in 2D space.
///

class OutlinerVector2D {
public:

  /// The x coordinate of the point.
  outlinerreal x;
  
  /// The y coordinate of the point.
  outlinerreal y;

  /// Create an empty object, all values initialized to zero.
  OutlinerVector2D() { x = y = 0; }
  
  /// Make an exact copy of another similar object. (But with lower precision.)
  OutlinerVector2D(const aiVector2D& in) : x(in.x), y(in.y) {}
  
  /// Make an exact copy of another similar object.
  OutlinerVector2D(const OutlinerVector2D& in) : x(in.x), y(in.y) {}

  /// Make a vector based on the x,y coordinates.
  OutlinerVector2D(const outlinerreal xIn,
                   const outlinerreal yIn) : x(xIn), y(yIn) {}

  /// Assignment
  OutlinerVector2D& operator=(const OutlinerVector2D& in) { x = in.x; y = in.y; return(*this); }
  
  /// Test for equality.
  bool equal(const OutlinerVector2D& b) const;

  /// Run unit tests associated with this class.
  static void test(void);
};

///
/// This object represents a "vector" to a point in 3D space.
///

class OutlinerVector3D {
public:
  
  /// The x coordinate of the point.
  
  outlinerreal x;
  
  /// The y coordinate of the point.
  outlinerreal y;
  
  /// The z coordinate of the point.
  outlinerreal z;

  /// Create an empty object, all values initialized to zero.
  OutlinerVector3D() { x = y = 0; z = 0; }
  
  /// Make an exact copy of another similar object. (But with lower precision.)
  OutlinerVector3D(const aiVector3D& in) : x(in.x), y(in.y), z(in.z) {}
  
  /// Make an exact copy of another similar object.
  OutlinerVector3D(const OutlinerVector3D& in) : x(in.x), y(in.y), z(in.z) {}
  
  /// Make a vector based on the x,y,z coordinates.
  OutlinerVector3D(const outlinerreal xIn,
                   const outlinerreal yIn,
                   const outlinerreal zIn) : x(xIn), y(yIn), z(zIn) {}
  
  /// Assignment
  OutlinerVector3D& operator=(const OutlinerVector3D& in) { x = in.x; y = in.y; z = in.z; return(*this); }
  
  /// Test for equality.
  bool equal(const OutlinerVector3D& b) const;

  /// Run unit tests associated with this class.
  static void test(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Lines //////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This object represents a "line" between two points in 2D space.
///

class OutlinerLine2D {
  
public:

  /// One end of the line.
  OutlinerVector2D start;
  
  /// Another end of the line.
  OutlinerVector2D end;
  
  /// Create an empty object, all values initialized to zero.
  OutlinerLine2D() {}
  
  /// Make an exact copy of another similar object.
  OutlinerLine2D(const OutlinerLine2D& in) : start(in.start), end(in.end) {}
  
  /// Make a line based on given two points.
  OutlinerLine2D(const OutlinerVector2D& startIn,
                 const OutlinerVector2D& endIn) : start(startIn), end(endIn) {}

  /// Make a line based on given two points, both given as x,y
  /// coordinates, four arguments in total.
  OutlinerLine2D(const outlinerreal startX,
                 const outlinerreal startY,
                 const outlinerreal endX,
                 const outlinerreal endY) : start(startX,startY), end(endX,endY) {}

  /// Assignment.
  OutlinerLine2D& operator=(const OutlinerLine2D& in) { start = in.start; end = in.end; return(*this); }
  
  /// Is a given point on a line?
  bool pointOnLine(const OutlinerVector2D& point) const;
  
  /// Is the line horizontal (along x axis)?
  bool horizontal() const { return(start.y == end.y); }
  
  /// Is the line vertical (along y axis)?
  bool vertical() const { return(start.x == end.x); }
  
  /// Run unit tests associated with this class.
  static void test(void);
};

///
/// This object represents a "line" between two points in 3D space.
///

class OutlinerLine3D {
  
public:
  
  /// One end of the line.
  OutlinerVector3D start;
  
  /// Another end of the line.
  OutlinerVector3D end;
  
  /// Create an empty object, all values initialized to zero.
  OutlinerLine3D() {}
  
  /// Make an exact copy of another similar object.
  OutlinerLine3D(const OutlinerLine3D& in) : start(in.start), end(in.end) {}
  
  /// Make a line based on given two points.
  OutlinerLine3D(const OutlinerVector3D& startIn,
                 const OutlinerVector3D& endIn) : start(startIn), end(endIn) {}

  /// Make a line based on given two points, both given as x,y,z
  /// coordinates, six arguments in total.
  OutlinerLine3D(const outlinerreal startX,
                 const outlinerreal startY,
                 const outlinerreal startZ,
                 const outlinerreal endX,
                 const outlinerreal endY,
                 const outlinerreal endZ) : start(startX,startY,startZ), end(endX,endY,endZ) {}
  
  // Assignment.
  OutlinerLine3D& operator=(const OutlinerLine3D& in) { start = in.start; end = in.end; return(*this); }
  
  /// Run unit tests associated with this class.
  static void test(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Bounding boxes /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This object represents a 1-dimensional "bounding box", i.e., a numeric range.
///

class OutlinerBox1D {
  
public:

  OutlinerBox1D() : start(0), end(0) { }
  
  OutlinerBox1D(outlinerreal from,
                outlinerreal to) : start(from), end(to) { }
  
  /// Assignment
  OutlinerBox1D& operator=(const OutlinerBox1D& in) { start = in.start; end = in.end; return(*this); }

  /// Test for equality.
  bool equal(const OutlinerBox1D& box2) const;
  
  /// Is a given point inside a bounding box (in 2-dimension model)?
  bool pointInside(const outlinerreal point) const;
  
  /// Does a bounding box intersect another one?
  bool doesIntersect(const OutlinerBox1D& boundingBox2) const;
  
  /// Take an intersection of this and another box, placing the
  /// resulting bounding box in "resultBox".
  void intersection(const OutlinerBox1D& box2,
                    OutlinerBox1D& resultBox) const;
  
  /// Take a union of this and another box, placing the resulting
  /// bounding box in "resultBox".
  void boxUnion(const OutlinerBox1D& box2,
                OutlinerBox1D& resultBox) const;
  
  /// Run unit tests associated with this class.
  static void test(void);
  
  outlinerreal start;
  outlinerreal end;
};

///
/// This object represents a "bounding box", a rectangular area in 2D space.
///

class OutlinerBox2D {
  
public:

  /// Lower left corner of the bounding box.
  OutlinerVector2D start;
  
  /// Upper right corner of the bounding box.
  OutlinerVector2D end;

  /// Create an empty object, all values initialized to zero.
  OutlinerBox2D() {}
  
  /// Make an exact copy of another similar object.
  OutlinerBox2D(const OutlinerLine2D& in) : start(in.start), end(in.end) {}
  
  /// Make a bounding box based on given two points.
  OutlinerBox2D(const OutlinerVector2D& startIn,
                const OutlinerVector2D& endIn) : start(startIn), end(endIn) {}

  /// Make a line based on given two points, both given as x,y
  /// coordinates, four arguments in total.
  OutlinerBox2D(const outlinerreal startX,
                const outlinerreal startY,
                const outlinerreal endX,
                const outlinerreal endY) : start(startX,startY), end(endX,endY) {}

  /// Assignment
  OutlinerBox2D& operator=(const OutlinerBox2D& in) { start = in.start; end = in.end; return(*this); }
  
  /// Test for equality.
  bool equal(const OutlinerBox2D& box2) const;
  
  /// Is a given point inside a bounding box (in 2-dimension model)?
  bool pointInside(const OutlinerVector2D& point) const;
  
  /// Does a bounding box intersect another one?
  bool doesIntersect(const OutlinerBox2D& boundingBox2) const;

  /// Take an intersection of this and another box, placing the
  /// resulting bounding box in "resultBox".
  void intersection(const OutlinerBox2D& box2,
                    OutlinerBox2D& resultBox) const;
  
  /// Take a union of this and another box, placing the resulting
  /// bounding box in "resultBox".
  void boxUnion(const OutlinerBox2D& box2,
                OutlinerBox2D& resultBox) const;

  /// Run unit tests associated with this class.
  static void test(void);

private:
  
  /// Run unit tests associated with this class.
  static void testEqual(void);
  
  /// Run unit tests associated with this class.
  static void testUnion(void);
  
  /// Run unit tests associated with this class.
  static void testIntersectionTest(void);
  
  /// Run unit tests associated with this class.
  static void testDoIntersection(void);
};

///
/// This object represents a "bounding box", a cubic/rectangular area in 3D space.
///

class OutlinerBox3D {
  
public:
  
  /// Lower left corner of the bounding box/cube.
  OutlinerVector3D start;
  
  /// Upper right corner of the bounding box/cube.
  OutlinerVector3D end;

  /// Create an empty object, all values initialized to zero.
  OutlinerBox3D() {}
  
  /// Make an exact copy of another similar object.
  OutlinerBox3D(const OutlinerBox3D& in) : start(in.start), end(in.end) {}
  
  /// Make a bounding box based on given two points.
  OutlinerBox3D(const OutlinerVector3D& startIn,
                const OutlinerVector3D& endIn) : start(startIn), end(endIn) {}

  /// Make a line based on given two points, both given as x,y,z
  /// coordinates, six arguments in total.
  OutlinerBox3D(const outlinerreal startX,
                const outlinerreal startY,
                const outlinerreal startZ,
                const outlinerreal endX,
                const outlinerreal endY,
                const outlinerreal endZ) : start(startX,startY,startZ), end(endX,endY,endZ) {}
  
  /// Assignment
  OutlinerBox3D& operator=(const OutlinerBox3D& in) { start = in.start; end = in.end; return(*this); }

  /// Test for equality.
  bool equal(const OutlinerBox3D& box2) const;
  
  /// Does a bounding box intersect another one?
  bool doesIntersect(const OutlinerBox3D& boundingBox2) const;
  
  /// Take an intersection of this and another box, placing the
  /// resulting bounding box in "resultBox".
  void intersection(const OutlinerBox3D& box2,
                    OutlinerBox3D& resultBox) const;
  
  /// Take a union of this and another box, placing the resulting
  /// bounding box in "resultBox".
  void boxUnion(const OutlinerBox3D& box2,
                OutlinerBox3D& resultBox) const;
  
  /// Run unit tests associated with this class.
  static void test(void);

private:
  
  /// Run unit tests associated with this class.
  static void testEqual(void);
  
  /// Run unit tests associated with this class.
  static void testUnion(void);

  /// Run unit tests associated with this class.
  static void testIntersectionTest(void);
  
  /// Run unit tests associated with this class.
  static void testDoIntersection(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Triangles //////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This object represents a "triangle", an area in 2D space bordered
/// by three points and lines drawn between them.
///

class OutlinerTriangle2D {
  
public:

  /// One corner of the triangle.
  OutlinerVector2D a;
  
  /// Another corner of the triangle.
  OutlinerVector2D b;
  
  /// And yet another corner of the triangle.
  OutlinerVector2D c;

  /// Create an empty object, all values initialized to zero.
  OutlinerTriangle2D() { a.x = a.y = 0; b.x = b.y = 0; c.x = c.y = 0; }
  
  /// Make an exact copy of another similar object.
  OutlinerTriangle2D(const OutlinerTriangle2D& in) : a(in.a), b(in.b), c(in.c) {}

  /// Make a triangle based on three given points.
  OutlinerTriangle2D(const OutlinerVector2D& aIn,
                     const OutlinerVector2D& bIn,
                     const OutlinerVector2D& cIn) : a(aIn), b(bIn), c(cIn) {}

  /// Assignment.
  OutlinerTriangle2D& operator=(const OutlinerTriangle2D& in) { a = in.a; b = in.b; c = in.c; return(*this); }
  
  /// Run unit tests associated with this class.
  static void test(void);
};

///
/// This object represents a "triangle", an area in 3D space bordered
/// by three points and lines drawn between them.
///

class OutlinerTriangle3D {
  
public:
  
  /// One corner of the triangle.
  OutlinerVector3D a;
  
  /// Another corner of the triangle.
  OutlinerVector3D b;
  
  /// And yet another corner of the triangle.
  OutlinerVector3D c;

  /// Create an empty object, all values initialized to zero.
  OutlinerTriangle3D() { a.x = a.y = a.z = 0; b.x = b.y = b.z = 0; c.x = c.y = c.z = 0; }

  /// Make an exact copy of another similar object.
  OutlinerTriangle3D(const OutlinerTriangle3D& in) : a(in.a), b(in.b), c(in.c) {}
  
  /// Make a triangle based on three given points.
  OutlinerTriangle3D(const OutlinerVector3D& aIn,
                     const OutlinerVector3D& bIn,
                     const OutlinerVector3D& cIn) : a(aIn), b(bIn), c(cIn) {}

  /// Assignment.
  OutlinerTriangle3D& operator=(const OutlinerTriangle3D& in) { a = in.a; b = in.b; c = in.c; return(*this); }
  
  /// Run unit tests associated with this class.
  static void test(void);
};

#endif // OUTLINERHIGHPRECISION_HH

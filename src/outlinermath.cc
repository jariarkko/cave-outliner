
///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <assert.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinermath.hh"
#include "outlinerdebug.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static void vectorTests(void);
static void detTests(void);
static void boundingBoxTests(void);
static void triangleBoundingBoxTests(void);
static void pointTests(void);
static void lineTests(void);
static void triangleTests(void);
static void sortVectorsX2D(const aiVector2D* a,
                           const aiVector2D* b,
                           const aiVector2D* c,
                           const aiVector2D** nth0,
                           const aiVector2D** nth1,
                           const aiVector2D** nth2);
static void sortVectorsY2D(const aiVector2D* a,
                           const aiVector2D* b,
                           const aiVector2D* c,
                           const aiVector2D** nth0,
                           const aiVector2D** nth1,
                           const aiVector2D** nth2);
static void sortVectorsX3D(const aiVector3D* a,
                           const aiVector3D* b,
                           const aiVector3D* c,
                           const aiVector3D** nth0,
                           const aiVector3D** nth1,
                           const aiVector3D** nth2);
static void sortVectorsY3D(const aiVector3D* a,
                           const aiVector3D* b,
                           const aiVector3D* c,
                           const aiVector3D** nth0,
                           const aiVector3D** nth1,
                           const aiVector3D** nth2);
static void sortVectorsZ3D(const aiVector3D* a,
                           const aiVector3D* b,
                           const aiVector3D* c,
                           const aiVector3D** nth0,
                           const aiVector3D** nth1,
                           const aiVector3D** nth2);

 ///////////////////////////////////////////////////////////////////////////////////////////////
// Math functions /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void triangleBoundingBox2D(const aiVector2D& a,
                           const aiVector2D& b,
                           const aiVector2D& c,
                           HighPrecisionVector2D& boundingBoxStart,
                           HighPrecisionVector2D& boundingBoxEnd) {

  const aiVector2D* nth1;
  const aiVector2D* nth2;
  const aiVector2D* nth3;
  outlinerhighprecisionreal xStart;
  outlinerhighprecisionreal xEnd;
  outlinerhighprecisionreal yStart;
  outlinerhighprecisionreal yEnd;
  
  // Order the points a,b,c so that the one with smallest x comes first
  sortVectorsX2D(&a,&b,&c,&nth1,&nth2,&nth3);
  
  // Fill in xStart and xEnd
  xStart = nth1->x;
  xEnd = nth3->x;
  
  // Order the points a,b,c so that the one with smallest y comes first
  sortVectorsY2D(&a,&b,&c,&nth1,&nth2,&nth3);
  
  // Fill in yStart and yEnd
  yStart = nth1->y;
  yEnd = nth3->y;

  // Construct the result
  boundingBoxStart.x = xStart;
  boundingBoxStart.y = yStart;
  boundingBoxEnd.x = xEnd;
  boundingBoxEnd.y = yEnd;
}

void triangleBoundingBox3D(const aiVector3D& a,
                           const aiVector3D& b,
                           const aiVector3D& c,
                           HighPrecisionVector3D& boundingBoxStart,
                           HighPrecisionVector3D& boundingBoxEnd) {

  const aiVector3D* nth1;
  const aiVector3D* nth2;
  const aiVector3D* nth3;
  outlinerhighprecisionreal xStart;
  outlinerhighprecisionreal xEnd;
  outlinerhighprecisionreal yStart;
  outlinerhighprecisionreal yEnd;
  outlinerhighprecisionreal zStart;
  outlinerhighprecisionreal zEnd;
  
  // Order the points a,b,c so that the one with smallest x comes first
  sortVectorsX3D(&a,&b,&c,&nth1,&nth2,&nth3);
  
  // Fill in xStart and xEnd
  xStart = nth1->x;
  xEnd = nth3->x;
  
  // Order the points a,b,c so that the one with smallest y comes first
  sortVectorsY3D(&a,&b,&c,&nth1,&nth2,&nth3);
  
  // Fill in yStart and yEnd
  yStart = nth1->y;
  yEnd = nth3->y;

  // Order the points a,b,c so that the one with smallest z comes first
  sortVectorsZ3D(&a,&b,&c,&nth1,&nth2,&nth3);
  
  // Fill in zStart and zEnd
  zStart = nth1->z;
  zEnd = nth3->z;

  // Construct the result
  boundingBoxStart.x = xStart;
  boundingBoxStart.y = yStart;
  boundingBoxStart.z = zStart;
  boundingBoxEnd.x = xEnd;
  boundingBoxEnd.y = yEnd;
  boundingBoxEnd.z = zEnd;
}

static void
sortVectorsX2D(const aiVector2D* a,
               const aiVector2D* b,
               const aiVector2D* c,
               const aiVector2D** nth0,
               const aiVector2D** nth1,
               const aiVector2D** nth2) {

  // There are 6 permutations of three numbers. Simply test for each condition.
  if (a->x < b->x) {
    if (c->x < a->x) {
      *nth0 = c;
      *nth1 = a;
      *nth2 = b;
    } else if (c->x < b->x) {
      *nth0 = a;
      *nth1 = c;
      *nth2 = b;
    } else {
      *nth0 = a;
      *nth1 = b;
      *nth2 = c;
    }
  } else {
    if (c->x < b->x) {
      *nth0 = c;
      *nth1 = b;
      *nth2 = a;
    } else if (c->x < a->x) {
      *nth0 = b;
      *nth1 = c;
      *nth2 = a;
    } else {
      *nth0 = b;
      *nth1 = a;
      *nth2 = c;
    }
  }
}

static void
sortVectorsY2D(const aiVector2D* a,
               const aiVector2D* b,
               const aiVector2D* c,
               const aiVector2D** nth0,
               const aiVector2D** nth1,
               const aiVector2D** nth2) {
  // There are 6 permutations of three numbers. Simply test for each condition.
  if (a->y < b->y) {
    if (c->y < a->y) {
      *nth0 = c;
      *nth1 = a;
      *nth2 = b;
    } else if (c->y < b->y) {
      *nth0 = a;
      *nth1 = c;
      *nth2 = b;
    } else {
      *nth0 = a;
      *nth1 = b;
      *nth2 = c;
    }
  } else {
    if (c->y < b->y) {
      *nth0 = c;
      *nth1 = b;
      *nth2 = a;
    } else if (c->y < a->y) {
      *nth0 = b;
      *nth1 = c;
      *nth2 = a;
    } else {
      *nth0 = b;
      *nth1 = a;
      *nth2 = c;
    }
  }
}

static void
sortVectorsX3D(const aiVector3D* a,
               const aiVector3D* b,
               const aiVector3D* c,
               const aiVector3D** nth0,
               const aiVector3D** nth1,
               const aiVector3D** nth2) {

  // There are 6 permutations of three numbers. Simply test for each condition.
  if (a->x < b->x) {
    if (c->x < a->x) {
      *nth0 = c;
      *nth1 = a;
      *nth2 = b;
    } else if (c->x < b->x) {
      *nth0 = a;
      *nth1 = c;
      *nth2 = b;
    } else {
      *nth0 = a;
      *nth1 = b;
      *nth2 = c;
    }
  } else {
    if (c->x < b->x) {
      *nth0 = c;
      *nth1 = b;
      *nth2 = a;
    } else if (c->x < a->x) {
      *nth0 = b;
      *nth1 = c;
      *nth2 = a;
    } else {
      *nth0 = b;
      *nth1 = a;
      *nth2 = c;
    }
  }
}

static void
sortVectorsY3D(const aiVector3D* a,
               const aiVector3D* b,
               const aiVector3D* c,
               const aiVector3D** nth0,
               const aiVector3D** nth1,
               const aiVector3D** nth2) {
  // There are 6 permutations of three numbers. Simply test for each condition.
  if (a->y < b->y) {
    if (c->y < a->y) {
      *nth0 = c;
      *nth1 = a;
      *nth2 = b;
    } else if (c->y < b->y) {
      *nth0 = a;
      *nth1 = c;
      *nth2 = b;
    } else {
      *nth0 = a;
      *nth1 = b;
      *nth2 = c;
    }
  } else {
    if (c->y < b->y) {
      *nth0 = c;
      *nth1 = b;
      *nth2 = a;
    } else if (c->y < a->y) {
      *nth0 = b;
      *nth1 = c;
      *nth2 = a;
    } else {
      *nth0 = b;
      *nth1 = a;
      *nth2 = c;
    }
  }
}

static void
sortVectorsZ3D(const aiVector3D* a,
               const aiVector3D* b,
               const aiVector3D* c,
               const aiVector3D** nth0,
               const aiVector3D** nth1,
               const aiVector3D** nth2) {
  // There are 6 permutations of three numbers. Simply test for each condition.
  if (a->z < b->z) {
    if (c->z < a->z) {
      *nth0 = c;
      *nth1 = a;
      *nth2 = b;
    } else if (c->z < b->z) {
      *nth0 = a;
      *nth1 = c;
      *nth2 = b;
    } else {
      *nth0 = a;
      *nth1 = b;
      *nth2 = c;
    }
  } else {
    if (c->z < b->z) {
      *nth0 = c;
      *nth1 = b;
      *nth2 = a;
    } else if (c->z < a->z) {
      *nth0 = b;
      *nth1 = c;
      *nth2 = a;
    } else {
      *nth0 = b;
      *nth1 = a;
      *nth2 = c;
    }
  }
}

bool
pointOnLine2D(const aiVector2D* a,
              const aiVector2D* b,
              const aiVector2D* point) {
  HighPrecisionVector2D tmp(*point);
  return(pointOnLine2D(a,b,&tmp));
}

bool
pointOnLine2D(const aiVector2D* a,
              const aiVector2D* b,
              const HighPrecisionVector2D* point) {
  
  // Check for a special case: line points are equal, resulting in
  // comparing to a point, not a line.
  
  if (vectorEqual(a,b))  {
    return(vectorEqual(a,point));
  }

  // Check for a special case: line is horizontal
  if (a->y == b->y) {
    if (point->y != a->y) return(0);
    return(a->x <= point->x && point->x <= b->x);
  }
  
  // Check for a special case: line is vertical
  if (a->x == b->x) {
    if (point->x != a->x) return(0);
    return(a->y <= point->y && point->y <= b->y);
  }
  
  // Not a special case. Run the general check, taking algorithm from
  // https://stackoverflow.com/questions/17692922/check-is-a-point-x-y-is-between-two-points-drawn-on-a-straight-line

  float alpha1 = (point->x - a->x) / (b->x - a->x);
  float alpha2 = (point->y - a->y) / (b->y - a->y);
  if (alpha1 != alpha2) return(0);
  if (alpha1 < 0) return(0);
  if (alpha1 > 1) return(0);
  return(1);
}

bool
pointInsideTriangle2D(const aiVector2D& triangleA,
                      const aiVector2D& triangleB,
                      const aiVector2D& triangleC,
                      const HighPrecisionVector2D& point) {

  // Check for a special case: all points are equal (resulting in
  // comparing to a point, not a triangle).
  if (vectorEqual(&triangleA,&triangleB) && vectorEqual(&triangleA,&triangleC)) {
    return(vectorEqual(&triangleA,&point));
  }
  
  // Check for a special case: triangle collapses to a line (at least
  // in 2D).
  if (vectorEqual(&triangleA,&triangleB)) {
    return(pointOnLine2D(&triangleA,&triangleC,&point));
  } else if (vectorEqual(&triangleA,&triangleC)) {
    return(pointOnLine2D(&triangleA,&triangleB,&point));
  } else if (vectorEqual(&triangleB,&triangleC)) {
    return(pointOnLine2D(&triangleA,&triangleB,&point));
  }
  
  // Not a special case. For the general case, we take the algorithm
  // from https://mathworld.wolfram.com/TriangleInterior.html
  
  HighPrecisionVector2D v = point; 
  HighPrecisionVector2D v0(triangleA);
  HighPrecisionVector2D v1; vectorTo(&triangleA,&triangleB,&v1);
  HighPrecisionVector2D v2; vectorTo(&triangleA,&triangleC,&v2);
  deepdebugf("triangle v = (%.2f,%.2f)", v.x, v.y);
  deepdebugf("triangle v0 = (%.2f,%.2f)", v0.x, v0.y);
  deepdebugf("triangle v1 = (%.2f,%.2f)", v1.x, v1.y);
  deepdebugf("triangle v2 = (%.2f,%.2f)", v2.x, v2.y);
  outlinerhighprecisionreal a = (determinant2x2(&v,&v2) - determinant2x2(&v0,&v2)) / determinant2x2(&v1,&v2);
  outlinerhighprecisionreal b = -((determinant2x2(&v,&v1) - determinant2x2(&v0,&v1)) / determinant2x2(&v1,&v2));
  deepdebugf("triangle (%.1f,%.1f)-(%.1f,%.1f)-(%.1f,%.1f) check a %.2f b %.2f a+b %.2f",
             triangleA.x, triangleA.y,
             triangleB.x, triangleB.y,
             triangleC.x, triangleC.y,
             a, b, a+b);
  return(a >= 0 && b >= 0 && a+b <= 1);
}

bool
pointInsideBoundingBox2D(const HighPrecisionVector2D& boxStart,
                         const HighPrecisionVector2D& boxEnd,
                         const aiVector2D& point) {
  return(point.x >= boxStart.x && point.x <= boxEnd.x &&
         point.y >= boxStart.y && point.y <= boxEnd.y);
}

bool
pointInsideBoundingBox2D(const HighPrecisionVector2D& boxStart,
                         const HighPrecisionVector2D& boxEnd,
                         const HighPrecisionVector2D& point) {
  return(point.x >= boxStart.x && point.x <= boxEnd.x &&
         point.y >= boxStart.y && point.y <= boxEnd.y);
}

bool
boundingBoxIntersectsTriangle2D(const aiVector2D& a,
                                const aiVector2D& b,
                                const aiVector2D& c,
                                const HighPrecisionVector2D& boxStart,
                                const HighPrecisionVector2D& boxEnd) {
  
  // First, if triangle corners are in the box, they intersect
  if (pointInsideBoundingBox2D(boxStart,boxEnd,a)) return(1);
  if (pointInsideBoundingBox2D(boxStart,boxEnd,b)) return(1);
  if (pointInsideBoundingBox2D(boxStart,boxEnd,c)) return(1);

  // Otherwise, (for now just an approximation) check if the box corners or middle are in the triangle
  HighPrecisionVector2D boxUpperRight(boxEnd.x,boxStart.y);
  HighPrecisionVector2D boxLowerLeft(boxStart.x,boxEnd.y);
  HighPrecisionVector2D boxMiddle((boxStart.x + boxEnd.x) / 2,(boxStart.y + boxEnd.y) / 2);
  if (pointInsideTriangle2D(a,b,c,boxStart)) return(1);
  if (pointInsideTriangle2D(a,b,c,boxEnd)) return(1);
  if (pointInsideTriangle2D(a,b,c,boxUpperRight)) return(1);
  if (pointInsideTriangle2D(a,b,c,boxLowerLeft)) return(1);
  if (pointInsideTriangle2D(a,b,c,boxMiddle)) return(1);
  
  // Otherwise no
  return(0);
}

bool
vectorEqual(const aiVector2D* a,
            const aiVector2D* b) {
  return(a->x == b->x && a->y == b->y);
}

bool
vectorEqual(const aiVector2D* a,
            const HighPrecisionVector2D* b) {
  return(a->x == b->x && a->y == b->y);
}

void
vectorTo(const aiVector2D* from,
         const aiVector2D* to,
         HighPrecisionVector2D* result) {
  *result = *to;
  result->x -= from->x;
  result->y -= from->y;
}

outlinerhighprecisionreal
determinant2x2(const HighPrecisionVector2D* u,
               const HighPrecisionVector2D* v) {
  return(u->x * v->y - u->y * v->x);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Test functions /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
mathTests(void) {
  infof("running math tests");
  vectorTests();
  detTests();
  pointTests();
  lineTests();
  triangleTests();
  boundingBoxTests();
  triangleBoundingBoxTests();
  infof("math tests ok");
}

static void
vectorTests(void) {
  aiVector2D a(2,2);
  aiVector2D b(3,3);
  HighPrecisionVector2D result;
  vectorTo(&a,&b,&result);
  deepdebugf("vector test: result: (%f,%f)", result.x, result.y);
  assert(result.x == 1);
  assert(result.y == 1);
}

static void
detTests(void) {
  HighPrecisionVector2D C1(4,2);
  HighPrecisionVector2D C2(1,3);
  outlinerhighprecisionreal result = determinant2x2(&C1,&C2);
  deepdebugf("determinant result = %.2f", result);
  assert(result == 10);
}

static void
boundingBoxTests(void) {
  aiVector2D a(0,0);
  aiVector2D b(0,3);
  aiVector2D c(2,0);
  HighPrecisionVector2D boundingBoxStart;
  HighPrecisionVector2D boundingBoxEnd;
  debugf("bounding box tests");
  triangleBoundingBox2D(a,a,a,boundingBoxStart,boundingBoxEnd);
  debugf("a,a,a bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBoxStart.x, boundingBoxStart.y, boundingBoxEnd.x, boundingBoxEnd.y);
  assert(boundingBoxStart.x == 0 && boundingBoxStart.y == 0);
  assert(boundingBoxEnd.x == 0 && boundingBoxEnd.y == 0);
  triangleBoundingBox2D(a,b,c,boundingBoxStart,boundingBoxEnd);
  debugf("a,b,c bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBoxStart.x, boundingBoxStart.y, boundingBoxEnd.x, boundingBoxEnd.y);
  assert(boundingBoxStart.x == 0 && boundingBoxStart.y == 0);
  assert(boundingBoxEnd.x == 2 && boundingBoxEnd.y == 3);
  triangleBoundingBox2D(c,b,a,boundingBoxStart,boundingBoxEnd);
  debugf("c,b,a bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBoxStart.x, boundingBoxStart.y, boundingBoxEnd.x, boundingBoxEnd.y);
  assert(boundingBoxStart.x == 0 && boundingBoxStart.y == 0);
  assert(boundingBoxEnd.x == 2 && boundingBoxEnd.y == 3);
  aiVector2D x(-10,-10);
  aiVector2D y(10,10);
  aiVector2D z(30,9);
  triangleBoundingBox2D(z,y,x,boundingBoxStart,boundingBoxEnd);
  debugf("z,y,x bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBoxStart.x, boundingBoxStart.y, boundingBoxEnd.x, boundingBoxEnd.y);
  assert(boundingBoxStart.x == -10 && boundingBoxStart.y == -10);
  assert(boundingBoxEnd.x == 30 && boundingBoxEnd.y == 10);
  triangleBoundingBox2D(y,z,x,boundingBoxStart,boundingBoxEnd);
  debugf("y,z,x bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBoxStart.x, boundingBoxStart.y, boundingBoxEnd.x, boundingBoxEnd.y);
  assert(boundingBoxStart.x == -10 && boundingBoxStart.y == -10);
  assert(boundingBoxEnd.x == 30 && boundingBoxEnd.y == 10);

  HighPrecisionVector2D bbtest1start(10,10);
  HighPrecisionVector2D bbtest1end(20,30);
  HighPrecisionVector2D bbtest1point1(0,25);
  HighPrecisionVector2D bbtest1point2(10,10);
  HighPrecisionVector2D bbtest1point3(20,30);
  HighPrecisionVector2D bbtest1point4(30,30);
  HighPrecisionVector2D bbtest1point5(11,15);
  bool ans = pointInsideBoundingBox2D(bbtest1start,bbtest1end,bbtest1point1);
  assert(ans == 0);
  ans = pointInsideBoundingBox2D(bbtest1start,bbtest1end,bbtest1point2);
  assert(ans == 1);
  ans = pointInsideBoundingBox2D(bbtest1start,bbtest1end,bbtest1point3);
  assert(ans == 1);
  ans = pointInsideBoundingBox2D(bbtest1start,bbtest1end,bbtest1point4);
  assert(ans == 0);
  ans = pointInsideBoundingBox2D(bbtest1start,bbtest1end,bbtest1point5);
  assert(ans == 1);
}

static void
pointTests(void) {
  aiVector2D a(1,1);
  aiVector2D b(1,2);
  aiVector2D c(1,1);
  assert(vectorEqual(&a,&b) == 0);
  assert(vectorEqual(&b,&c) == 0);
  assert(vectorEqual(&a,&c) == 1);
}

static void
lineTests(void) {

  // Horizontal
  {
    aiVector2D a(0,0);
    aiVector2D b(1,0);
    aiVector2D c(2,0);
    HighPrecisionVector2D d(0.5,2);
    bool ans = pointOnLine2D(&a,&c,&d);
    assert(ans == 0);
    ans = pointOnLine2D(&a,&b,&c);
    assert(ans == 0);
    ans = pointOnLine2D(&a,&c,&b);
    assert(ans == 1);
  }
  
  // Vertical
  {
    aiVector2D a(0,0);
    aiVector2D b(0,1);
    aiVector2D c(0,2);
    HighPrecisionVector2D d(0.5,1);
    bool ans = pointOnLine2D(&a,&c,&d);
    assert(ans == 0);
    ans = pointOnLine2D(&a,&b,&c);
    assert(ans == 0);
    ans = pointOnLine2D(&a,&c,&b);
    assert(ans == 1);
  }
  
  // Sloping line
  {
    aiVector2D a(0,0);
    aiVector2D b(1,1);
    aiVector2D c(2,2);
    aiVector2D d(1,2);
    bool ans = pointOnLine2D(&a,&c,&d);
    assert(ans == 0);
    ans = pointOnLine2D(&a,&b,&c);
    assert(ans == 0);
    ans = pointOnLine2D(&a,&c,&b);
    assert(ans == 1);
  }
}

static void
triangleTests(void) {
  aiVector2D a(0,0);
  aiVector2D b(0,2);
  aiVector2D c(2,0);
  HighPrecisionVector2D pointfar(2,2);
  HighPrecisionVector2D pointnear(0.5,0.5);
  HighPrecisionVector2D pointverynear(0.1,0.2);
  HighPrecisionVector2D pointata = a;
  HighPrecisionVector2D pointatb = b;
  HighPrecisionVector2D pointatc = c;
  HighPrecisionVector2D pointbeyondb = b;
  pointbeyondb.x += 0.01;
  pointbeyondb.y += 0.01;
  HighPrecisionVector2D pointbefore1(-0.001,0);
  HighPrecisionVector2D pointbefore2(0,-0.001);
  HighPrecisionVector2D pointbefore3(-0.001,-0.001);
  bool ansfar = pointInsideTriangle2D(a,b,c,pointfar);
  deepdebugf("triangle test: pointfar = %u", ansfar);
  bool ansnear = pointInsideTriangle2D(a,b,c,pointnear);
  deepdebugf("triangle test: pointnear = %u", ansnear);
  bool ansverynear = pointInsideTriangle2D(a,b,c,pointverynear);
  deepdebugf("triangle test: pointverynear = %u", ansverynear);
  bool ansata = pointInsideTriangle2D(a,b,c,pointata);
  deepdebugf("triangle test: pointata = %u", ansata);
  bool ansatb = pointInsideTriangle2D(a,b,c,pointatb);
  deepdebugf("triangle test: pointatb = %u", ansatb);
  bool ansatc = pointInsideTriangle2D(a,b,c,pointatc);
  deepdebugf("triangle test: pointatc = %u", ansatc);
  bool ansbeyondb = pointInsideTriangle2D(a,b,c,pointbeyondb);
  deepdebugf("triangle test: pointbeyondb = %u", ansbeyondb);
  bool ansbefore1 = pointInsideTriangle2D(a,b,c,pointbefore1);
  deepdebugf("triangle test: pointbefore1 = %u", ansbefore1);
  bool ansbefore2 = pointInsideTriangle2D(a,b,c,pointbefore2);
  deepdebugf("triangle test: pointbefore2 = %u", ansbefore2);
  bool ansbefore3 = pointInsideTriangle2D(a,b,c,pointbefore3);
  deepdebugf("triangle test: pointbefore3 = %u", ansbefore3);
  assert(ansfar == 0);
  assert(ansnear == 1);
  assert(ansverynear == 1);
  assert(ansata == 1);
  assert(ansatb == 1);
  assert(ansatc == 1);
  assert(ansbeyondb == 0);
  assert(ansbefore1 == 0);
  assert(ansbefore2 == 0);
  assert(ansbefore3 == 0);
}

static void
triangleBoundingBoxTests(void) {
  infof("triangle bounding box tests");
  aiVector2D a(0,0);
  aiVector2D b(0,10);
  aiVector2D c(20,0);
  HighPrecisionVector2D box1Start(-10,-10);
  HighPrecisionVector2D box1End(-1,-1);
  HighPrecisionVector2D box2Start(-10,-10);
  HighPrecisionVector2D box2End(5,5);
  HighPrecisionVector2D box3Start(-1000,-1000);
  HighPrecisionVector2D box3End(1000,1000);
  bool ans = boundingBoxIntersectsTriangle2D(a,b,c,box1Start,box1End);
  assert(!ans);
  ans = boundingBoxIntersectsTriangle2D(a,b,c,box2Start,box2End);
  assert(ans);
  ans = boundingBoxIntersectsTriangle2D(a,b,c,box3Start,box3End);
  assert(ans);
  infof("triangle bounding box tests ok");
}

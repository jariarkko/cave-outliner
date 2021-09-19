
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

static void utilityTests(void);
static void vectorTests(void);
static void detTests(void);
static void boundingBoxTests(void);
static void triangleBoundingBoxTests(void);
static void boundingBoxIntersectionTests(void);
static void pointTests(void);
static void lineTests(void);
static void lineIntersectionTests(void);
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
// Local macro utilities //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

# define betweenanyorder(a,b,c)    ((a) < (c) ? between(a,b,c) : between(c,b,a))
# define between(a,b,c)            ((a) <= (b) && (b) <= (c))
# define overlapanyorder(a,b,c,d)  (((a) <= (b) && (c) <= (d)) ? overlap(a,b,c,d) :         \
                                    (((b) <= (a) && (c) <= (d)) ? overlap(b,a,c,d) :        \
                                     (((b) <= (a) && (d) <= (c)) ? overlap(b,a,d,c) :       \
                                      overlap(a,b,d,c))))
# define overlap(a,b,c,d)          ((((a) <= (c)) && ((c) <= (b))) ||                       \
                                    (((c) <= (a)) && ((a) <= (d))))

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
boundingBoxesIntersect3D(HighPrecisionVector3D& boundingBox1Start,
                         HighPrecisionVector3D& boundingBox1End,
                         HighPrecisionVector3D& boundingBox2Start,
                         HighPrecisionVector3D& boundingBox2End) {
  // Following the algorithm from https://math.stackexchange.com/questions/2651710/simplest-way-to-determine-if-two-3d-boxes-intersect

  bool xOverlap = (between(boundingBox1Start.x,boundingBox2Start.x,boundingBox1End.x) ||
                   between(boundingBox1Start.x,boundingBox2End.x,boundingBox1End.x) ||
                   between(boundingBox2Start.x,boundingBox1Start.x,boundingBox2End.x) ||
                   between(boundingBox2Start.x,boundingBox1End.x,boundingBox2End.x));
  if (!xOverlap) return(0);
  bool yOverlap = (between(boundingBox1Start.y,boundingBox2Start.y,boundingBox1End.y) ||
                   between(boundingBox1Start.y,boundingBox2End.y,boundingBox1End.y) ||
                   between(boundingBox2Start.y,boundingBox1Start.y,boundingBox2End.y) ||
                   between(boundingBox2Start.y,boundingBox1End.y,boundingBox2End.y));
  if (!yOverlap) return(0);
  bool zOverlap = (between(boundingBox1Start.z,boundingBox2Start.z,boundingBox1End.z) ||
                   between(boundingBox1Start.z,boundingBox2End.z,boundingBox1End.z) ||
                   between(boundingBox2Start.z,boundingBox1Start.z,boundingBox2End.z) ||
                   between(boundingBox2Start.z,boundingBox1End.z,boundingBox2End.z));
  if (!zOverlap) return(0);
  return(1);
}

bool
lineIntersectsVerticalLine2D(const aiVector2D& lineStart,
                             const aiVector2D& lineEnd,
                             const aiVector2D& verticalLineStart,
                             const aiVector2D& verticalLineEnd,
                             aiVector2D& intersectionPoint) {
  assert(verticalLineStart.x == verticalLineEnd.x);

  // Fetch basic values
  outlinerhighprecisionreal verticalStartY = outlinermin(verticalLineStart.y,verticalLineEnd.y);
  outlinerhighprecisionreal verticalEndY = outlinermax(verticalLineStart.y,verticalLineEnd.y);
  outlinerhighprecisionreal verticalX = verticalLineStart.x;
  
  // Order line points such that X grows from start to end
  outlinerhighprecisionreal lineStartX;
  outlinerhighprecisionreal lineStartY;
  outlinerhighprecisionreal lineEndX;
  outlinerhighprecisionreal lineEndY;
  if (lineStart.x <= lineEnd.x) {
    lineStartX = lineStart.x;
    lineStartY = lineStart.y;
    lineEndX = lineEnd.x;
    lineEndY = lineEnd.y;
  } else {
    lineStartX = lineEnd.x;
    lineStartY = lineEnd.y;
    lineEndX = lineStart.x;
    lineEndY = lineStart.y;
  }
  
  // Calculate line equation
  //
  //    y(x) = lineStartY + (x - lineStartX) * (equationTotalDifferenceY / equationTotalDifferenceX)
  //
  outlinerhighprecisionreal equationBaseY = lineStartY;
  outlinerhighprecisionreal equationTotalDifferenceY = lineEndY - lineStartY; // positive or negative
  outlinerhighprecisionreal equationTotalDifferenceX = lineEndX - lineStartX; // positive

  // Check for the case of parallel lines
  if (equationTotalDifferenceX == 0) {
    if (lineStartX != verticalX) return(0);
    if (overlap(lineStartY,lineEndY,verticalStartY,verticalEndY)) {
      intersectionPoint.x = verticalX;
      if (lineStartY <= verticalStartY && verticalStartY <= lineEndY) intersectionPoint.y = verticalStartY;
      else if (verticalStartY <= lineStartY && lineStartY <= verticalEndY) intersectionPoint.y = lineStartY;
      else assert(0);
      return(1);
    }
    return(0);
  }

  // Continue calculating the line equation
  outlinerhighprecisionreal equationFactor = equationTotalDifferenceY / equationTotalDifferenceX; // positive or negative
  outlinerhighprecisionreal verticalLineDifferenceX = verticalX - lineStartX; // positive or negative

  // Check if vertical line is in the range on X
  if (!between(lineStartX,verticalX,lineEndX)) return(0);

  // Calculate resulting Y at vertical line position
  outlinerhighprecisionreal lineY = equationBaseY + verticalLineDifferenceX * equationFactor;

  // Check if the resulting position is within the vertical line Y range
  if (!between(verticalStartY,lineY,verticalEndY)) return(0);

  // Lines intersect! Set the intersection point
  intersectionPoint.x = verticalX;
  intersectionPoint.y = lineY;
  return(1);
}

bool
lineIntersectsHorizontalLine2D(const aiVector2D& lineStart,
                               const aiVector2D& lineEnd,
                               const aiVector2D& horizontalLineStart,
                               const aiVector2D& horizontalLineEnd,
                               aiVector2D& intersectionPoint) {
  assert(horizontalLineStart.y == horizontalLineEnd.y);
  
  // Fetch basic values
  outlinerhighprecisionreal horizontalStartX = outlinermin(horizontalLineStart.x,horizontalLineEnd.x);
  outlinerhighprecisionreal horizontalEndX = outlinermax(horizontalLineStart.x,horizontalLineEnd.x);
  outlinerhighprecisionreal horizontalY = horizontalLineStart.y;
  
  // Order line points such that Y grows from start to end
  outlinerhighprecisionreal lineStartX;
  outlinerhighprecisionreal lineStartY;
  outlinerhighprecisionreal lineEndX;
  outlinerhighprecisionreal lineEndY;
  if (lineStart.y <= lineEnd.y) {
    lineStartX = lineStart.x;
    lineStartY = lineStart.y;
    lineEndX = lineEnd.x;
    lineEndY = lineEnd.y;
  } else {
    lineStartX = lineEnd.x;
    lineStartY = lineEnd.y;
    lineEndX = lineStart.x;
    lineEndY = lineStart.y;
  }
  
  // Calculate line equation
  //
  //    x(y) = lineStartX + (y - lineStartY) * (equationTotalDifferenceX / equationTotalDifferenceY)
  //
  outlinerhighprecisionreal equationBaseX = lineStartX;
  outlinerhighprecisionreal equationTotalDifferenceX = lineEndX - lineStartX; // positive or negative
  outlinerhighprecisionreal equationTotalDifferenceY = lineEndY - lineStartY; // positive

  // Check for the case of parallel lines
  if (equationTotalDifferenceY == 0) {
    if (lineStartY != horizontalY) return(0);
    if (overlap(lineStartX,lineEndX,horizontalStartX,horizontalEndX)) {
      intersectionPoint.y = horizontalY;
      if (lineStartX <= horizontalStartX && horizontalStartX <= lineEndX) intersectionPoint.x = horizontalStartX;
      else if (horizontalStartX <= lineStartX && lineStartX <= horizontalEndX) intersectionPoint.x = lineStartX;
      else assert(0);
      return(1);
    }
    return(0);
  }

  // Continue calculating the line equation
  outlinerhighprecisionreal equationFactor = equationTotalDifferenceX / equationTotalDifferenceY; // positive or negative
  outlinerhighprecisionreal horizontalLineDifferenceY = horizontalY - lineStartY; // positive or negative

  // Check if horizontal line is in the range on Y
  if (!between(lineStartY,horizontalY,lineEndY)) return(0);

  // Calculate resulting X at horizontal line position
  outlinerhighprecisionreal lineX = equationBaseX + horizontalLineDifferenceY * equationFactor;

  // Check if the resulting position is within the horizontal line X range
  if (!between(horizontalStartX,lineX,horizontalEndX)) return(0);

  // Lines intersect! Set the intersection point
  intersectionPoint.x = lineX;
  intersectionPoint.y = horizontalY;
  return(1);
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
  utilityTests();
  vectorTests();
  detTests();
  pointTests();
  lineTests();
  lineIntersectionTests();
  triangleTests();
  boundingBoxTests();
  triangleBoundingBoxTests();
  boundingBoxIntersectionTests();
  infof("math tests ok");
}

static void
utilityTests(void) {

  debugf("utility tests...");
  // Between tests
  assert(between(1,2,3));
  assert(between(1,2,2));
  assert(!between(1,3,2));
  assert(!between(3,2,1));

  // Betweenanyorder tests
  assert(betweenanyorder(1,2,3));
  assert(betweenanyorder(1,2,2));
  assert(betweenanyorder(2,2,1));
  assert(!betweenanyorder(1,3,2));
  assert(!betweenanyorder(2,3,1));
  assert(betweenanyorder(3,2,1));

  // Overlap tests
  assert(overlap(1,2,2,4));
  assert(overlap(1,10,2,4));
  assert(overlap(2,3,0,10));
  assert(!overlap(1,2,3,4));
  assert(!overlap(3,4,1,2));

  // Overlapanyorder tests
  assert(overlapanyorder(1,2,2,4));
  assert(overlapanyorder(2,1,2,4));
  assert(overlapanyorder(2,1,4,2));
  assert(overlapanyorder(1,2,4,2));
  assert(overlapanyorder(1,10,4,2));
  assert(overlapanyorder(2,3,100,1));
  assert(!overlapanyorder(1,2,3,4));
  assert(!overlapanyorder(4,3,2,1));
}

static void
vectorTests(void) {
  debugf("vector tests...");
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
  debugf("det tests...");
  HighPrecisionVector2D C1(4,2);
  HighPrecisionVector2D C2(1,3);
  outlinerhighprecisionreal result = determinant2x2(&C1,&C2);
  deepdebugf("determinant result = %.2f", result);
  assert(result == 10);
}

static void
boundingBoxTests(void) {
  debugf("bounding box tests...");
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

static
void boundingBoxIntersectionTests(void) {
  infof("bounding box intersection tests...");
  
  HighPrecisionVector3D test1boundingBox1Start(0,0,0);
  HighPrecisionVector3D test1boundingBox1End(100,100,100);
  HighPrecisionVector3D test1boundingBox2Start(10,10,10);
  HighPrecisionVector3D test1boundingBox2End(11,11,11);
  bool ans = boundingBoxesIntersect3D(test1boundingBox1Start,
                                      test1boundingBox1End,
                                      test1boundingBox2Start,
                                      test1boundingBox2End);
  assert(ans);
  
  HighPrecisionVector3D test2boundingBox1Start(0,0,0);
  HighPrecisionVector3D test2boundingBox1End(10,10,10);
  HighPrecisionVector3D test2boundingBox2Start(11,11,11);
  HighPrecisionVector3D test2boundingBox2End(12,12,12);
  ans = boundingBoxesIntersect3D(test2boundingBox1Start,
                                 test2boundingBox1End,
                                 test2boundingBox2Start,
                                 test2boundingBox2End);
  assert(!ans);
  
  HighPrecisionVector3D test3boundingBox1Start(0,0,0);
  HighPrecisionVector3D test3boundingBox1End(10,10,10);
  HighPrecisionVector3D test3boundingBox2Start(0,0,11);
  HighPrecisionVector3D test3boundingBox2End(2,2,12);
  ans = boundingBoxesIntersect3D(test3boundingBox1Start,
                                 test3boundingBox1End,
                                 test3boundingBox2Start,
                                 test3boundingBox2End);
  assert(!ans);
  
  infof("bounding box intersection tests ok");
}

static void
pointTests(void) {
  debugf("point tests...");
  aiVector2D a(1,1);
  aiVector2D b(1,2);
  aiVector2D c(1,1);
  assert(vectorEqual(&a,&b) == 0);
  assert(vectorEqual(&b,&c) == 0);
  assert(vectorEqual(&a,&c) == 1);
}

static void
lineTests(void) {

  debugf("line tests...");
  
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
lineIntersectionTests(void) {

  debugf("line intersection tests...");
  
  // Vertical line intersection 1
  {
    aiVector2D a(10,10);
    aiVector2D b1(20,20);
    aiVector2D b2(20,15);
    aiVector2D vl1start(1,0);
    aiVector2D vl1end(1,40);
    aiVector2D vl2start(11,0);
    aiVector2D vl2end(11,40);
    aiVector2D inter;
    bool ans;
    ans = lineIntersectsVerticalLine2D(a,b1,vl1start,vl1end,inter);
    assert(!ans);
    ans = lineIntersectsVerticalLine2D(a,b1,vl2start,vl2end,inter);
    assert(ans);
    deepdebugf("vertical line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 11);
    assert(inter.y == 11);
    ans = lineIntersectsVerticalLine2D(a,b2,vl2start,vl2end,inter);
    assert(ans);
    deepdebugf("vertical line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 11);
    assert(inter.y == 10.5);
  }
  
  // Vertical line intersection 2
  {
    aiVector2D a(10,10);
    aiVector2D b(12,20);
    aiVector2D vl1start(11,0);
    aiVector2D vl1end(11,9);
    aiVector2D vl2start(11,0);
    aiVector2D vl2end(11,14);
    aiVector2D vl3start(11,10);
    aiVector2D vl3end(11,15);
    aiVector2D vl4start(11,10);
    aiVector2D vl4end(11,16);
    aiVector2D inter;
    bool ans;
    ans = lineIntersectsVerticalLine2D(a,b,vl1start,vl1end,inter);
    assert(!ans);
    ans = lineIntersectsVerticalLine2D(a,b,vl2start,vl2end,inter);
    assert(!ans);
    ans = lineIntersectsVerticalLine2D(a,b,vl3start,vl3end,inter);
    assert(ans);
    deepdebugf("vertical line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 11);
    assert(inter.y == 15);
    ans = lineIntersectsVerticalLine2D(a,b,vl4start,vl4end,inter);
    assert(ans);
    deepdebugf("vertical line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 11);
    assert(inter.y == 15);
  }
  
  // Vertical line intersection 3 (parallel lines)
  {
    aiVector2D a(10,10);
    aiVector2D b(10,20);
    aiVector2D vl1start(9,15);
    aiVector2D vl1end(9,16);
    aiVector2D vl2start(10,5);
    aiVector2D vl2end(10,9);
    aiVector2D vl3start(10,15);
    aiVector2D vl3end(10,16);
    aiVector2D inter;
    bool ans;
    ans = lineIntersectsVerticalLine2D(a,b,vl1start,vl1end,inter);
    assert(!ans);
    ans = lineIntersectsVerticalLine2D(a,b,vl2start,vl2end,inter);
    assert(!ans);
    ans = lineIntersectsVerticalLine2D(a,b,vl3start,vl3end,inter);
    assert(ans);
    deepdebugf("vertical parallel line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 10);
    assert(inter.y == 15);
  }
  
  // Horizontal line intersection 1
  {
    aiVector2D a(10,10);
    aiVector2D b(20,20);
    aiVector2D hl1start(0,10);
    aiVector2D hl1end(10,10);
    aiVector2D hl2start(0,15);
    aiVector2D hl2end(20,15);
    aiVector2D hl3start(0,21);
    aiVector2D hl3end(20,21);
    aiVector2D inter;
    bool ans;
    ans = lineIntersectsHorizontalLine2D(a,b,hl1start,hl1end,inter);
    assert(ans);
    deepdebugf("horizontal line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 10);
    assert(inter.y == 10);
    ans = lineIntersectsHorizontalLine2D(a,b,hl2start,hl2end,inter);
    assert(ans);
    deepdebugf("horizontal line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 15);
    assert(inter.y == 15);
    ans = lineIntersectsHorizontalLine2D(a,b,hl3start,hl3end,inter);
    assert(!ans);
  }
  
  // Vertical line intersection 2 (parallel lines)
  {
    aiVector2D a(10,10);
    aiVector2D b(20,10);
    aiVector2D hl1start(15,9);
    aiVector2D hl1end(16,9);
    aiVector2D hl2start(5,10);
    aiVector2D hl2end(9,10);
    aiVector2D hl3start(15,10);
    aiVector2D hl3end(16,10);
    aiVector2D inter;
    bool ans;
    ans = lineIntersectsHorizontalLine2D(a,b,hl1start,hl1end,inter);
    assert(!ans);
    ans = lineIntersectsHorizontalLine2D(a,b,hl2start,hl2end,inter);
    assert(!ans);
    ans = lineIntersectsHorizontalLine2D(a,b,hl3start,hl3end,inter);
    assert(ans);
    deepdebugf("horizontal parallel line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.y == 10);
    assert(inter.x == 15);
  }
  
}

static void
triangleTests(void) {
  debugf("triangle tests...");
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
  infof("triangle bounding box tests..");
  
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

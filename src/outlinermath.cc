
///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <cassert>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinermath.hh"
#include "outlinerdebug.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Debugs /////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
  
# define  debugreturn(f,why,x) {                                             \
    bool drv = (x);                                                          \
    deepdeepdebugf("%s returns %u due to %s", (f), drv, (why));              \
    return(drv); \
 }

///////////////////////////////////////////////////////////////////////////////////////////////
// Math functions /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
OutlinerMath::boundingBoxEqual(const OutlinerVector2D& box1Start,
                               const OutlinerVector2D& box1End,
                               const OutlinerVector2D& box2Start,
                               const OutlinerVector2D& box2End) {
  return(box1Start.x == box2Start.x &&
         box1Start.y == box2Start.y &&
         box1End.x == box2End.x &&
         box1End.y == box2End.y);
}

void
OutlinerMath::boundingBoxIntersection(const OutlinerVector2D& box1Start,
                                      const OutlinerVector2D& box1End,
                                      const OutlinerVector2D& box2Start,
                                      const OutlinerVector2D& box2End,
                                      OutlinerVector2D& resultBoxStart,
                                      OutlinerVector2D& resultBoxEnd) {
  resultBoxStart = box1Start;
  resultBoxEnd = box1End;
  if (box2Start.x > resultBoxStart.x) resultBoxStart.x = box2Start.x;
  if (box2Start.y > resultBoxStart.y) resultBoxStart.y = box2Start.y;
  if (box2End.x < resultBoxEnd.x) resultBoxEnd.x = box2End.x;
  if (box2End.y < resultBoxEnd.y) resultBoxEnd.y = box2End.y;
}

void
OutlinerMath::boundingBoxUnion(const OutlinerVector2D& box1Start,
                               const OutlinerVector2D& box1End,
                               const OutlinerVector2D& box2Start,
                               const OutlinerVector2D& box2End,
                               OutlinerVector2D& resultBoxStart,
                               OutlinerVector2D& resultBoxEnd) {
  resultBoxStart = box1Start;
  resultBoxEnd = box1End;
  if (box2Start.x < resultBoxStart.x) resultBoxStart.x = box2Start.x;
  if (box2Start.y < resultBoxStart.y) resultBoxStart.y = box2Start.y;
  if (box2End.x > resultBoxEnd.x) resultBoxEnd.x = box2End.x;
  if (box2End.y > resultBoxEnd.y) resultBoxEnd.y = box2End.y;
}

void
OutlinerMath::triangleDescribe(const OutlinerTriangle3D& triangle,
                               char* buf,
                               unsigned int bufSize) {
  assert(buf != 0);
  assert(bufSize > 0);
  memset(buf,0,bufSize);
  outlinerreal xlow = outlinermin3(triangle.a.x,triangle.b.x,triangle.c.x);
  outlinerreal xhigh = outlinermax3(triangle.a.x,triangle.b.x,triangle.c.x);
  outlinerreal ylow = outlinermin3(triangle.a.y,triangle.b.y,triangle.c.y);
  outlinerreal yhigh = outlinermax3(triangle.a.y,triangle.b.y,triangle.c.y);
  outlinerreal zlow = outlinermin3(triangle.a.z,triangle.b.z,triangle.c.z);
  outlinerreal zhigh = outlinermax3(triangle.a.z,triangle.b.z,triangle.c.z);
  if (triangle.a.z == triangle.b.z && triangle.a.z == triangle.c.z) {
    snprintf(buf,bufSize-1,"horizontal z %.2f", zlow);
  } else if (triangle.a.x == triangle.b.x && triangle.a.x == triangle.c.x) {
    snprintf(buf,bufSize-1,"vertical along y %.2f..%.2f, z %.2f..%.2f", ylow, yhigh, zlow, zhigh);
  } else if (triangle.a.y == triangle.b.y && triangle.a.y == triangle.c.y) {
    snprintf(buf,bufSize-1,"vertical along x %.2f..%.2f, z %.2f..%.2f", xlow, xhigh, zlow, zhigh);
  } else {
    snprintf(buf,bufSize-1,"irregular, z %.2f..%.2f", zlow, zhigh);
  }
}

void
OutlinerMath::triangleBoundingBox2D(const OutlinerTriangle2D& triangle,
                                    OutlinerVector2D& boundingBoxStart,
                                    OutlinerVector2D& boundingBoxEnd) {
  
  const OutlinerVector2D* nth1;
  const OutlinerVector2D* nth2;
  const OutlinerVector2D* nth3;
  outlinerreal xStart;
  outlinerreal xEnd;
  outlinerreal yStart;
  outlinerreal yEnd;
  
  // Order the points a,b,c so that the one with smallest x comes first
  sortVectorsX2D(&triangle.a,&triangle.b,&triangle.c,&nth1,&nth2,&nth3);
  
  // Fill in xStart and xEnd
  xStart = nth1->x;
  xEnd = nth3->x;
  
  // Order the points a,b,c so that the one with smallest y comes first
  sortVectorsY2D(&triangle.a,&triangle.b,&triangle.c,&nth1,&nth2,&nth3);
  
  // Fill in yStart and yEnd
  yStart = nth1->y;
  yEnd = nth3->y;

  // Construct the result
  boundingBoxStart.x = xStart;
  boundingBoxStart.y = yStart;
  boundingBoxEnd.x = xEnd;
  boundingBoxEnd.y = yEnd;
}

void
OutlinerMath::triangleBoundingBox3D(const OutlinerTriangle3D& triangle,
                                    OutlinerVector3D& boundingBoxStart,
                                    OutlinerVector3D& boundingBoxEnd) {

  const OutlinerVector3D* nth1;
  const OutlinerVector3D* nth2;
  const OutlinerVector3D* nth3;
  outlinerreal xStart;
  outlinerreal xEnd;
  outlinerreal yStart;
  outlinerreal yEnd;
  outlinerreal zStart;
  outlinerreal zEnd;
  
  // Order the points a,b,c so that the one with smallest x comes first
  sortVectorsX3D(&triangle.a,&triangle.b,&triangle.c,&nth1,&nth2,&nth3);
  
  // Fill in xStart and xEnd
  xStart = nth1->x;
  xEnd = nth3->x;
  
  // Order the points a,b,c so that the one with smallest y comes first
  sortVectorsY3D(&triangle.a,&triangle.b,&triangle.c,&nth1,&nth2,&nth3);
  
  // Fill in yStart and yEnd
  yStart = nth1->y;
  yEnd = nth3->y;

  // Order the points a,b,c so that the one with smallest z comes first
  sortVectorsZ3D(&triangle.a,&triangle.b,&triangle.c,&nth1,&nth2,&nth3);
  
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

bool
OutlinerMath::pointOnLine2D(const aiVector2D& a,
                            const aiVector2D& b,
                            const aiVector2D& point) {
  deepdeepdebugf("pointOnLine2D aiVector");
  OutlinerVector2D tmp(point);
  return(pointOnLine2D(a,b,tmp));
}

bool
OutlinerMath::pointOnLine2D(const OutlinerVector2D& a,
                            const OutlinerVector2D& b,
                            const OutlinerVector2D& point) {

  // Debugs
  deepdeepdebugf("          pointOnLine2D high precision (%.2f,%.2f)-(%.2f,%.2f) vs. (%.2f,%.2f)",
                 a.x, a.y, b.x, b.y,
                 point.x, point.y);
  
  // Check for a special case: line points are equal, resulting in
  // comparing to a point, not a line.
  
  if (vectorEqual(a,b))  {
    debugreturn("          pol2","line points equal",vectorEqual(a,point));
  }

  // Check for a special case: line is horizontal
  if (a.y == b.y) {
    if (point.y != a.y) debugreturn("          pol","horizontal y diff",0);
    debugreturn("          pol2","line is horizontal",outlinerbetweenanyorder(a.x,point.x,b.x));
    //a.x <= point.x && point.x <= b.x);
  }
  
  // Check for a special case: line is vertical
  if (a.x == b.x) {
    if (point.x != a.x) debugreturn("          pol2","vertical x diff",0);
    debugreturn("          pol2","line is vertical",outlinerbetweenanyorder(a.y,point.y,b.y));
    //a.y <= point.y && point.y <= b.y);
  }
  
  // Not a special case. Run the general check, taking algorithm from
  // https://stackoverflow.com/questions/17692922/check-is-a-point-x-y-is-between-two-points-drawn-on-a-straight-line

  float alpha1 = (point.x - a.x) / (b.x - a.x);
  float alpha2 = (point.y - a.y) / (b.y - a.y);
  if (alpha1 != alpha2) debugreturn("          pol2","alpha diff",0);
  if (alpha1 < 0) debugreturn("          pol2","alpha neg",0);
  if (alpha1 > 1) debugreturn("          pol2","alpha above one",0);
  debugreturn("          pol2","fallthrough",1);
}

bool
OutlinerMath::pointInsideTriangle2D(const OutlinerTriangle2D& triangle,
                                    const OutlinerVector2D& point) {

  // Check for a special case: all points are equal (resulting in
  // comparing to a point, not a triangle).
  if (vectorEqual(triangle.a,triangle.b) && vectorEqual(triangle.a,triangle.c)) {
    //deepdeepdebugf("pit2 special case all equal");
    debugreturn("        pit2","vectorEqual",vectorEqual(triangle.a,point));
  }
  
  // Check for a special case: triangle collapses to a line (at least
  // in 2D).
  if (vectorEqual(triangle.a,triangle.b)) {
    //deepdeepdebugf("pit2 special case AB equal");
    debugreturn("        pit2","point on AB line",pointOnLine2D(triangle.a,triangle.c,point));
  } else if (vectorEqual(triangle.a,triangle.c)) {
    //deepdeepdebugf("pit2 special case AC equal");
    debugreturn("        pit2","point on AC line",pointOnLine2D(triangle.a,triangle.b,point));
  } else if (vectorEqual(triangle.b,triangle.c)) {
    //deepdeepdebugf("pit2 special case BC equal");
    debugreturn("        pit2","point on BC line ",pointOnLine2D(triangle.a,triangle.b,point));
  }
  
  // Not a special case. For the general case, we take the algorithm
  // from https://mathworld.wolfram.com/TriangleInterior.html

  //deepdeepdebugf("pit2 regular case");
  OutlinerVector2D v = point; 
  OutlinerVector2D v0(triangle.a);
  OutlinerVector2D v1; vectorTo(triangle.a,triangle.b,v1);
  OutlinerVector2D v2; vectorTo(triangle.a,triangle.c,v2);
  //deepdeepdebugf("pit2 triangle v = (%.2f,%.2f)", v.x, v.y);
  //deepdeepdebugf("pit2 triangle v0 = (%.2f,%.2f)", v0.x, v0.y);
  //deepdeepdebugf("pit2 triangle v1 = (%.2f,%.2f)", v1.x, v1.y);
  //deepdeepdebugf("pit2 triangle v2 = (%.2f,%.2f)", v2.x, v2.y);
  outlinerreal a = (determinant2x2(v,v2) - determinant2x2(v0,v2)) / determinant2x2(v1,v2);
  outlinerreal b = -((determinant2x2(v,v1) - determinant2x2(v0,v1)) / determinant2x2(v1,v2));
  debugreturn("        pit2","regular",a >= 0 && b >= 0 && a+b <= 1);
}

bool
OutlinerMath::pointInsideBoundingBox2D(const OutlinerVector2D& boxStart,
                                       const OutlinerVector2D& boxEnd,
                                       const aiVector2D& point) {
  return(point.x >= boxStart.x && point.x <= boxEnd.x &&
         point.y >= boxStart.y && point.y <= boxEnd.y);
}

bool
OutlinerMath::pointInsideBoundingBox2D(const OutlinerVector2D& boxStart,
                                       const OutlinerVector2D& boxEnd,
                                       const OutlinerVector2D& point) {
  return(point.x >= boxStart.x && point.x <= boxEnd.x &&
         point.y >= boxStart.y && point.y <= boxEnd.y);
}

bool
OutlinerMath::boundingBoxIntersectsTriangle2D(const OutlinerTriangle2D& triangle,
                                              const OutlinerVector2D& boxStart,
                                              const OutlinerVector2D& boxEnd) {
  
  // Debugs
  deepdeepdebugf("        boundingBoxIntersectsTriangle2D (%.2f,%.2f)-(%.2f,%.2f)-(%.2f,%.2f) and (%.2f,%.2f)-(%.2f,%.2f)",
                 triangle.a.x, triangle.a.y, triangle.b.x, triangle.b.y, triangle.c.x, triangle.c.y,
                 boxStart.x, boxStart.y,
                 boxEnd.x, boxEnd.y);
  
  // First, if triangle corners are in the box, they intersect
  if (pointInsideBoundingBox2D(boxStart,boxEnd,triangle.a)) debugreturn("        bbit2","corner a",1);
  if (pointInsideBoundingBox2D(boxStart,boxEnd,triangle.b)) debugreturn("        bbit2","corner b",1);
  if (pointInsideBoundingBox2D(boxStart,boxEnd,triangle.c)) debugreturn("        bbit2","corner c",1);

  // Otherwise, (for now just an approximation) check if the box corners or middle are in the triangle
  OutlinerVector2D boxUpperRight(boxEnd.x,boxStart.y);
  OutlinerVector2D boxLowerLeft(boxStart.x,boxEnd.y);
  OutlinerVector2D boxMiddle((boxStart.x + boxEnd.x) / 2,(boxStart.y + boxEnd.y) / 2);
  if (pointInsideTriangle2D(triangle,boxStart)) debugreturn("        bbit2","start",1);
  if (pointInsideTriangle2D(triangle,boxEnd)) debugreturn("        bbit2","end",1);
  if (pointInsideTriangle2D(triangle,boxUpperRight)) debugreturn("        bbit2","upper",1);
  if (pointInsideTriangle2D(triangle,boxLowerLeft)) debugreturn("        bbit2","lower",1);
  if (pointInsideTriangle2D(triangle,boxMiddle)) debugreturn("        bbit2","middle",1);
  
  // Otherwise no
  debugreturn("        bbit2","fallback",0);
}

bool
OutlinerMath::boundingBoxIntersectsTriangle3D(const OutlinerTriangle3D& triangle,
                                              const OutlinerVector3D& boxStart,
                                              const OutlinerVector3D& boxEnd) {
  // Sanity checks
  deepdeepdebugf("        bbit3 starts");
  assert(boxStart.x <= boxEnd.x);
  assert(boxStart.y <= boxEnd.y);
  assert(boxStart.z <= boxEnd.z);
  
  // Heuristic algorithm, first check if there's an xy-plane match
  deepdeepdebugf("        bbit3 2d");
  OutlinerVector2D a2(triangle.a.x,triangle.a.y);
  OutlinerVector2D b2(triangle.b.x,triangle.b.y);
  OutlinerVector2D c2(triangle.c.x,triangle.c.y);
  deepdeepdebugf("        bbit3 boxes");
  OutlinerVector2D boxStart2(boxStart.x,boxStart.y);
  OutlinerVector2D boxEnd2(boxEnd.x,boxEnd.y);
  deepdeepdebugf("        bbit3 xy plane check");
  OutlinerTriangle2D t2(a2,b2,c2);
  if (!boundingBoxIntersectsTriangle2D(t2,boxStart2,boxEnd2)) return(0);
  deepdeepdebugf("        bbit2 call returned");
  
  // If there was a match, check if the range of the triangle in
  // z axis overlaps with the given bounding box
  outlinerreal zlow = outlinermin3(triangle.a.z,triangle.b.z,triangle.c.z);
  outlinerreal zhigh = outlinermax3(triangle.a.z,triangle.b.z,triangle.c.z);
  deepdeepdebugf("        bbit3 z overlap check %.2f..%.2f", zlow, zhigh);
  debugreturn("        bbit3","final",outlineroverlap(zlow,zhigh,boxStart.z,boxEnd.z));
}

bool
OutlinerMath::boundingBoxesIntersect3D(OutlinerVector3D& boundingBox1Start,
                                       OutlinerVector3D& boundingBox1End,
                                       OutlinerVector3D& boundingBox2Start,
                                       OutlinerVector3D& boundingBox2End) {
  // Following the algorithm from https://math.stackexchange.com/questions/2651710/simplest-way-to-determine-if-two-3d-boxes-intersect

  bool xOverlap = (outlinerbetween(boundingBox1Start.x,boundingBox2Start.x,boundingBox1End.x) ||
                   outlinerbetween(boundingBox1Start.x,boundingBox2End.x,boundingBox1End.x) ||
                   outlinerbetween(boundingBox2Start.x,boundingBox1Start.x,boundingBox2End.x) ||
                   outlinerbetween(boundingBox2Start.x,boundingBox1End.x,boundingBox2End.x));
  if (!xOverlap) return(0);
  bool yOverlap = (outlinerbetween(boundingBox1Start.y,boundingBox2Start.y,boundingBox1End.y) ||
                   outlinerbetween(boundingBox1Start.y,boundingBox2End.y,boundingBox1End.y) ||
                   outlinerbetween(boundingBox2Start.y,boundingBox1Start.y,boundingBox2End.y) ||
                   outlinerbetween(boundingBox2Start.y,boundingBox1End.y,boundingBox2End.y));
  if (!yOverlap) return(0);
  bool zOverlap = (outlinerbetween(boundingBox1Start.z,boundingBox2Start.z,boundingBox1End.z) ||
                   outlinerbetween(boundingBox1Start.z,boundingBox2End.z,boundingBox1End.z) ||
                   outlinerbetween(boundingBox2Start.z,boundingBox1Start.z,boundingBox2End.z) ||
                   outlinerbetween(boundingBox2Start.z,boundingBox1End.z,boundingBox2End.z));
  if (!zOverlap) return(0);
  return(1);
}

bool
OutlinerMath::lineIntersectsVerticalLine2D(const aiVector2D& lineStart,
                                           const aiVector2D& lineEnd,
                                           const aiVector2D& verticalLineStart,
                                           const aiVector2D& verticalLineEnd,
                                           aiVector2D& intersectionPoint) {
  assert(verticalLineStart.x == verticalLineEnd.x);

  // Fetch basic values
  outlinerreal verticalStartY = outlinermin(verticalLineStart.y,verticalLineEnd.y);
  outlinerreal verticalEndY = outlinermax(verticalLineStart.y,verticalLineEnd.y);
  outlinerreal verticalX = verticalLineStart.x;
  
  // Order line points such that X grows from start to end
  outlinerreal lineStartX;
  outlinerreal lineStartY;
  outlinerreal lineEndX;
  outlinerreal lineEndY;
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
  outlinerreal equationBaseY = lineStartY;
  outlinerreal equationTotalDifferenceY = lineEndY - lineStartY; // positive or negative
  outlinerreal equationTotalDifferenceX = lineEndX - lineStartX; // positive

  // Check for the case of parallel lines
  if (equationTotalDifferenceX == 0) {
    if (lineStartX != verticalX) return(0);
    if (outlineroverlap(lineStartY,lineEndY,verticalStartY,verticalEndY)) {
      intersectionPoint.x = verticalX;
      if (lineStartY <= verticalStartY && verticalStartY <= lineEndY) intersectionPoint.y = verticalStartY;
      else if (verticalStartY <= lineStartY && lineStartY <= verticalEndY) intersectionPoint.y = lineStartY;
      else assert(0);
      return(1);
    }
    return(0);
  }

  // Continue calculating the line equation
  outlinerreal equationFactor = equationTotalDifferenceY / equationTotalDifferenceX; // positive or negative
  outlinerreal verticalLineDifferenceX = verticalX - lineStartX; // positive or negative

  // Check if vertical line is in the range on X
  if (!outlinerbetween(lineStartX,verticalX,lineEndX)) return(0);

  // Calculate resulting Y at vertical line position
  outlinerreal lineY = equationBaseY + verticalLineDifferenceX * equationFactor;

  // Check if the resulting position is within the vertical line Y range
  if (!outlinerbetween(verticalStartY,lineY,verticalEndY)) return(0);

  // Lines intersect! Set the intersection point
  intersectionPoint.x = verticalX;
  intersectionPoint.y = lineY;
  return(1);
}

bool
OutlinerMath::lineIntersectsHorizontalLine2D(const aiVector2D& lineStart,
                                             const aiVector2D& lineEnd,
                                             const aiVector2D& horizontalLineStart,
                                             const aiVector2D& horizontalLineEnd,
                                             aiVector2D& intersectionPoint) {
  assert(horizontalLineStart.y == horizontalLineEnd.y);
  
  // Fetch basic values
  outlinerreal horizontalStartX = outlinermin(horizontalLineStart.x,horizontalLineEnd.x);
  outlinerreal horizontalEndX = outlinermax(horizontalLineStart.x,horizontalLineEnd.x);
  outlinerreal horizontalY = horizontalLineStart.y;
  
  // Order line points such that Y grows from start to end
  outlinerreal lineStartX;
  outlinerreal lineStartY;
  outlinerreal lineEndX;
  outlinerreal lineEndY;
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
  outlinerreal equationBaseX = lineStartX;
  outlinerreal equationTotalDifferenceX = lineEndX - lineStartX; // positive or negative
  outlinerreal equationTotalDifferenceY = lineEndY - lineStartY; // positive

  // Check for the case of parallel lines
  if (equationTotalDifferenceY == 0) {
    if (lineStartY != horizontalY) return(0);
    if (outlineroverlap(lineStartX,lineEndX,horizontalStartX,horizontalEndX)) {
      intersectionPoint.y = horizontalY;
      if (lineStartX <= horizontalStartX && horizontalStartX <= lineEndX) intersectionPoint.x = horizontalStartX;
      else if (horizontalStartX <= lineStartX && lineStartX <= horizontalEndX) intersectionPoint.x = lineStartX;
      else assert(0);
      return(1);
    }
    return(0);
  }

  // Continue calculating the line equation
  outlinerreal equationFactor = equationTotalDifferenceX / equationTotalDifferenceY; // positive or negative
  outlinerreal horizontalLineDifferenceY = horizontalY - lineStartY; // positive or negative

  // Check if horizontal line is in the range on Y
  if (!outlinerbetween(lineStartY,horizontalY,lineEndY)) return(0);

  // Calculate resulting X at horizontal line position
  outlinerreal lineX = equationBaseX + horizontalLineDifferenceY * equationFactor;

  // Check if the resulting position is within the horizontal line X range
  if (!outlinerbetween(horizontalStartX,lineX,horizontalEndX)) return(0);

  // Lines intersect! Set the intersection point
  intersectionPoint.x = lineX;
  intersectionPoint.y = horizontalY;
  return(1);
}

bool
OutlinerMath::vectorEqual(const aiVector2D& a,
                          const aiVector2D& b) {
  return(a.x == b.x && a.y == b.y);
}

bool
OutlinerMath::vectorEqual(const OutlinerVector2D& a,
                          const OutlinerVector2D& b) {
  return(a.x == b.x && a.y == b.y);
}

void
OutlinerMath::vectorTo(const OutlinerVector2D& from,
                       const OutlinerVector2D& to,
                       OutlinerVector2D& result) {
  result = to;
  result.x -= from.x;
  result.y -= from.y;
}

outlinerreal
OutlinerMath::determinant2x2(const OutlinerVector2D& u,
                             const OutlinerVector2D& v) {
  return(u.x * v.y - u.y * v.x);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Auxiliary private functions ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
OutlinerMath::sortVectorsX2D(const OutlinerVector2D* a,
                             const OutlinerVector2D* b,
                             const OutlinerVector2D* c,
                             const OutlinerVector2D** nth0,
                             const OutlinerVector2D** nth1,
                             const OutlinerVector2D** nth2) {

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

void
OutlinerMath::sortVectorsY2D(const OutlinerVector2D* a,
                             const OutlinerVector2D* b,
                             const OutlinerVector2D* c,
                             const OutlinerVector2D** nth0,
                             const OutlinerVector2D** nth1,
                             const OutlinerVector2D** nth2) {
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

void
OutlinerMath::sortVectorsX3D(const OutlinerVector3D* a,
                             const OutlinerVector3D* b,
                             const OutlinerVector3D* c,
                             const OutlinerVector3D** nth0,
                             const OutlinerVector3D** nth1,
                             const OutlinerVector3D** nth2) {

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

void
OutlinerMath::sortVectorsY3D(const OutlinerVector3D* a,
                             const OutlinerVector3D* b,
                             const OutlinerVector3D* c,
                             const OutlinerVector3D** nth0,
                             const OutlinerVector3D** nth1,
                             const OutlinerVector3D** nth2) {
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

void
OutlinerMath::sortVectorsZ3D(const OutlinerVector3D* a,
                             const OutlinerVector3D* b,
                             const OutlinerVector3D* c,
                             const OutlinerVector3D** nth0,
                             const OutlinerVector3D** nth1,
                             const OutlinerVector3D** nth2) {
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

///////////////////////////////////////////////////////////////////////////////////////////////
// Test functions /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
OutlinerMath::mathTests(void) {
  infof("running math tests");
  utilityTests();
  vectorTests();
  detTests();
  pointTests();
  lineTests();
  lineIntersectionTests();
  triangleTests();
  boundingBoxTests();
  boundingBoxEqualTests();
  triangleBoundingBoxTests();
  boundingBoxIntersectionTests();
  boundingBoxUnionTests();
  infof("math tests ok");
}

void
OutlinerMath::utilityTests(void) {

  debugf("utility tests...");
  // Between tests
  assert(outlinerbetween(1,2,3));
  assert(outlinerbetween(1,2,2));
  assert(!outlinerbetween(1,3,2));
  assert(!outlinerbetween(3,2,1));

  // Betweenanyorder tests
  assert(outlinerbetweenanyorder(1,2,3));
  assert(outlinerbetweenanyorder(1,2,2));
  assert(outlinerbetweenanyorder(2,2,1));
  assert(!outlinerbetweenanyorder(1,3,2));
  assert(!outlinerbetweenanyorder(2,3,1));
  assert(outlinerbetweenanyorder(3,2,1));

  // Overlap tests
  assert(outlineroverlap(1,2,2,4));
  assert(outlineroverlap(1,10,2,4));
  assert(outlineroverlap(2,3,0,10));
  assert(!outlineroverlap(1,2,3,4));
  assert(!outlineroverlap(3,4,1,2));

  // Overlapanyorder tests
  assert(outlineroverlapanyorder(1,2,2,4));
  assert(outlineroverlapanyorder(2,1,2,4));
  assert(outlineroverlapanyorder(2,1,4,2));
  assert(outlineroverlapanyorder(1,2,4,2));
  assert(outlineroverlapanyorder(1,10,4,2));
  assert(outlineroverlapanyorder(2,3,100,1));
  assert(!outlineroverlapanyorder(1,2,3,4));
  assert(!outlineroverlapanyorder(4,3,2,1));
}

void
OutlinerMath::vectorTests(void) {
  debugf("vector tests...");
  OutlinerVector2D a(2,2);
  OutlinerVector2D b(3,3);
  OutlinerVector2D result;
  vectorTo(a,b,result);
  deepdebugf("vector test: result: (%f,%f)", result.x, result.y);
  assert(result.x == 1);
  assert(result.y == 1);
}

void
OutlinerMath::detTests(void) {
  debugf("det tests...");
  OutlinerVector2D C1(4,2);
  OutlinerVector2D C2(1,3);
  outlinerreal result = determinant2x2(C1,C2);
  deepdebugf("determinant result = %.2f", result);
  assert(result == 10);
}

void
OutlinerMath::boundingBoxTests(void) {
  debugf("bounding box tests...");
  aiVector2D a(0,0);
  aiVector2D b(0,3);
  aiVector2D c(2,0);
  OutlinerVector2D boundingBoxStart;
  OutlinerVector2D boundingBoxEnd;
  debugf("bounding box tests");
  OutlinerTriangle2D tone(a,a,a);
  triangleBoundingBox2D(tone,boundingBoxStart,boundingBoxEnd);
  debugf("a,a,a bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBoxStart.x, boundingBoxStart.y, boundingBoxEnd.x, boundingBoxEnd.y);
  assert(boundingBoxStart.x == 0 && boundingBoxStart.y == 0);
  assert(boundingBoxEnd.x == 0 && boundingBoxEnd.y == 0);
  OutlinerTriangle2D t(a,b,c);
  triangleBoundingBox2D(t,boundingBoxStart,boundingBoxEnd);
  debugf("a,b,c bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBoxStart.x, boundingBoxStart.y, boundingBoxEnd.x, boundingBoxEnd.y);
  assert(boundingBoxStart.x == 0 && boundingBoxStart.y == 0);
  assert(boundingBoxEnd.x == 2 && boundingBoxEnd.y == 3);
  OutlinerTriangle2D trev(c,b,a);
  triangleBoundingBox2D(trev,boundingBoxStart,boundingBoxEnd);
  debugf("c,b,a bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBoxStart.x, boundingBoxStart.y, boundingBoxEnd.x, boundingBoxEnd.y);
  assert(boundingBoxStart.x == 0 && boundingBoxStart.y == 0);
  assert(boundingBoxEnd.x == 2 && boundingBoxEnd.y == 3);
  aiVector2D x(-10,-10);
  aiVector2D y(10,10);
  aiVector2D z(30,9);
  OutlinerTriangle2D trevx(z,y,x);
  triangleBoundingBox2D(trevx,boundingBoxStart,boundingBoxEnd);
  debugf("z,y,x bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBoxStart.x, boundingBoxStart.y, boundingBoxEnd.x, boundingBoxEnd.y);
  assert(boundingBoxStart.x == -10 && boundingBoxStart.y == -10);
  assert(boundingBoxEnd.x == 30 && boundingBoxEnd.y == 10);
  OutlinerTriangle2D trevx2(y,z,x);
  triangleBoundingBox2D(trevx2,boundingBoxStart,boundingBoxEnd);
  debugf("y,z,x bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBoxStart.x, boundingBoxStart.y, boundingBoxEnd.x, boundingBoxEnd.y);
  assert(boundingBoxStart.x == -10 && boundingBoxStart.y == -10);
  assert(boundingBoxEnd.x == 30 && boundingBoxEnd.y == 10);

  OutlinerVector2D bbtest1start(10,10);
  OutlinerVector2D bbtest1end(20,30);
  OutlinerVector2D bbtest1point1(0,25);
  OutlinerVector2D bbtest1point2(10,10);
  OutlinerVector2D bbtest1point3(20,30);
  OutlinerVector2D bbtest1point4(30,30);
  OutlinerVector2D bbtest1point5(11,15);
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

void
OutlinerMath::boundingBoxIntersectionTests(void) {
  infof("bounding box intersection tests...");
  
  OutlinerVector3D test1boundingBox1Start(0,0,0);
  OutlinerVector3D test1boundingBox1End(100,100,100);
  OutlinerVector3D test1boundingBox2Start(10,10,10);
  OutlinerVector3D test1boundingBox2End(11,11,11);
  bool ans = boundingBoxesIntersect3D(test1boundingBox1Start,
                                      test1boundingBox1End,
                                      test1boundingBox2Start,
                                      test1boundingBox2End);
  assert(ans);
  
  OutlinerVector3D test2boundingBox1Start(0,0,0);
  OutlinerVector3D test2boundingBox1End(10,10,10);
  OutlinerVector3D test2boundingBox2Start(11,11,11);
  OutlinerVector3D test2boundingBox2End(12,12,12);
  ans = boundingBoxesIntersect3D(test2boundingBox1Start,
                                 test2boundingBox1End,
                                 test2boundingBox2Start,
                                 test2boundingBox2End);
  assert(!ans);
  
  OutlinerVector3D test3boundingBox1Start(0,0,0);
  OutlinerVector3D test3boundingBox1End(10,10,10);
  OutlinerVector3D test3boundingBox2Start(0,0,11);
  OutlinerVector3D test3boundingBox2End(2,2,12);
  ans = boundingBoxesIntersect3D(test3boundingBox1Start,
                                 test3boundingBox1End,
                                 test3boundingBox2Start,
                                 test3boundingBox2End);
  assert(!ans);

  const OutlinerVector2D box1aStart(0,0);
  const OutlinerVector2D box1aEnd(10,10);
  const OutlinerVector2D box1bStart(3,0);
  const OutlinerVector2D box1bEnd(7,6);
  const OutlinerVector2D box2Start(2,2);
  const OutlinerVector2D box2End(8,8);
  OutlinerVector2D resultBoxStart;
  OutlinerVector2D resultBoxEnd;
  boundingBoxIntersection(box1aStart,
                          box1aEnd,
                          box2Start,
                          box2End,
                          resultBoxStart,
                          resultBoxEnd);
  assert(resultBoxStart.x == 2 && resultBoxStart.y == 2);
  assert(resultBoxEnd.x == 8 && resultBoxEnd.y == 8);
  boundingBoxIntersection(box1bStart,
                          box1bEnd,
                          box2Start,
                          box2End,
                          resultBoxStart,
                          resultBoxEnd);
  assert(resultBoxStart.x == 3 && resultBoxStart.y == 2);
  assert(resultBoxEnd.x == 7 && resultBoxEnd.y == 6);
  
  infof("bounding box intersection tests ok");
}

void
OutlinerMath::boundingBoxEqualTests(void) {
  debugf("bounding box equal  tests...");
  const OutlinerVector2D box1Start(0,0);
  const OutlinerVector2D box1End(10,10);
  const OutlinerVector2D box2Start(3,0);
  const OutlinerVector2D box2End(7,6);
  const OutlinerVector2D box3Start(0,0);
  const OutlinerVector2D box3End(10,10);
  const OutlinerVector2D box4Start(0,-1);
  const OutlinerVector2D box4End(10,10);
  bool ans;
  ans = boundingBoxEqual(box1Start,box1End,box2Start,box2End);
  assert(!ans);
  ans = boundingBoxEqual(box1Start,box1End,box3Start,box3End);
  assert(ans);
  ans = boundingBoxEqual(box1Start,box1End,box4Start,box4End);
  assert(!ans);
  debugf("bounding box equal tests ok");
}

void
OutlinerMath::boundingBoxUnionTests(void) {
  infof("bounding box union tests...");
  
  const OutlinerVector2D box1aStart(0,0);
  const OutlinerVector2D box1aEnd(10,10);
  const OutlinerVector2D box1bStart(-3,0);
  const OutlinerVector2D box1bEnd(7,16);
  const OutlinerVector2D box2Start(2,2);
  const OutlinerVector2D box2End(8,8);
  OutlinerVector2D resultBoxStart;
  OutlinerVector2D resultBoxEnd;
  boundingBoxUnion(box1aStart,
                   box1aEnd,
                   box2Start,
                   box2End,
                   resultBoxStart,
                   resultBoxEnd);
  deepdebugf("union result (%.2f,%.2f)-(%.2f,%.2f)",
             resultBoxStart.x, resultBoxStart.y,
             resultBoxEnd.x, resultBoxEnd.y);
  assert(resultBoxStart.x == 0 && resultBoxStart.y == 0);
  assert(resultBoxEnd.x == 10 && resultBoxEnd.y == 10);
  boundingBoxUnion(box1bStart,
                   box1bEnd,
                   box2Start,
                   box2End,
                   resultBoxStart,
                   resultBoxEnd);
  deepdebugf("union result (%.2f,%.2f)-(%.2f,%.2f)",
             resultBoxStart.x, resultBoxStart.y,
             resultBoxEnd.x, resultBoxEnd.y);
  assert(resultBoxStart.x == -3 && resultBoxStart.y == 0);
  assert(resultBoxEnd.x == 8 && resultBoxEnd.y == 16);
  
  infof("bounding box union tests ok");
}

void
OutlinerMath::pointTests(void) {
  debugf("point tests...");
  aiVector2D a(1,1);
  aiVector2D b(1,2);
  aiVector2D c(1,1);
  assert(vectorEqual(a,b) == 0);
  assert(vectorEqual(b,c) == 0);
  assert(vectorEqual(a,c) == 1);
}

void
OutlinerMath::lineTests(void) {

  debugf("line tests...");
  
  // Horizontal
  {
    aiVector2D a(0,0);
    aiVector2D b(1,0);
    aiVector2D c(2,0);
    OutlinerVector2D d(0.5,2);
    bool ans = pointOnLine2D(a,c,d);
    assert(ans == 0);
    ans = pointOnLine2D(a,b,c);
    assert(ans == 0);
    ans = pointOnLine2D(a,c,b);
    assert(ans == 1);
  }
  
  // Vertical
  {
    aiVector2D a(0,0);
    aiVector2D b(0,1);
    aiVector2D c(0,2);
    OutlinerVector2D d(0.5,1);
    bool ans = pointOnLine2D(a,c,d);
    assert(ans == 0);
    ans = pointOnLine2D(a,b,c);
    assert(ans == 0);
    ans = pointOnLine2D(a,c,b);
    assert(ans == 1);
  }
  
  // Sloping line
  {
    aiVector2D a(0,0);
    aiVector2D b(1,1);
    aiVector2D c(2,2);
    aiVector2D d(1,2);
    bool ans = pointOnLine2D(a,c,d);
    assert(ans == 0);
    ans = pointOnLine2D(a,b,c);
    assert(ans == 0);
    ans = pointOnLine2D(a,c,b);
    assert(ans == 1);
  }

}

void
OutlinerMath::lineIntersectionTests(void) {

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

void
OutlinerMath::triangleTests(void) {
  debugf("triangle tests (2D)...");
  aiVector2D a(0,0);
  aiVector2D b(0,2);
  aiVector2D c(2,0);
  OutlinerVector2D pointfar(2,2);
  OutlinerVector2D pointnear(0.5,0.5);
  OutlinerVector2D pointverynear(0.1,0.2);
  OutlinerVector2D pointata = a;
  OutlinerVector2D pointatb = b;
  OutlinerVector2D pointatc = c;
  OutlinerVector2D pointbeyondb = b;
  pointbeyondb.x += 0.01;
  pointbeyondb.y += 0.01;
  OutlinerVector2D pointbefore1(-0.001,0);
  OutlinerVector2D pointbefore2(0,-0.001);
  OutlinerVector2D pointbefore3(-0.001,-0.001);
  OutlinerTriangle2D t(a,b,c);
  bool ansfar = pointInsideTriangle2D(t,pointfar);
  deepdebugf("triangle test: pointfar = %u", ansfar);
  bool ansnear = pointInsideTriangle2D(t,pointnear);
  deepdebugf("triangle test: pointnear = %u", ansnear);
  bool ansverynear = pointInsideTriangle2D(t,pointverynear);
  deepdebugf("triangle test: pointverynear = %u", ansverynear);
  bool ansata = pointInsideTriangle2D(t,pointata);
  deepdebugf("triangle test: pointata = %u", ansata);
  bool ansatb = pointInsideTriangle2D(t,pointatb);
  deepdebugf("triangle test: pointatb = %u", ansatb);
  bool ansatc = pointInsideTriangle2D(t,pointatc);
  deepdebugf("triangle test: pointatc = %u", ansatc);
  bool ansbeyondb = pointInsideTriangle2D(t,pointbeyondb);
  deepdebugf("triangle test: pointbeyondb = %u", ansbeyondb);
  bool ansbefore1 = pointInsideTriangle2D(t,pointbefore1);
  deepdebugf("triangle test: pointbefore1 = %u", ansbefore1);
  bool ansbefore2 = pointInsideTriangle2D(t,pointbefore2);
  deepdebugf("triangle test: pointbefore2 = %u", ansbefore2);
  bool ansbefore3 = pointInsideTriangle2D(t,pointbefore3);
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

  // 3D triangle tests
  debugf("triangle tests (3D)...");
  aiVector3D a3(0,0,10);
  aiVector3D b3(0,2,10);
  aiVector3D c3(2,0,10);
  OutlinerVector3D boundingStart3a(0,0,0);
  OutlinerVector3D boundingEnd3a(5,5,5);
  OutlinerVector3D boundingStart3b(0,0,0);
  OutlinerVector3D boundingEnd3b(5,5,10);
  OutlinerTriangle3D t3(a3,b3,c3);
  bool ans3 = boundingBoxIntersectsTriangle3D(t3,boundingStart3a,boundingEnd3a);
  assert(!ans3);
  ans3 = boundingBoxIntersectsTriangle3D(t3,boundingStart3b,boundingEnd3b);
  assert(ans3);

  // Bug test for 3D triangle cases
  debugf("triangle tests (3D bug)...");
  OutlinerVector3D bugBoxStart(0.00,-1.00,0.00);
  OutlinerVector3D bugBoxEnd(0.00,-0.50,0.50);
  aiVector3D buga(1.00, -1.00, -1.00);
  aiVector3D bugb(-1.00,-1.00, 1.00);
  aiVector3D bugc(-1.00,-1.00,-1.00);
  OutlinerTriangle3D bugt(buga,bugb,bugc);
  bool ansbug = boundingBoxIntersectsTriangle3D(bugt,bugBoxStart,bugBoxEnd);
  assert(ansbug);
}

void
OutlinerMath::triangleBoundingBoxTests(void) {
  infof("triangle bounding box tests..");
  
  aiVector2D a(0,0);
  aiVector2D b(0,10);
  aiVector2D c(20,0);
  aiVector2D degenerate1a(1,1);
  aiVector2D degenerate1b(1,1);
  aiVector2D degenerate1c(-1,1);
  aiVector2D degenerate2a(1,1);
  aiVector2D degenerate2b(1,1);
  aiVector2D degenerate2c(-1,-1);
  OutlinerVector2D box1Start(-10,-10);
  OutlinerVector2D box1End(-1,-1);
  OutlinerVector2D box2Start(-10,-10);
  OutlinerVector2D box2End(5,5);
  OutlinerVector2D box3Start(-1000,-1000);
  OutlinerVector2D box3End(1000,1000);
  OutlinerVector2D box4Start(-10,-10);
  OutlinerVector2D box4End(0,0);
  OutlinerVector2D box5Start(0.0,0.0);
  OutlinerVector2D box5End(0.0,0.50);
  OutlinerTriangle2D t(a,b,c);
  bool ans = boundingBoxIntersectsTriangle2D(t,box1Start,box1End);
  assert(!ans);
  ans = boundingBoxIntersectsTriangle2D(t,box2Start,box2End);
  assert(ans);
  ans = boundingBoxIntersectsTriangle2D(t,box3Start,box3End);
  assert(ans);
  ans = boundingBoxIntersectsTriangle2D(t,box4Start,box4End);
  assert(ans);
  OutlinerTriangle2D degenerate1t(degenerate1a,degenerate1b,degenerate1c);
  ans = boundingBoxIntersectsTriangle2D(degenerate1t,box5Start,box5End);
  assert(!ans);
  OutlinerTriangle2D degenerate2t(degenerate2a,degenerate2b,degenerate2c);
  ans = boundingBoxIntersectsTriangle2D(degenerate2t,box5Start,box5End);
  assert(ans);
  infof("triangle bounding box tests ok");
}

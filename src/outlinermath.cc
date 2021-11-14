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
  
bool debugbbit3 = 0;

# define  debugreturn(f,why,x) {                                             \
    bool drv = (x);                                                          \
    deepdeepdebugf("%s returns %u due to %s", (f), drv, (why));              \
    return(drv); \
 }

///////////////////////////////////////////////////////////////////////////////////////////////
// Math functions /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
OutlinerMath::triangleDescribe(const OutlinerTriangle3D& triangle,
                               char* buf,
                               unsigned int bufSize,
                               bool full) {
  assert(buf != 0);
  assert(bufSize > 0);
  memset(buf,0,bufSize);
  if (full) {
    snprintf(buf,bufSize-1," (%f,%f,%f) (%f,%f,%f) (%f,%f,%f) ",
             triangle.a.x, triangle.a.y, triangle.a.z,
             triangle.b.x, triangle.b.y, triangle.b.z,
             triangle.c.x, triangle.c.y, triangle.c.z);
  } else {
    outlinerreal xlow = outlinermin3(triangle.a.x,triangle.b.x,triangle.c.x);
    outlinerreal xhigh = outlinermax3(triangle.a.x,triangle.b.x,triangle.c.x);
    outlinerreal ylow = outlinermin3(triangle.a.y,triangle.b.y,triangle.c.y);
    outlinerreal yhigh = outlinermax3(triangle.a.y,triangle.b.y,triangle.c.y);
    outlinerreal zlow = outlinermin3(triangle.a.z,triangle.b.z,triangle.c.z);
    outlinerreal zhigh = outlinermax3(triangle.a.z,triangle.b.z,triangle.c.z);
    if (zlow == zhigh) {
      snprintf(buf,bufSize-1,"horizontal along z at %.2f, x %.2f..%.2f y %.2f..%.2f",
               zlow, xlow, xhigh, ylow, yhigh);
    } else if (xlow == xhigh) {
      snprintf(buf,bufSize-1,"vertical along x at %.2f, y %.2f..%.2f, z %.2f..%.2f",
               xlow, ylow, yhigh, zlow, zhigh);
    } else if (ylow == yhigh) {
      snprintf(buf,bufSize-1,"vertical along y at %.2f, x %.2f..%.2f, z %.2f..%.2f",
               ylow, xlow, xhigh, zlow, zhigh);
    } else {
      snprintf(buf,bufSize-1,"irregular, x %.2f..%.2f y %.2f..%.2f z %.2f..%.2f",
               xlow, xhigh,
               ylow, yhigh,
               zlow, zhigh);
    }
  }
}

void
OutlinerMath::triangleDescribe(const OutlinerTriangle2D& triangle,
                               char* buf,
                               unsigned int bufSize,
                               bool full) {
  assert(buf != 0);
  assert(bufSize > 0);
  memset(buf,0,bufSize);
  if (full) {
    snprintf(buf,bufSize-1," (%f,%f) (%f,%f) (%f,%f)",
             triangle.a.x, triangle.a.y, 
             triangle.b.x, triangle.b.y,
             triangle.c.x, triangle.c.y);
  } else {
    outlinerreal xlow = outlinermin3(triangle.a.x,triangle.b.x,triangle.c.x);
    outlinerreal xhigh = outlinermax3(triangle.a.x,triangle.b.x,triangle.c.x);
    outlinerreal ylow = outlinermin3(triangle.a.y,triangle.b.y,triangle.c.y);
    outlinerreal yhigh = outlinermax3(triangle.a.y,triangle.b.y,triangle.c.y);
    if (xlow == xhigh) {
      snprintf(buf,bufSize-1,"vertical along x at %.2f, y %.2f..%.2f",
               xlow, ylow, yhigh);
    } else if (ylow == yhigh) {
      snprintf(buf,bufSize-1,"vertical along y at %.2f, x %.2f..%.2f",
               ylow, xlow, xhigh);
    } else {
      snprintf(buf,bufSize-1,"irregular, x %.2f..%.2f y %.2f..%.2f",
               xlow, xhigh,
               ylow, yhigh);
    }
  }
}

void
OutlinerMath::boxDescribe(const OutlinerBox3D& box,
                          char* buf,
                          unsigned int bufSize,
                          bool full) {
  assert(buf != 0);
  assert(bufSize > 0);
  memset(buf,0,bufSize);
  snprintf(buf,bufSize-1,"x %.2f..%.2f y %.2f..%.2f z %.2f..%.2f",
           box.start.x, box.end.x,
           box.start.y, box.end.y,
           box.start.z, box.end.z);
}

void
OutlinerMath::boxDescribe(const OutlinerBox2D& box,
                          char* buf,
                          unsigned int bufSize,
                          bool full) {
  assert(buf != 0);
  assert(bufSize > 0);
  memset(buf,0,bufSize);
  snprintf(buf,bufSize-1,"x %.2f..%.2f y %.2f..%.2f",
           box.start.x, box.end.x,
           box.start.y, box.end.y);
}

void
OutlinerMath::triangleBoundingBox2D(const OutlinerTriangle2D& triangle,
                                    OutlinerBox2D& boundingBox) {
  
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
  boundingBox.start.x = xStart;
  boundingBox.start.y = yStart;
  boundingBox.end.x = xEnd;
  boundingBox.end.y = yEnd;
}

void
OutlinerMath::triangleBoundingBox3D(const OutlinerTriangle3D& triangle,
                                    OutlinerBox3D& boundingBox) {

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
  if (debugbbit3) debugf("        z range %.2f...%.2f from %.2f, %.2f, %.2f", zStart, zEnd, triangle.a.z, triangle.b.z, triangle.c.z);
  
  // Construct the result
  boundingBox.start.x = xStart;
  boundingBox.start.y = yStart;
  boundingBox.start.z = zStart;
  boundingBox.end.x = xEnd;
  boundingBox.end.y = yEnd;
  boundingBox.end.z = zEnd;
}

bool
OutlinerMath::pointInsideTriangle2D(const OutlinerTriangle2D& triangle,
                                    const OutlinerVector2D& point) {

  // Check for a special case: all points are equal (resulting in
  // comparing to a point, not a triangle).
  if (triangle.a.equal(triangle.b) && triangle.a.equal(triangle.c)) {
    //deepdeepdebugf("pit2 special case all equal");
    debugreturn("        pit2","vectorEqual",triangle.a.equal(point));
  }
  
  // Check for a special case: triangle collapses to a line (at least
  // in 2D).
  if (triangle.a.equal(triangle.b)) {
    OutlinerLine2D ac(triangle.a,triangle.c);
    debugreturn("        pit2","point on AB line",ac.pointOnLine(point));
  } else if (triangle.a.equal(triangle.c)) {
    OutlinerLine2D ab(triangle.a,triangle.b);
    debugreturn("        pit2","point on AC line",ab.pointOnLine(point));
  } else if (triangle.b.equal(triangle.c)) {
    OutlinerLine2D ab(triangle.a,triangle.b);
    debugreturn("        pit2","point on BC line",ab.pointOnLine(point));
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
OutlinerMath::boundingBoxIntersectsTriangle2D(const OutlinerTriangle2D& triangle,
                                              const OutlinerBox2D& box) {
  
  // Debugs
  deepdeepdebugf("        boundingBoxIntersectsTriangle2D (%.2f,%.2f)-(%.2f,%.2f)-(%.2f,%.2f) and (%.2f,%.2f)-(%.2f,%.2f)",
                 triangle.a.x, triangle.a.y, triangle.b.x, triangle.b.y, triangle.c.x, triangle.c.y,
                 box.start.x, box.start.y,
                 box.end.x, box.end.y);
  
  // First, if triangle corners are in the box, they intersect
  if (box.pointInside(triangle.a)) debugreturn("        bbit2","corner a",1);
  if (box.pointInside(triangle.b)) debugreturn("        bbit2","corner b",1);
  if (box.pointInside(triangle.c)) debugreturn("        bbit2","corner c",1);

  // Otherwise, (for now just an approximation) check if the box corners or middle are in the triangle
  OutlinerVector2D boxUpperRight(box.end.x,box.start.y);
  OutlinerVector2D boxLowerLeft(box.start.x,box.end.y);
  OutlinerVector2D boxMiddle((box.start.x + box.end.x) / 2,(box.start.y + box.end.y) / 2);
  if (pointInsideTriangle2D(triangle,box.start)) debugreturn("        bbit2","start",1);
  if (pointInsideTriangle2D(triangle,box.end)) debugreturn("        bbit2","end",1);
  if (pointInsideTriangle2D(triangle,boxUpperRight)) debugreturn("        bbit2","upper",1);
  if (pointInsideTriangle2D(triangle,boxLowerLeft)) debugreturn("        bbit2","lower",1);
  if (pointInsideTriangle2D(triangle,boxMiddle)) debugreturn("        bbit2","middle",1);
  
  // Otherwise no
  debugreturn("        bbit2","fallback",0);
}

bool
OutlinerMath::boundingBoxIntersectsTriangle3D(const OutlinerTriangle3D& triangle,
                                              const OutlinerBox3D& box) {
  // Sanity checks
  deepdeepdebugf("        bbit3 starts");
  assert(box.start.x <= box.end.x);
  assert(box.start.y <= box.end.y);
  assert(box.start.z <= box.end.z);

  // NEW:
  // Approximate algorithm, check if the triangle's bounding box intersects
  OutlinerBox3D triangleBoundingBox;
  if (debugbbit3) {
    debugf("        bbit3 triangle ys %.2f, %.2f, %.2f",
           triangle.a.y, triangle.b.y, triangle.c.y);
    debugf("        bbit3 triangle zs %.2f, %.2f, %.2f",
           triangle.a.z, triangle.b.z, triangle.c.z);
  }
  triangleBoundingBox3D(triangle,triangleBoundingBox);
  bool ans = boundingBoxesIntersect3D(triangleBoundingBox,box);
  if (debugbbit3) {
    char buf[150];
    triangleDescribe(triangle,buf,sizeof(buf),1);
    debugf("      triangle = %s", buf);
    boxDescribe(triangleBoundingBox,buf,sizeof(buf),1);
    debugf("      triangle box = %s", buf);
    boxDescribe(box,buf,sizeof(buf),1);
    debugf("      box = %s", buf);
    debugf("      bbit3 return %u", ans);
  }
  return(ans);

  // OLD:
  // Heuristic algorithm, first check if there's an xy-plane match
  deepdeepdebugf("        bbit3 2d");
  OutlinerVector2D a2(triangle.a.x,triangle.a.y);
  OutlinerVector2D b2(triangle.b.x,triangle.b.y);
  OutlinerVector2D c2(triangle.c.x,triangle.c.y);
  deepdeepdebugf("        bbit3 boxes");
  OutlinerVector2D box2Start(box.start.x,box.start.y);
  OutlinerVector2D box2End(box.end.x,box.end.y);
  OutlinerBox2D box2(box2Start,box2End);
  deepdeepdebugf("        bbit3 xy plane check");
  OutlinerTriangle2D t2(a2,b2,c2);
  if (!boundingBoxIntersectsTriangle2D(t2,box2)) {
    if (debugbbit3) {
      char buf[120];
      triangleDescribe(t2,buf,sizeof(buf),1);
      debugf("      t2 = %s", buf);
      boxDescribe(box2,buf,sizeof(buf),1);
      debugf("      box2 = %s", buf);
      debugf("      bbit3 2d zero return");
    }
    return(0);
  }
  deepdeepdebugf("        bbit2 call returned");
  
  // If there was a match, check if the range of the triangle in
  // z axis overlaps with the given bounding box
  outlinerreal zlow = outlinermin3(triangle.a.z,triangle.b.z,triangle.c.z);
  outlinerreal zhigh = outlinermax3(triangle.a.z,triangle.b.z,triangle.c.z);
  deepdeepdebugf("        bbit3 z overlap check %.2f..%.2f", zlow, zhigh);
  bool overlapval = outlineroverlapepsilon(zlow,zhigh,box.start.z,box.end.z);
  if (debugbbit3) debugf("      bbit3 z check returns %u", overlapval);
  debugreturn("        bbit3","final",overlapval);
}

bool
OutlinerMath::boundingBoxesIntersect2D(const OutlinerBox2D& boundingBox1,
                                       const OutlinerBox2D& boundingBox2) {
  // Following the algorithm from https://math.stackexchange.com/questions/2651710/simplest-way-to-determine-if-two-3d-boxes-intersect
  
  bool xOverlap = (outlinerbetweenepsilon(boundingBox1.start.x,boundingBox2.start.x,boundingBox1.end.x) ||
                   outlinerbetweenepsilon(boundingBox1.start.x,boundingBox2.end.x,boundingBox1.end.x) ||
                   outlinerbetweenepsilon(boundingBox2.start.x,boundingBox1.start.x,boundingBox2.end.x) ||
                   outlinerbetweenepsilon(boundingBox2.start.x,boundingBox1.end.x,boundingBox2.end.x));
  if (!xOverlap) return(0);
  bool yOverlap = (outlinerbetweenepsilon(boundingBox1.start.y,boundingBox2.start.y,boundingBox1.end.y) ||
                   outlinerbetweenepsilon(boundingBox1.start.y,boundingBox2.end.y,boundingBox1.end.y) ||
                   outlinerbetweenepsilon(boundingBox2.start.y,boundingBox1.start.y,boundingBox2.end.y) ||
                   outlinerbetweenepsilon(boundingBox2.start.y,boundingBox1.end.y,boundingBox2.end.y));
  if (!yOverlap) return(0);
  return(1);
}

bool
OutlinerMath::boundingBoxesIntersect3D(const OutlinerBox3D& boundingBox1,
                                       const OutlinerBox3D& boundingBox2) {
  // Following the algorithm from https://math.stackexchange.com/questions/2651710/simplest-way-to-determine-if-two-3d-boxes-intersect
  
  bool xOverlap = (outlinerbetweenepsilon(boundingBox1.start.x, boundingBox2.start.x,boundingBox1.end.x) ||
                   outlinerbetweenepsilon(boundingBox1.start.x, boundingBox2.end.x,  boundingBox1.end.x) ||
                   outlinerbetweenepsilon(boundingBox2.start.x, boundingBox1.start.x,boundingBox2.end.x) ||
                   outlinerbetweenepsilon(boundingBox2.start.x, boundingBox1.end.x,  boundingBox2.end.x));
  if (debugbbit3) debugf("        bbi3 xOverlap %u", xOverlap);
  if (!xOverlap) return(0);
  bool yOverlap = (outlinerbetweenepsilon(boundingBox1.start.y, boundingBox2.start.y,boundingBox1.end.y) ||
                   outlinerbetweenepsilon(boundingBox1.start.y, boundingBox2.end.y,  boundingBox1.end.y) ||
                   outlinerbetweenepsilon(boundingBox2.start.y, boundingBox1.start.y,boundingBox2.end.y) ||
                   outlinerbetweenepsilon(boundingBox2.start.y, boundingBox1.end.y,  boundingBox2.end.y));
  if (debugbbit3) debugf("        bbi3 yOverlap %u    %.2f..%.2f and %.2f...%.2f",
                         yOverlap,
                         boundingBox1.start.y, boundingBox1.end.y,
                         boundingBox2.start.y, boundingBox2.end.y);
  if (debugbbit3) debugf("        bbi3 yOverlap components %u, %u, %u, %u",
                         outlinerbetweenepsilon(boundingBox1.start.y, boundingBox2.start.y,boundingBox1.end.y),
                         outlinerbetweenepsilon(boundingBox1.start.y, boundingBox2.end.y,  boundingBox1.end.y),
                         outlinerbetweenepsilon(boundingBox2.start.y, boundingBox1.start.y,boundingBox2.end.y),
                         outlinerbetweenepsilon(boundingBox2.start.y, boundingBox1.end.y,  boundingBox2.end.y));
  if (debugbbit3) debugf("        bbi3 yOverlap tests %u %u box1 ydiff %f box12 diff %f",
                         boundingBox1.start.y <= boundingBox2.start.y,
                         boundingBox2.start.y <= boundingBox1.end.y,
                         boundingBox1.end.y - boundingBox1.start.y,
                         boundingBox1.end.y - boundingBox2.start.y);
  if (!yOverlap) return(0);
  bool zOverlap = (outlinerbetweenepsilon(boundingBox1.start.z, boundingBox2.start.z,boundingBox1.end.z) ||
                   outlinerbetweenepsilon(boundingBox1.start.z, boundingBox2.end.z,  boundingBox1.end.z) ||
                   outlinerbetweenepsilon(boundingBox2.start.z, boundingBox1.start.z,boundingBox2.end.z) ||
                   outlinerbetweenepsilon(boundingBox2.start.z, boundingBox1.end.z,  boundingBox2.end.z));
  if (debugbbit3) debugf("        bbi3 zOverlap %u    %.2f..%2.f and %.2f...%.2f",
                         zOverlap,
                         boundingBox1.start.z, boundingBox1.end.z,
                         boundingBox2.start.z, boundingBox2.end.z);
  if (!zOverlap) return(0);
  return(1);
}

bool
OutlinerMath::lineIntersectsVerticalLine2D(const OutlinerLine2D& line,
                                           const OutlinerLine2D& verticalLine,
                                           OutlinerVector2D& intersectionPoint) {
  assert(verticalLine.start.x == verticalLine.end.x);

  // Fetch basic values
  outlinerreal verticalStartY = outlinermin(verticalLine.start.y,verticalLine.end.y);
  outlinerreal verticalEndY = outlinermax(verticalLine.start.y,verticalLine.end.y);
  outlinerreal verticalX = verticalLine.start.x;
  
  // Order line points such that X grows from start to end
  outlinerreal lineStartX;
  outlinerreal lineStartY;
  outlinerreal lineEndX;
  outlinerreal lineEndY;
  if (line.start.x <= line.end.x) {
    lineStartX = line.start.x;
    lineStartY = line.start.y;
    lineEndX = line.end.x;
    lineEndY = line.end.y;
  } else {
    lineStartX = line.end.x;
    lineStartY = line.end.y;
    lineEndX = line.start.x;
    lineEndY = line.start.y;
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
    if (outlineroverlapepsilon(lineStartY,lineEndY,verticalStartY,verticalEndY)) {
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
  if (!outlinerbetweenepsilon(lineStartX,verticalX,lineEndX)) return(0);

  // Calculate resulting Y at vertical line position
  outlinerreal lineY = equationBaseY + verticalLineDifferenceX * equationFactor;

  // Check if the resulting position is within the vertical line Y range
  if (!outlinerbetweenepsilon(verticalStartY,lineY,verticalEndY)) return(0);

  // Lines intersect! Set the intersection point
  intersectionPoint.x = verticalX;
  intersectionPoint.y = lineY;
  return(1);
}

bool
OutlinerMath::lineIntersectsHorizontalLine2D(const OutlinerLine2D& line,
                                             const OutlinerLine2D& horizontalLine,
                                             OutlinerVector2D& intersectionPoint) {
  assert(horizontalLine.start.y == horizontalLine.end.y);
  
  // Fetch basic values
  outlinerreal horizontalStartX = outlinermin(horizontalLine.start.x,horizontalLine.end.x);
  outlinerreal horizontalEndX = outlinermax(horizontalLine.start.x,horizontalLine.end.x);
  outlinerreal horizontalY = horizontalLine.start.y;
  
  // Order line points such that Y grows from start to end
  outlinerreal lineStartX;
  outlinerreal lineStartY;
  outlinerreal lineEndX;
  outlinerreal lineEndY;
  if (line.start.y <= line.end.y) {
    lineStartX = line.start.x;
    lineStartY = line.start.y;
    lineEndX = line.end.x;
    lineEndY = line.end.y;
  } else {
    lineStartX = line.end.x;
    lineStartY = line.end.y;
    lineEndX = line.start.x;
    lineEndY = line.start.y;
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
    if (outlineroverlapepsilon(lineStartX,lineEndX,horizontalStartX,horizontalEndX)) {
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
  if (!outlinerbetweenepsilon(lineStartY,horizontalY,lineEndY)) return(0);

  // Calculate resulting X at horizontal line position
  outlinerreal lineX = equationBaseX + horizontalLineDifferenceY * equationFactor;

  // Check if the resulting position is within the horizontal line X range
  if (!outlinerbetweenepsilon(horizontalStartX,lineX,horizontalEndX)) return(0);

  // Lines intersect! Set the intersection point
  intersectionPoint.x = lineX;
  intersectionPoint.y = horizontalY;
  return(1);
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

  // Sanity checks
  assert(a != 0 && b != 0 && c != 0);
  assert(a != b && a != c && b != c);
  assert(nth0 != 0 && nth1 != 0 && nth2 != 0);
  *nth0 = *nth1 = *nth2 = 0;
  
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
  
  // More sanity checks
  assert(*nth0 != *nth1);
  assert(*nth0 != *nth2);
  assert(*nth1 != *nth2);
  assert((*nth0)->x <= (*nth1)->x);
  assert((*nth1)->x <= (*nth2)->x);
}

void
OutlinerMath::sortVectorsY3D(const OutlinerVector3D* a,
                             const OutlinerVector3D* b,
                             const OutlinerVector3D* c,
                             const OutlinerVector3D** nth0,
                             const OutlinerVector3D** nth1,
                             const OutlinerVector3D** nth2) {
  // Sanity checks
  assert(a != 0 && b != 0 && c != 0);
  assert(a != b && a != c && b != c);
  assert(nth0 != 0 && nth1 != 0 && nth2 != 0);
  *nth0 = *nth1 = *nth2 = 0;
  
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

  // More sanity checks
  assert(*nth0 != *nth1);
  assert(*nth0 != *nth2);
  assert(*nth1 != *nth2);
  assert((*nth0)->y <= (*nth1)->y);
  assert((*nth1)->y <= (*nth2)->y);
}

void
OutlinerMath::sortVectorsZ3D(const OutlinerVector3D* a,
                             const OutlinerVector3D* b,
                             const OutlinerVector3D* c,
                             const OutlinerVector3D** nth0,
                             const OutlinerVector3D** nth1,
                             const OutlinerVector3D** nth2) {
  // Sanity checks
  assert(a != 0 && b != 0 && c != 0);
  assert(a != b && a != c && b != c);
  assert(nth0 != 0 && nth1 != 0 && nth2 != 0);
  *nth0 = *nth1 = *nth2 = 0;
  if (debugbbit3) debugf("        z input range in svz3 %.2f, %.2f, %.2f", a->z, b->z, c->z);
  
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
  
  // More sanity checks
  assert(*nth0 != *nth1);
  assert(*nth0 != *nth2);
  assert(*nth1 != *nth2);
  assert((*nth0)->z <= (*nth1)->z);
  assert((*nth1)->z <= (*nth2)->z);
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
  lineIntersectionTests();
  triangleTests();
  boundingBoxTests();
  boundingBoxIntersectionTests();
  triangleBoundingBoxTests();
  infof("math tests ok");
}

void
OutlinerMath::utilityTests(void) {

  debugf("utility tests...");
  // Between tests
  assert(outlinerbetweenepsilon(1,2,3));
  assert(outlinerbetweenepsilon(1,2.000000001,2));
  assert(!outlinerbetweenepsilon(1,3,2));
  assert(!outlinerbetweenepsilon(3,2,1));

  // Betweenanyorder tests
  assert(outlinerbetweenanyorderepsilon(1,2,3));
  assert(outlinerbetweenanyorderepsilon(1,2,2));
  assert(outlinerbetweenanyorderepsilon(2,2.0000000000003,1));
  assert(!outlinerbetweenanyorderepsilon(1,3,2));
  assert(!outlinerbetweenanyorderepsilon(2,3,1));
  assert(outlinerbetweenanyorderepsilon(3,2,1));

  // Overlap tests
  assert(outlineroverlapepsilon(1,2,2.00000000007,4));
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
  OutlinerVector2D a(0,0);
  OutlinerVector2D b(0,3);
  OutlinerVector2D c(2,0);
  OutlinerBox2D boundingBox;
  debugf("bounding box tests");
  OutlinerTriangle2D tone(a,a,a);
  triangleBoundingBox2D(tone,boundingBox);
  debugf("a,a,a bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBox.start.x, boundingBox.start.y, boundingBox.end.x, boundingBox.end.y);
  assert(boundingBox.start.x == 0 && boundingBox.start.y == 0);
  assert(boundingBox.end.x == 0 && boundingBox.end.y == 0);
  OutlinerTriangle2D t(a,b,c);
  triangleBoundingBox2D(t,boundingBox);
  debugf("a,b,c bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBox.start.x, boundingBox.start.y, boundingBox.end.x, boundingBox.end.y);
  assert(boundingBox.start.x == 0 && boundingBox.start.y == 0);
  assert(boundingBox.end.x == 2 && boundingBox.end.y == 3);
  OutlinerTriangle2D trev(c,b,a);
  triangleBoundingBox2D(trev,boundingBox);
  debugf("c,b,a bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBox.start.x, boundingBox.start.y, boundingBox.end.x, boundingBox.end.y);
  assert(boundingBox.start.x == 0 && boundingBox.start.y == 0);
  assert(boundingBox.end.x == 2 && boundingBox.end.y == 3);
  OutlinerVector2D x(-10,-10);
  OutlinerVector2D y(10,10);
  OutlinerVector2D z(30,9);
  OutlinerTriangle2D trevx(z,y,x);
  triangleBoundingBox2D(trevx,boundingBox);
  debugf("z,y,x bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBox.start.x, boundingBox.start.y, boundingBox.end.x, boundingBox.end.y);
  assert(boundingBox.start.x == -10 && boundingBox.start.y == -10);
  assert(boundingBox.end.x == 30 && boundingBox.end.y == 10);
  OutlinerTriangle2D trevx2(y,z,x);
  triangleBoundingBox2D(trevx2,boundingBox);
  debugf("y,z,x bounding box [%.2f,%.2f] to [%.2f,%.2f]",
         boundingBox.start.x, boundingBox.start.y, boundingBox.end.x, boundingBox.end.y);
  assert(boundingBox.start.x == -10 && boundingBox.start.y == -10);
  assert(boundingBox.end.x == 30 && boundingBox.end.y == 10);

  OutlinerVector2D bbtest1start(10,10);
  OutlinerVector2D bbtest1end(20,30);
  OutlinerBox2D bbtest1(bbtest1start,bbtest1end);
  OutlinerVector2D bbtest1point1(0,25);
  OutlinerVector2D bbtest1point2(10,10);
  OutlinerVector2D bbtest1point3(20,30);
  OutlinerVector2D bbtest1point4(30,30);
  OutlinerVector2D bbtest1point5(11,15);
  bool ans = bbtest1.pointInside(bbtest1point1);
  assert(ans == 0);
  ans = bbtest1.pointInside(bbtest1point2);
  assert(ans == 1);
  ans = bbtest1.pointInside(bbtest1point3);
  assert(ans == 1);
  ans = bbtest1.pointInside(bbtest1point4);
  assert(ans == 0);
  ans = bbtest1.pointInside(bbtest1point5);
  assert(ans == 1);
}

void
OutlinerMath::boundingBoxIntersectionTests(void) {
  infof("bounding box intersection tests...");
  
  OutlinerVector3D test1boundingBox1Start(0,0,0);
  OutlinerVector3D test1boundingBox1End(100,100,100);
  OutlinerVector3D test1boundingBox2Start(10,10,10);
  OutlinerVector3D test1boundingBox2End(11,11,11);
  OutlinerBox3D test1boundingBox1(test1boundingBox1Start,test1boundingBox1End);
  OutlinerBox3D test1boundingBox2(test1boundingBox2Start,test1boundingBox2End);
  bool ans = boundingBoxesIntersect3D(test1boundingBox1,
                                      test1boundingBox2);
  assert(ans);
  
  OutlinerVector3D test2boundingBox1Start(0,0,0);
  OutlinerVector3D test2boundingBox1End(10,10,10);
  OutlinerVector3D test2boundingBox2Start(11,11,11);
  OutlinerVector3D test2boundingBox2End(12,12,12);
  OutlinerBox3D test2boundingBox1(test2boundingBox1Start,test2boundingBox1End);
  OutlinerBox3D test2boundingBox2(test2boundingBox2Start,test2boundingBox2End);
  ans = boundingBoxesIntersect3D(test2boundingBox1,
                                 test2boundingBox2);
  assert(!ans);
  
  OutlinerVector3D test3boundingBox1Start(0,0,0);
  OutlinerVector3D test3boundingBox1End(10,10,10);
  OutlinerVector3D test3boundingBox2Start(0,0,11);
  OutlinerVector3D test3boundingBox2End(2,2,12);
  OutlinerBox3D test3boundingBox1(test3boundingBox1Start,test3boundingBox1End);
  OutlinerBox3D test3boundingBox2(test3boundingBox2Start,test3boundingBox2End);
  ans = boundingBoxesIntersect3D(test3boundingBox1,
                                 test3boundingBox2);
  assert(!ans);

  infof("bounding box intersection tests ok");
}

void
OutlinerMath::lineIntersectionTests() {
  
  debugf("line intersection tests...");
  
  // Vertical line intersection 1
  {
    OutlinerVector2D a(10,10);
    OutlinerVector2D b1(20,20);
    OutlinerVector2D b2(20,15);
    OutlinerVector2D vl1start(1,0);
    OutlinerVector2D vl1end(1,40);
    OutlinerVector2D vl2start(11,0);
    OutlinerVector2D vl2end(11,40);
    OutlinerLine2D ab1(a,b1);
    OutlinerLine2D ab2(a,b2);
    OutlinerLine2D vl1(vl1start,vl1end);
    OutlinerLine2D vl2(vl2start,vl2end);
    OutlinerVector2D inter;
    bool ans;
    ans = lineIntersectsVerticalLine2D(ab1,vl1,inter);
    assert(!ans);
    ans = lineIntersectsVerticalLine2D(ab1,vl2,inter);
    assert(ans);
    deepdebugf("vertical line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 11);
    assert(inter.y == 11);
    ans = lineIntersectsVerticalLine2D(ab2,vl2,inter);
    assert(ans);
    deepdebugf("vertical line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 11);
    assert(inter.y == 10.5);
  }
  
  // Vertical line intersection 2
  {
    OutlinerVector2D a(10,10);
    OutlinerVector2D b(12,20);
    OutlinerVector2D vl1start(11,0);
    OutlinerVector2D vl1end(11,9);
    OutlinerVector2D vl2start(11,0);
    OutlinerVector2D vl2end(11,14);
    OutlinerVector2D vl3start(11,10);
    OutlinerVector2D vl3end(11,15);
    OutlinerVector2D vl4start(11,10);
    OutlinerVector2D vl4end(11,16);
    OutlinerLine2D ab(a,b);
    OutlinerLine2D vl1(vl1start,vl1end);
    OutlinerLine2D vl2(vl2start,vl2end);
    OutlinerLine2D vl3(vl3start,vl3end);
    OutlinerLine2D vl4(vl4start,vl4end);
    OutlinerVector2D inter;
    bool ans;
    ans = lineIntersectsVerticalLine2D(ab,vl1,inter);
    assert(!ans);
    ans = lineIntersectsVerticalLine2D(ab,vl2,inter);
    assert(!ans);
    ans = lineIntersectsVerticalLine2D(ab,vl3,inter);
    assert(ans);
    deepdebugf("vertical line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 11);
    assert(inter.y == 15);
    ans = lineIntersectsVerticalLine2D(ab,vl4,inter);
    assert(ans);
    deepdebugf("vertical line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 11);
    assert(inter.y == 15);
  }
  
  // Vertical line intersection 3 (parallel lines)
  {
    OutlinerVector2D a(10,10);
    OutlinerVector2D b(10,20);
    OutlinerVector2D vl1start(9,15);
    OutlinerVector2D vl1end(9,16);
    OutlinerVector2D vl2start(10,5);
    OutlinerVector2D vl2end(10,9);
    OutlinerVector2D vl3start(10,15);
    OutlinerVector2D vl3end(10,16);
    OutlinerLine2D ab(a,b);
    OutlinerLine2D vl1(vl1start,vl1end);
    OutlinerLine2D vl2(vl2start,vl2end);
    OutlinerLine2D vl3(vl3start,vl3end);
    OutlinerVector2D inter;
    bool ans;
    ans = lineIntersectsVerticalLine2D(ab,vl1,inter);
    assert(!ans);
    ans = lineIntersectsVerticalLine2D(ab,vl2,inter);
    assert(!ans);
    ans = lineIntersectsVerticalLine2D(ab,vl3,inter);
    assert(ans);
    deepdebugf("vertical parallel line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 10);
    assert(inter.y == 15);
  }
  
  // Horizontal line intersection 1
  {
    OutlinerVector2D a(10,10);
    OutlinerVector2D b(20,20);
    OutlinerVector2D hl1start(0,10);
    OutlinerVector2D hl1end(10,10);
    OutlinerVector2D hl2start(0,15);
    OutlinerVector2D hl2end(20,15);
    OutlinerVector2D hl3start(0,21);
    OutlinerVector2D hl3end(20,21);
    OutlinerLine2D ab(a,b);
    OutlinerLine2D hl1(hl1start,hl1end);
    OutlinerLine2D hl2(hl2start,hl2end);
    OutlinerLine2D hl3(hl3start,hl3end);
    OutlinerVector2D inter;
    bool ans;
    ans = lineIntersectsHorizontalLine2D(ab,hl1,inter);
    assert(ans);
    deepdebugf("horizontal line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 10);
    assert(inter.y == 10);
    ans = lineIntersectsHorizontalLine2D(ab,hl2,inter);
    assert(ans);
    deepdebugf("horizontal line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.x == 15);
    assert(inter.y == 15);
    ans = lineIntersectsHorizontalLine2D(ab,hl3,inter);
    assert(!ans);
  }
  
  // Vertical line intersection 2 (parallel lines)
  {
    OutlinerVector2D a(10,10);
    OutlinerVector2D b(20,10);
    OutlinerVector2D hl1start(15,9);
    OutlinerVector2D hl1end(16,9);
    OutlinerVector2D hl2start(5,10);
    OutlinerVector2D hl2end(9,10);
    OutlinerVector2D hl3start(15,10);
    OutlinerVector2D hl3end(16,10);
    OutlinerLine2D ab(a,b);
    OutlinerLine2D hl1(hl1start,hl1end);
    OutlinerLine2D hl2(hl2start,hl2end);
    OutlinerLine2D hl3(hl3start,hl3end);
    OutlinerVector2D inter;
    bool ans;
    ans = lineIntersectsHorizontalLine2D(ab,hl1,inter);
    assert(!ans);
    ans = lineIntersectsHorizontalLine2D(ab,hl2,inter);
    assert(!ans);
    ans = lineIntersectsHorizontalLine2D(ab,hl3,inter);
    assert(ans);
    deepdebugf("horizontal parallel line intersection %.2f, %.2f", inter.x, inter.y);
    assert(inter.y == 10);
    assert(inter.x == 15);
  }
}

void
OutlinerMath::triangleTests(void) {
  debugf("triangle tests (2D)...");
  OutlinerVector2D a(0,0);
  OutlinerVector2D b(0,2);
  OutlinerVector2D c(2,0);
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
  OutlinerVector3D a3(0,0,10);
  OutlinerVector3D b3(0,2,10);
  OutlinerVector3D c3(2,0,10);
  OutlinerVector3D boundingStart3a(0,0,0);
  OutlinerVector3D boundingEnd3a(5,5,5);
  OutlinerBox3D boundingBox3a(boundingStart3a,boundingEnd3a);
  OutlinerVector3D boundingStart3b(0,0,0);
  OutlinerVector3D boundingEnd3b(5,5,10);
  OutlinerBox3D boundingBox3b(boundingStart3b,boundingEnd3b);
  OutlinerTriangle3D t3(a3,b3,c3);
  bool ans3 = boundingBoxIntersectsTriangle3D(t3,boundingBox3a);
  assert(!ans3);
  ans3 = boundingBoxIntersectsTriangle3D(t3,boundingBox3b);
  assert(ans3);
  
  // Bug test for 3D triangle cases
  debugf("triangle tests (3D bug)...");
  OutlinerVector3D bugBoxStart(0.00,-1.00,0.00);
  OutlinerVector3D bugBoxEnd(0.00,-0.50,0.50);
  OutlinerBox3D bugBox(bugBoxStart,bugBoxEnd);
  OutlinerVector3D buga(1.00, -1.00, -1.00);
  OutlinerVector3D bugb(-1.00,-1.00, 1.00);
  OutlinerVector3D bugc(-1.00,-1.00,-1.00);
  OutlinerTriangle3D bugt(buga,bugb,bugc);
  bool ansbug = boundingBoxIntersectsTriangle3D(bugt,bugBox);
  assert(ansbug);

  // Cross section bug tests
  debugf("triangle tests (cross section bug)...");
  OutlinerVector3D ta( 1.000000,-1.000000,-1.000000);
  OutlinerVector3D tb(-1.000000,-1.000000, 1.000000);
  OutlinerVector3D tc(-1.000000,-1.000000,-1.000000);
  OutlinerTriangle3D tr(ta,tb,tc);
  OutlinerBox3D thisBox(0.00,-1.00,0.00,0.00,-0.90,0.10);
  bool crossBugAns = boundingBoxIntersectsTriangle3D(tr,thisBox);
  debugf("crossBugAns = %u", crossBugAns);
  assert(crossBugAns);
  debugf("triangle tests (cross section bug) ok");
}

void
OutlinerMath::triangleBoundingBoxTests(void) {
  infof("triangle bounding box tests..");
  
  OutlinerVector2D a(0,0);
  OutlinerVector2D b(0,10);
  OutlinerVector2D c(20,0);
  OutlinerVector2D degenerate1a(1,1);
  OutlinerVector2D degenerate1b(1,1);
  OutlinerVector2D degenerate1c(-1,1);
  OutlinerVector2D degenerate2a(1,1);
  OutlinerVector2D degenerate2b(1,1);
  OutlinerVector2D degenerate2c(-1,-1);
  OutlinerVector2D box1Start(-10,-10);
  OutlinerVector2D box1End(-1,-1);
  OutlinerBox2D box1(box1Start,box1End);
  OutlinerVector2D box2Start(-10,-10);
  OutlinerVector2D box2End(5,5);
  OutlinerBox2D box2(box2Start,box2End);
  OutlinerVector2D box3Start(-1000,-1000);
  OutlinerVector2D box3End(1000,1000);
  OutlinerBox2D box3(box3Start,box3End);
  OutlinerVector2D box4Start(-10,-10);
  OutlinerVector2D box4End(0,0);
  OutlinerBox2D box4(box4Start,box4End);
  OutlinerVector2D box5Start(0.0,0.0);
  OutlinerVector2D box5End(0.0,0.50);
  OutlinerBox2D box5(box5Start,box5End);
  OutlinerTriangle2D t(a,b,c);
  bool ans = boundingBoxIntersectsTriangle2D(t,box1);
  assert(!ans);
  ans = boundingBoxIntersectsTriangle2D(t,box2);
  assert(ans);
  ans = boundingBoxIntersectsTriangle2D(t,box3);
  assert(ans);
  ans = boundingBoxIntersectsTriangle2D(t,box4);
  assert(ans);
  OutlinerTriangle2D degenerate1t(degenerate1a,degenerate1b,degenerate1c);
  ans = boundingBoxIntersectsTriangle2D(degenerate1t,box5);
  assert(!ans);
  OutlinerTriangle2D degenerate2t(degenerate2a,degenerate2b,degenerate2c);
  ans = boundingBoxIntersectsTriangle2D(degenerate2t,box5);
  assert(ans);
  infof("triangle bounding box tests ok");

  infof("bounding box intersection cross cut bug tests...");
  OutlinerBox3D bug(-1.00, -1.00, -1.00,
                     1.00, -1.00,  1.00);
  OutlinerBox3D pixel( 0.00, -1.00, 0.00,
                       0.00, -0.90, 0.10);
  bool buganswer = boundingBoxesIntersect3D(bug,pixel);
  debugf("buganswer = %u", buganswer);
  assert(buganswer);
  infof("bounding box intersection cross cut bug tests ok");
}

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

#include <cassert>
#include "outlinerconstants.hh"
#include "outlinertypes.hh"
#include "outlinerhighprecision.hh"
#include "outlinerdebug.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Debugs /////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
  
# define  debugreturn(f,why,x) {                                             \
    bool drv = (x);                                                          \
    deepdeepdebugf("%s returns %u due to %s", (f), drv, (why));              \
    return(drv);                                                             \
 }

///////////////////////////////////////////////////////////////////////////////////////////////
// OutlinerVector2D Functions /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
OutlinerVector2D::equal(const OutlinerVector2D& b) const {
  return(x == b.x && y == b.y);
}

void
OutlinerVector2D::test(void) {
  debugf("Vector2D tests...");
  OutlinerVector2D a(1,1);
  OutlinerVector2D b(1,2);
  OutlinerVector2D c(1,1);
  assert(a.equal(b) == 0);
  assert(b.equal(c) == 0);
  assert(a.equal(c) == 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// OutlinerVector3D Functions /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
OutlinerVector3D::equal(const OutlinerVector3D& b) const {
  return(x == b.x && y == b.y && z == b.z);
}

void
OutlinerVector3D::test(void) {
  debugf("Vector3D tests...");
  OutlinerVector3D a(1,1,0);
  OutlinerVector3D b(1,2,0);
  OutlinerVector3D c(1,1,0);
  assert(a.equal(b) == 0);
  assert(b.equal(c) == 0);
  assert(a.equal(c) == 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// OutlinerLine2D Functions ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
OutlinerLine2D::pointOnLine(const OutlinerVector2D& point) const {
  
  // Debugs
  deepdeepdebugf("          pointOnLine2D high precision (%.2f,%.2f)-(%.2f,%.2f) vs. (%.2f,%.2f)",
                 start.x, start.y, end.x, end.y,
                 point.x, point.y);
  
  // Check for a special case: line points are equal, resulting in
  // comparing to a point, not a line.
  
  if (start.equal(end))  {
    debugreturn("          pol2","line points equal",start.equal(point));
  }

  // Check for a special case: line is horizontal
  if (start.y == end.y) {
    if (point.y != start.y) debugreturn("          pol","horizontal y diff",0);
    debugreturn("          pol2","line is horizontal",outlinerbetweenanyorderepsilon(start.x,point.x,end.x));
  }
  
  // Check for a special case: line is vertical
  if (start.x == end.x) {
    if (point.x != start.x) debugreturn("          pol2","vertical x diff",0);
    debugreturn("          pol2","line is vertical",outlinerbetweenanyorderepsilon(start.y,point.y,end.y));
  }
  
  // Not a special case. Run the general check, taking algorithm from
  // https://stackoverflow.com/questions/17692922/check-is-a-point-x-y-is-between-two-points-drawn-on-a-straight-line

  float alpha1 = (point.x - start.x) / (end.x - start.x);
  float alpha2 = (point.y - start.y) / (end.y - start.y);
  if (alpha1 != alpha2) debugreturn("          pol2","alpha diff",0);
  if (alpha1 < 0) debugreturn("          pol2","alpha neg",0);
  if (alpha1 > 1) debugreturn("          pol2","alpha above one",0);
  debugreturn("          pol2","fallthrough",1);
}

void
OutlinerLine2D::test(void) {
  debugf("line tests...");
  
  // Horizontal
  {
    OutlinerVector2D a(0,0);
    OutlinerVector2D b(1,0);
    OutlinerLine2D ab(a,b);
    OutlinerVector2D c(2,0);
    OutlinerLine2D ac(a,c);
    OutlinerVector2D d(0.5,2);
    bool ans = ac.pointOnLine(d);
    assert(ans == 0);
    ans = ab.pointOnLine(c);
    assert(ans == 0);
    ans = ac.pointOnLine(b);
    assert(ans == 1);
  }
  
  // Vertical
  {
    OutlinerVector2D a(0,0);
    OutlinerVector2D b(0,1);
    OutlinerLine2D ab(a,b);
    OutlinerVector2D c(0,2);
    OutlinerLine2D ac(a,c);
    OutlinerVector2D d(0.5,1);
    bool ans = ac.pointOnLine(d);
    assert(ans == 0);
    ans = ab.pointOnLine(c);
    assert(ans == 0);
    ans = ac.pointOnLine(b);
    assert(ans == 1);
  }
  
  // Sloping line
  {
    OutlinerVector2D a(0,0);
    OutlinerVector2D b(1,1);
    OutlinerLine2D ab(a,b);
    OutlinerVector2D c(2,2);
    OutlinerLine2D ac(a,c);
    OutlinerVector2D d(1,2);
    bool ans = ac.pointOnLine(d);
    assert(ans == 0);
    ans = ab.pointOnLine(c);
    assert(ans == 0);
    ans = ac.pointOnLine(b);
    assert(ans == 1);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// OutlinerLine3D Functions ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
OutlinerLine3D::test(void) {
}

///////////////////////////////////////////////////////////////////////////////////////////////
// OutlinerBox1D Functions ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
OutlinerBox1D::equal(const OutlinerBox1D& box2) const {
  return(start == box2.start &&
         end == box2.end);
}

bool
OutlinerBox1D::pointInside(const outlinerreal point) const {
  return(point >= start && point <= end);
}

bool
OutlinerBox1D::doesIntersect(const OutlinerBox1D& boundingBox2) const {
  return(outlineroverlapepsilon(start,end,boundingBox2.start,boundingBox2.end));
}

void
OutlinerBox1D::intersection(const OutlinerBox1D& box2,
                            OutlinerBox1D& resultBox) const {
  resultBox.start = start;
  resultBox.end = end;
  if (box2.start > resultBox.start) resultBox.start = box2.start;
  if (box2.end < resultBox.end) resultBox.end = box2.end;
}

void
OutlinerBox1D::boxUnion(const OutlinerBox1D& box2,
                        OutlinerBox1D& resultBox) const {
  resultBox.start = start;
  resultBox.end = end;
  if (box2.start < resultBox.start) resultBox.start = box2.start;
  if (box2.end > resultBox.end) resultBox.end = box2.end;
}

void
OutlinerBox1D::test(void) {
  OutlinerBox1D range1(1,2);
  assert(!range1.pointInside(0));
  assert(range1.pointInside(1));
  assert(range1.pointInside(1.5));
  assert(range1.pointInside(2));
  assert(!range1.pointInside(3));
  OutlinerBox1D range2(2,3);
  assert(range1.equal(range1));
  assert(!range1.equal(range2));
  OutlinerBox1D range3;
  range1.intersection(range2,range3);
  assert(range3.start == 2);
  assert(range3.end == 2);
  range1.boxUnion(range2,range3);
  assert(range3.start == 1);
  assert(range3.end == 3);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// OutlinerBox2D Functions ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
OutlinerBox2D::equal(const OutlinerBox2D& box2) const {
  return(start.x == box2.start.x &&
         start.y == box2.start.y &&
         end.x == box2.end.x &&
         end.y == box2.end.y);
}

bool
OutlinerBox2D::pointInside(const OutlinerVector2D& point) const {
  return(point.x >= start.x && point.x <= end.x &&
         point.y >= start.y && point.y <= end.y);
}

bool
OutlinerBox2D::doesIntersect(const OutlinerBox2D& boundingBox2) const {
  // Following the algorithm from
  // https://math.stackexchange.com/questions/2651710/simplest-way-to-determine-if-two-3d-boxes-intersect
  
  bool xOverlap = (outlinerbetweenepsilon(             start.x,boundingBox2.start.x,end.x) ||
                   outlinerbetweenepsilon(             start.x,boundingBox2.end.x,  end.x) ||
                   outlinerbetweenepsilon(boundingBox2.start.x,             start.x,boundingBox2.end.x) ||
                   outlinerbetweenepsilon(boundingBox2.start.x,             end.x,  boundingBox2.end.x));
  if (!xOverlap) return(0);
  bool yOverlap = (outlinerbetweenepsilon(             start.y,boundingBox2.start.y,end.y) ||
                   outlinerbetweenepsilon(             start.y,boundingBox2.end.y,  end.y) ||
                   outlinerbetweenepsilon(boundingBox2.start.y,             start.y,boundingBox2.end.y) ||
                   outlinerbetweenepsilon(boundingBox2.start.y,              end.y, boundingBox2.end.y));
  if (!yOverlap) return(0);
  
  return(1);
}

void
OutlinerBox2D::intersection(const OutlinerBox2D& box2,
                            OutlinerBox2D& resultBox) const {
  resultBox.start = start;
  resultBox.end = end;
  if (box2.start.x > resultBox.start.x) resultBox.start.x = box2.start.x;
  if (box2.start.y > resultBox.start.y) resultBox.start.y = box2.start.y;
  if (box2.end.x < resultBox.end.x) resultBox.end.x = box2.end.x;
  if (box2.end.y < resultBox.end.y) resultBox.end.y = box2.end.y;
}

void
OutlinerBox2D::boxUnion(const OutlinerBox2D& box2,
                        OutlinerBox2D& resultBox) const {
  resultBox.start = start;
  resultBox.end = end;
  if (box2.start.x < resultBox.start.x) resultBox.start.x = box2.start.x;
  if (box2.start.y < resultBox.start.y) resultBox.start.y = box2.start.y;
  if (box2.end.x > resultBox.end.x) resultBox.end.x = box2.end.x;
  if (box2.end.y > resultBox.end.y) resultBox.end.y = box2.end.y;
}

void
OutlinerBox2D::test(void) {
  testEqual();
  testUnion();
  testIntersectionTest();
  testDoIntersection();
}

void
OutlinerBox2D::testEqual(void) {
  debugf("bounding box equal  tests...");
  const OutlinerVector2D box1Start(0,0);
  const OutlinerVector2D box1End(10,10);
  const OutlinerBox2D box1(box1Start,box1End);
  const OutlinerVector2D box2Start(3,0);
  const OutlinerVector2D box2End(7,6);
  const OutlinerBox2D box2(box2Start,box2End);
  const OutlinerVector2D box3Start(0,0);
  const OutlinerVector2D box3End(10,10);
  const OutlinerBox2D box3(box3Start,box3End);
  const OutlinerVector2D box4Start(0,-1);
  const OutlinerVector2D box4End(10,10);
  const OutlinerBox2D box4(box4Start,box4End);
  bool ans;
  ans = box1.equal(box2);
  assert(!ans);
  ans = box1.equal(box3);
  assert(ans);
  ans = box1.equal(box4);
  assert(!ans);
  debugf("bounding box equal tests ok");
}

void
OutlinerBox2D::testUnion(void) {
  
  infof("bounding box union tests...");
  
  const OutlinerVector2D box1aStart(0,0);
  const OutlinerVector2D box1aEnd(10,10);
  const OutlinerBox2D box1a(box1aStart,box1aEnd);
  const OutlinerVector2D box1bStart(-3,0);
  const OutlinerVector2D box1bEnd(7,16);
  const OutlinerBox2D box1b(box1bStart,box1bEnd);
  const OutlinerVector2D box2Start(2,2);
  const OutlinerVector2D box2End(8,8);
  const OutlinerBox2D box2(box2Start,box2End);
  OutlinerBox2D resultBox;
  box1a.boxUnion(box2,resultBox);
  deepdebugf("union result (%.2f,%.2f)-(%.2f,%.2f)",
             resultBox.start.x, resultBox.start.y,
             resultBox.end.x, resultBox.end.y);
  assert(resultBox.start.x == 0 && resultBox.start.y == 0);
  assert(resultBox.end.x == 10 && resultBox.end.y == 10);
  box1b.boxUnion(box2,resultBox);
  deepdebugf("union result (%.2f,%.2f)-(%.2f,%.2f)",
             resultBox.start.x, resultBox.start.y,
             resultBox.end.x, resultBox.end.y);
  assert(resultBox.start.x == -3 && resultBox.start.y == 0);
  assert(resultBox.end.x == 8 && resultBox.end.y == 16);
  
  infof("bounding box union tests ok");
}

void
OutlinerBox2D::testIntersectionTest(void) {
  infof("bounding box intersection tests...");
  
  OutlinerVector3D test1boundingBox1Start(0,0,0);
  OutlinerVector3D test1boundingBox1End(100,100,100);
  OutlinerVector3D test1boundingBox2Start(10,10,10);
  OutlinerVector3D test1boundingBox2End(11,11,11);
  OutlinerBox3D test1boundingBox1(test1boundingBox1Start,test1boundingBox1End);
  OutlinerBox3D test1boundingBox2(test1boundingBox2Start,test1boundingBox2End);
  bool ans = test1boundingBox1.doesIntersect(test1boundingBox2);
  assert(ans);
  
  OutlinerVector3D test2boundingBox1Start(0,0,0);
  OutlinerVector3D test2boundingBox1End(10,10,10);
  OutlinerVector3D test2boundingBox2Start(11,11,11);
  OutlinerVector3D test2boundingBox2End(12,12,12);
  OutlinerBox3D test2boundingBox1(test2boundingBox1Start,test2boundingBox1End);
  OutlinerBox3D test2boundingBox2(test2boundingBox2Start,test2boundingBox2End);
  ans = test2boundingBox1.doesIntersect(test2boundingBox2);
  assert(!ans);
  
  OutlinerVector3D test3boundingBox1Start(0,0,0);
  OutlinerVector3D test3boundingBox1End(10,10,10);
  OutlinerVector3D test3boundingBox2Start(0,0,11);
  OutlinerVector3D test3boundingBox2End(2,2,12);
  OutlinerBox3D test3boundingBox1(test3boundingBox1Start,test3boundingBox1End);
  OutlinerBox3D test3boundingBox2(test3boundingBox2Start,test3boundingBox2End);
  ans = test3boundingBox1.doesIntersect(test3boundingBox2);
  assert(!ans);

  infof("bounding box intersection tests ok");
  
  infof("bounding box intersection cross cut bug tests...");
  OutlinerBox3D bug(-1.00, -1.00, -1.00,
                     1.00, -1.00,  1.00);
  OutlinerBox3D pixel( 0.00, -1.00, 0.00,
                       0.00, -0.90, 0.10);
  bool buganswer = bug.doesIntersect(pixel);
  debugf("buganswer = %u", buganswer);
  assert(buganswer);
  infof("bounding box intersection cross cut bug tests ok");
}

void
OutlinerBox2D::testDoIntersection(void) {

  const OutlinerVector2D box1aStart(0,0);
  const OutlinerVector2D box1aEnd(10,10);
  const OutlinerVector2D box1bStart(3,0);
  const OutlinerVector2D box1bEnd(7,6);
  const OutlinerVector2D box2Start(2,2);
  const OutlinerVector2D box2End(8,8);
  const OutlinerBox2D box1a(box1aStart,box1aEnd);
  const OutlinerBox2D box1b(box1bStart,box1bEnd);
  const OutlinerBox2D box2(box2Start,box2End);
  OutlinerBox2D resultBox;
  box1a.intersection(box2,resultBox);
  assert(resultBox.start.x == 2 && resultBox.start.y == 2);
  assert(resultBox.end.x == 8 && resultBox.end.y == 8);
  box1b.intersection(box2,resultBox);
  assert(resultBox.start.x == 3 && resultBox.start.y == 2);
  assert(resultBox.end.x == 7 && resultBox.end.y == 6);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// OutlinerBox3D Functions ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
OutlinerBox3D::equal(const OutlinerBox3D& box2) const {
  return(start.x == box2.start.x &&
         start.y == box2.start.y &&
         start.z == box2.start.z &&
         end.x == box2.end.x &&
         end.y == box2.end.y &&
         end.z == box2.end.z);
}

void
OutlinerBox3D::intersection(const OutlinerBox3D& box2,
                            OutlinerBox3D& resultBox) const {
  resultBox.start = start;
  resultBox.end = end;
  if (box2.start.x > resultBox.start.x) resultBox.start.x = box2.start.x;
  if (box2.start.y > resultBox.start.y) resultBox.start.y = box2.start.y;
  if (box2.start.z > resultBox.start.z) resultBox.start.z = box2.start.z;
  if (box2.end.x < resultBox.end.x) resultBox.end.x = box2.end.x;
  if (box2.end.y < resultBox.end.y) resultBox.end.y = box2.end.y;
  if (box2.end.z < resultBox.end.z) resultBox.end.z = box2.end.z;
}

void
OutlinerBox3D::boxUnion(const OutlinerBox3D& box2,
                        OutlinerBox3D& resultBox) const {
  resultBox.start = start;
  resultBox.end = end;
  if (box2.start.x < resultBox.start.x) resultBox.start.x = box2.start.x;
  if (box2.start.y < resultBox.start.y) resultBox.start.y = box2.start.y;
  if (box2.start.z < resultBox.start.z) resultBox.start.z = box2.start.z;
  if (box2.end.x > resultBox.end.x) resultBox.end.x = box2.end.x;
  if (box2.end.y > resultBox.end.y) resultBox.end.y = box2.end.y;
  if (box2.end.z > resultBox.end.z) resultBox.end.z = box2.end.z;
}

void
OutlinerBox3D::test(void) {
  testEqual();
  testUnion();
  testIntersectionTest();
  testDoIntersection();
}

void
OutlinerBox3D::testEqual(void) {
  OutlinerBox3D a(1,2,3,4,5,6);
  OutlinerBox3D b(1,2,3,4,5,6);
  OutlinerBox3D c(1,2,4,4,5,7);
  assert(a.equal(a));
  assert(a.equal(b));
  assert(!a.equal(c));
}

void
OutlinerBox3D::testUnion(void) {
  OutlinerBox3D a(0,0,0,10,10,10);
  OutlinerBox3D b(0,0,0,20,20,20);
  OutlinerBox3D result;
  a.boxUnion(b,result);
  assert(result.equal(b));
}

void
OutlinerBox3D::testIntersectionTest(void) {
  infof("bounding box intersection tests...");
  
  OutlinerVector3D test1boundingBox1Start(0,0,0);
  OutlinerVector3D test1boundingBox1End(100,100,100);
  OutlinerVector3D test1boundingBox2Start(10,10,10);
  OutlinerVector3D test1boundingBox2End(11,11,11);
  OutlinerBox3D test1boundingBox1(test1boundingBox1Start,test1boundingBox1End);
  OutlinerBox3D test1boundingBox2(test1boundingBox2Start,test1boundingBox2End);
  bool ans = test1boundingBox1.doesIntersect(test1boundingBox2);
  assert(ans);
  
  OutlinerVector3D test2boundingBox1Start(0,0,0);
  OutlinerVector3D test2boundingBox1End(10,10,10);
  OutlinerVector3D test2boundingBox2Start(11,11,11);
  OutlinerVector3D test2boundingBox2End(12,12,12);
  OutlinerBox3D test2boundingBox1(test2boundingBox1Start,test2boundingBox1End);
  OutlinerBox3D test2boundingBox2(test2boundingBox2Start,test2boundingBox2End);
  ans = test2boundingBox1.doesIntersect(test2boundingBox2);
  assert(!ans);
  
  OutlinerVector3D test3boundingBox1Start(0,0,0);
  OutlinerVector3D test3boundingBox1End(10,10,10);
  OutlinerVector3D test3boundingBox2Start(0,0,11);
  OutlinerVector3D test3boundingBox2End(2,2,12);
  OutlinerBox3D test3boundingBox1(test3boundingBox1Start,test3boundingBox1End);
  OutlinerBox3D test3boundingBox2(test3boundingBox2Start,test3boundingBox2End);
  ans = test3boundingBox1.doesIntersect(test3boundingBox2);
  assert(!ans);

  infof("bounding box intersection tests ok");
  
  infof("bounding box intersection cross cut bug tests...");
  OutlinerBox3D bug(-1.00, -1.00, -1.00,
                     1.00, -1.00,  1.00);
  OutlinerBox3D pixel( 0.00, -1.00, 0.00,
                       0.00, -0.90, 0.10);
  bool buganswer = bug.doesIntersect(pixel);
  debugf("buganswer = %u", buganswer);
  assert(buganswer);
  infof("bounding box intersection cross cut bug tests ok");
}

void
OutlinerBox3D::testDoIntersection(void) {
  OutlinerBox3D a(0,0,0,10,10,10);
  OutlinerBox3D b(-2,-2,-7,20,30,20);
  OutlinerBox3D result;
  a.intersection(b,result);
  assert(result.equal(a));
}


///////////////////////////////////////////////////////////////////////////////////////////////
// OutlinerTriangle2D Functions ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
OutlinerTriangle2D::test(void) {
}

///////////////////////////////////////////////////////////////////////////////////////////////
// OutlinerTriangle3D Functions ///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
OutlinerTriangle3D::test(void) {
}



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
  testIntersection();
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
OutlinerBox2D::testIntersection(void) {

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



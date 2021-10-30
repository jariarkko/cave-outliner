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
#include "outlinerhighprecision.hh"
#include "outlinerdebug.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// OutlinerVector2D Functions /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
OutlinerVector2D::test(void) {
}

///////////////////////////////////////////////////////////////////////////////////////////////
// OutlinerVector3D Functions /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
OutlinerVector3D::test(void) {
}

///////////////////////////////////////////////////////////////////////////////////////////////
// OutlinerLine2D Functions ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
OutlinerLine2D::test(void) {
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
  resultBox = *this;
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



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

class OutlinerVector2D {
public:
  outlinerreal x;
  outlinerreal y;
  OutlinerVector2D() { x = y = 0; }
  OutlinerVector2D(const aiVector2D& in) : x(in.x), y(in.y) {}
  OutlinerVector2D(const OutlinerVector2D& in) : x(in.x), y(in.y) {}
  OutlinerVector2D(const outlinerreal xIn,
                   const outlinerreal yIn) : x(xIn), y(yIn) {}
};

class OutlinerVector3D {
public:
  outlinerreal x;
  outlinerreal y;
  outlinerreal z;
  OutlinerVector3D() { x = y = 0; z = 0; }
  OutlinerVector3D(const aiVector3D& in) : x(in.x), y(in.y), z(in.z) {}
  OutlinerVector3D(const OutlinerVector3D& in) : x(in.x), y(in.y), z(in.z) {}
  OutlinerVector3D(const outlinerreal xIn,
                   const outlinerreal yIn,
                   const outlinerreal zIn) : x(xIn), y(yIn), z(zIn) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Lines //////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class OutlinerLine2D {
public:
  OutlinerVector2D start;
  OutlinerVector2D end;
  OutlinerLine2D() {}
  OutlinerLine2D(const OutlinerLine2D& in) : start(in.start), end(in.end) {}
  OutlinerLine2D(const OutlinerVector2D& startIn,
                 const OutlinerVector2D& endIn) : start(startIn), end(endIn) {}
};

class OutlinerLine3D {
public:
  OutlinerVector3D start;
  OutlinerVector3D end;
  OutlinerLine3D() {}
  OutlinerLine3D(const OutlinerLine3D& in) : start(in.start), end(in.end) {}
  OutlinerLine3D(const OutlinerVector3D& startIn,
                 const OutlinerVector3D& endIn) : start(startIn), end(endIn) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Bounding boxes /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class OutlinerBox2D {
public:
  OutlinerVector2D start;
  OutlinerVector2D end;
  OutlinerBox2D() {}
  OutlinerBox2D(const OutlinerLine2D& in) : start(in.start), end(in.end) {}
  OutlinerBox2D(const OutlinerVector2D& startIn,
                const OutlinerVector2D& endIn) : start(startIn), end(endIn) {}
};

class OutlinerBox3D {
public:
  OutlinerVector3D start;
  OutlinerVector3D end;
  OutlinerBox3D() {}
  OutlinerBox3D(const OutlinerBox3D& in) : start(in.start), end(in.end) {}
  OutlinerBox3D(const OutlinerVector3D& startIn,
                const OutlinerVector3D& endIn) : start(startIn), end(endIn) {}
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Triangles //////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class OutlinerTriangle2D {
public:
  OutlinerVector2D a;
  OutlinerVector2D b;
  OutlinerVector2D c;
  OutlinerTriangle2D() { a.x = a.y = 0; b.x = b.y = 0; c.x = c.y = 0; }
  OutlinerTriangle2D(const OutlinerTriangle2D& in) : a(in.a), b(in.b), c(in.c) {}
  OutlinerTriangle2D(const OutlinerVector2D& aIn,
                     const OutlinerVector2D& bIn,
                     const OutlinerVector2D& cIn) : a(aIn), b(bIn), c(cIn) {}
};

class OutlinerTriangle3D {
public:
  OutlinerVector3D a;
  OutlinerVector3D b;
  OutlinerVector3D c;
  OutlinerTriangle3D() { a.x = a.y = a.z = 0; b.x = b.y = b.z = 0; c.x = c.y = c.z = 0; }
  OutlinerTriangle3D(const OutlinerTriangle3D& in) : a(in.a), b(in.b), c(in.c) {}
  OutlinerTriangle3D(const OutlinerVector3D& aIn,
                     const OutlinerVector3D& bIn,
                     const OutlinerVector3D& cIn) : a(aIn), b(bIn), c(cIn) {}
};

#endif // OUTLINERHIGHPRECISION_HH

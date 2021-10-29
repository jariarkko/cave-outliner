
#ifndef OUTLINERHIGHPRECISION_HH
#define OUTLINERHIGHPRECISION_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "outlinertypes.hh"

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

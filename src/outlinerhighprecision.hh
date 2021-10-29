
#ifndef OUTLINERHIGHPRECISION_HH
#define OUTLINERHIGHPRECISION_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "outlinertypes.hh"

class HighPrecisionVector2D {
public:
  outlinerhighprecisionreal x;
  outlinerhighprecisionreal y;
  HighPrecisionVector2D() { x = y = 0; }
  HighPrecisionVector2D(const aiVector2D& in) : x(in.x), y(in.y) {}
  HighPrecisionVector2D(const HighPrecisionVector2D& in) : x(in.x), y(in.y) {}
  HighPrecisionVector2D(const outlinerhighprecisionreal xIn,
                        const outlinerhighprecisionreal yIn) : x(xIn), y(yIn) {}
};

class HighPrecisionVector3D {
public:
  outlinerhighprecisionreal x;
  outlinerhighprecisionreal y;
  outlinerhighprecisionreal z;
  HighPrecisionVector3D() { x = y = 0; z = 0; }
  HighPrecisionVector3D(const aiVector3D& in) : x(in.x), y(in.y), z(in.z) {}
  HighPrecisionVector3D(const HighPrecisionVector3D& in) : x(in.x), y(in.y), z(in.z) {}
  HighPrecisionVector3D(const outlinerhighprecisionreal xIn,
                        const outlinerhighprecisionreal yIn,
                        const outlinerhighprecisionreal zIn) : x(xIn), y(yIn), z(zIn) {}
};

class HighPrecisionTriangle2D {
public:
  HighPrecisionVector2D a;
  HighPrecisionVector2D b;
  HighPrecisionVector2D c;
  HighPrecisionTriangle2D() { a.x = a.y = 0; b.x = b.y = 0; c.x = c.y = 0; }
  HighPrecisionTriangle2D(const HighPrecisionTriangle2D& in) : a(in.a), b(in.b), c(in.c) {}
  HighPrecisionTriangle2D(const HighPrecisionVector2D& aIn,
                          const HighPrecisionVector2D& bIn,
                          const HighPrecisionVector2D& cIn) : a(aIn), b(bIn), c(cIn) {}
};

class HighPrecisionTriangle3D {
public:
  HighPrecisionVector3D a;
  HighPrecisionVector3D b;
  HighPrecisionVector3D c;
  HighPrecisionTriangle3D() { a.x = a.y = a.z = 0; b.x = b.y = b.z = 0; c.x = c.y = c.z = 0; }
  HighPrecisionTriangle3D(const HighPrecisionTriangle3D& in) : a(in.a), b(in.b), c(in.c) {}
  HighPrecisionTriangle3D(const HighPrecisionVector3D& aIn,
                          const HighPrecisionVector3D& bIn,
                          const HighPrecisionVector3D& cIn) : a(aIn), b(bIn), c(cIn) {}
};

#endif // OUTLINERHIGHPRECISION_HH

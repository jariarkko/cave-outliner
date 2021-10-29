
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

#endif // OUTLINERHIGHPRECISION_HH

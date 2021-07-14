
#ifndef OUTLINERMATH_HH
#define OUTLINERMATH_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void triangleBoundingBox2D(const aiVector2D* a,
                           const aiVector2D* b,
                           const aiVector2D* c,
                           aiVector2D* boundingBoxStart,
                           aiVector2D* boundingBoxEnd);
bool pointInsideTriangle2D(const aiVector2D* a,
                           const aiVector2D* b,
                           const aiVector2D* c,
                           const aiVector2D* point);
void vectorTo(const aiVector2D* from,
              const aiVector2D* to,
              aiVector2D* result);
float determinant2x2(const aiVector2D* u,
                     const aiVector2D* v);
void mathTests(void);

#endif // OUTLINERMATH_HH

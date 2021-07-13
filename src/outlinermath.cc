
#include <stdlib.h>
#include <assert.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinermath.hh"
#include "outlinerdebug.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Function prototypes ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static void vectorTests(void);
static void detTests(void);
static void triangleTests(void);

///////////////////////////////////////////////////////////////////////////////////////////////
// Math functions /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
pointInsideTriangle2D(const aiVector2D* triangleA,
                      const aiVector2D* triangleB,
                      const aiVector2D* triangleC,
                      const aiVector2D* point) {
  // Algorithm from https://mathworld.wolfram.com/TriangleInterior.html
  aiVector2D v = *point; 
  aiVector2D v0 = *triangleA;
  aiVector2D v1; vectorTo(triangleA,triangleB,&v1);
  aiVector2D v2; vectorTo(triangleA,triangleC,&v2);
  deepdebugf("triangle v = (%.2f,%.2f)", v.x, v.y);
  deepdebugf("triangle v0 = (%.2f,%.2f)", v0.x, v0.y);
  deepdebugf("triangle v1 = (%.2f,%.2f)", v1.x, v1.y);
  deepdebugf("triangle v2 = (%.2f,%.2f)", v2.x, v2.y);
  float a = (determinant2x2(&v,&v2) - determinant2x2(&v0,&v2)) / determinant2x2(&v1,&v2);
  float b = -((determinant2x2(&v,&v1) - determinant2x2(&v0,&v1)) / determinant2x2(&v1,&v2));
  deepdebugf("triangle check a %.2f b %.2f a+b %.2f", a, b, a+b);
  return(a >= 0 && b >= 0 && a+b <= 1);
}

void
vectorTo(const aiVector2D* from,
         const aiVector2D* to,
         aiVector2D* result) {
  *result = *to;
  result->x -= from->x;
  result->y -= from->y;
}

float
determinant2x2(const aiVector2D* u,
               const aiVector2D* v) {
  return(u->x * v->y - u->y * v->x);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Test functions /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
mathTests(void) {
  debugf("running math tests");
  vectorTests();
  detTests();
  triangleTests();
}

static void
vectorTests(void) {
  aiVector2D a(2,2);
  aiVector2D b(3,3);
  aiVector2D result;
  vectorTo(&a,&b,&result);
  deepdebugf("vector test: result: (%f,%f)", result.x, result.y);
  assert(result.x == 1);
  assert(result.y == 1);
}

static void
detTests(void) {
  aiVector2D C1(4,2);
  aiVector2D C2(1,3);
  ai_real result = determinant2x2(&C1,&C2);
  deepdebugf("determinant result = %.2f", result);
  assert(result == 10);
}

static void
triangleTests(void) {
  aiVector2D a(0,0);
  aiVector2D b(0,2);
  aiVector2D c(2,0);
  aiVector2D pointfar(2,2);
  aiVector2D pointnear(0.5,0.5);
  aiVector2D pointverynear(0.1,0.2);
  aiVector2D pointata = a;
  aiVector2D pointatb = b;
  aiVector2D pointatc = c;
  aiVector2D pointbeyondb = b;
  pointbeyondb.x += 0.01;
  pointbeyondb.y += 0.01;
  aiVector2D pointbefore1(-0.001,0);
  aiVector2D pointbefore2(0,-0.001);
  aiVector2D pointbefore3(-0.001,-0.001);
  bool ansfar = pointInsideTriangle2D(&a,&b,&c,&pointfar);
  deepdebugf("triangle test: pointfar = %u", ansfar);
  bool ansnear = pointInsideTriangle2D(&a,&b,&c,&pointnear);
  deepdebugf("triangle test: pointnear = %u", ansnear);
  bool ansverynear = pointInsideTriangle2D(&a,&b,&c,&pointverynear);
  deepdebugf("triangle test: pointverynear = %u", ansverynear);
  bool ansata = pointInsideTriangle2D(&a,&b,&c,&pointata);
  deepdebugf("triangle test: pointata = %u", ansata);
  bool ansatb = pointInsideTriangle2D(&a,&b,&c,&pointatb);
  deepdebugf("triangle test: pointatb = %u", ansatb);
  bool ansatc = pointInsideTriangle2D(&a,&b,&c,&pointatc);
  deepdebugf("triangle test: pointatc = %u", ansatc);
  bool ansbeyondb = pointInsideTriangle2D(&a,&b,&c,&pointbeyondb);
  deepdebugf("triangle test: pointbeyondb = %u", ansbeyondb);
  bool ansbefore1 = pointInsideTriangle2D(&a,&b,&c,&pointbefore1);
  deepdebugf("triangle test: pointbefore1 = %u", ansbefore1);
  bool ansbefore2 = pointInsideTriangle2D(&a,&b,&c,&pointbefore2);
  deepdebugf("triangle test: pointbefore2 = %u", ansbefore2);
  bool ansbefore3 = pointInsideTriangle2D(&a,&b,&c,&pointbefore3);
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
}

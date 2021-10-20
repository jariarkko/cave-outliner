
#ifndef OUTLINERTYPES_HH
#define OUTLINERTYPES_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////////////////
// Defines ////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define outlinerhighprecisionreal double

///////////////////////////////////////////////////////////////////////////////////////////////
// Enumerated types ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

enum outlineralgorithm {
  alg_pixel,
  alg_borderpixel,
  alg_borderline,
  alg_borderactual
};

#define outlineralgorithm_generatespicture(a) \
  ((a) == alg_pixel ||  (a) == alg_borderpixel ||  (a) == alg_borderline ||  (a) == alg_borderactual)

///////////////////////////////////////////////////////////////////////////////////////////////
// Common macros //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define outlinerisnumber(s)  ((isdigit(*(s)) || (*(s) == '-' && isdigit(*((s)+1))) || (*(s) == '+' && isdigit(*((s)+1))) ))
#define outlinermin(a,b)     (((a) < (b)) ? (a) : (b))
#define outlinermax(a,b)     (((a) > (b)) ? (a) : (b))
#define outlinermin3(a,b,c)  (outlinermin((a),outlinermin((b),(c))))
#define outlinermax3(a,b,c)  (outlinermax((a),outlinermax((b),(c))))
#define outlinersaneindex(x) ((x) < (2U*1000U*1000U*1000U))

///////////////////////////////////////////////////////////////////////////////////////////////
// Math common utilities //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define outlinerbetweenanyorder(a,b,c)    ((a) < (c) ? outlinerbetween(a,b,c) :                           \
                                                       outlinerbetween(c,b,a))
#define outlinerbetween(a,b,c)            ((a) <= (b) && (b) <= (c))
#define outlineroverlapanyorder(a,b,c,d)  (((a) <= (b) && (c) <= (d)) ? outlineroverlap(a,b,c,d) :         \
                                           (((b) <= (a) && (c) <= (d)) ? outlineroverlap(b,a,c,d) :        \
                                            (((b) <= (a) && (d) <= (c)) ? outlineroverlap(b,a,d,c) :       \
                                             outlineroverlap(a,b,d,c))))
#define outlineroverlap(a,b,c,d)          ((((a) <= (c)) && ((c) <= (b))) ||                               \
                                           (((c) <= (a)) && ((a) <= (d))))

#endif // OUTLINERTYPES_HH

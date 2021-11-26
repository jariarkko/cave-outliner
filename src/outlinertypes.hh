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

#ifndef OUTLINERTYPES_HH
#define OUTLINERTYPES_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "outlinerconstants.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Defines ////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define outlinerreal double

///////////////////////////////////////////////////////////////////////////////////////////////
// Enumerated types ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

enum outlineralgorithm {
  alg_pixel,
  alg_pixelform,
  alg_triangle,
  alg_depthmap,
  alg_depthdiffmap,
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
#define outlinerabs(x)       ((x) < 0 ? -(x) : (x))

///////////////////////////////////////////////////////////////////////////////////////////////
// Math common utilities //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define outlinerle(a,b)                         ((a) <= (b))
#define outlinerleepsilon(a,b)                  ((a) <= ((b)+outlinerepsilon))
#define outlinerge(a,b)                         (((a)+outlinerepsilon) >= (b))
#define outlinergeepsilon(a,b)                  (((a)+outlinerepsilon) >= (b))
#define outlinerbetweenanyorder(a,b,c)          ((a) < (c) ? outlinerbetween(a,b,c) :                               \
                                                             outlinerbetween(c,b,a))
#define outlinerbetweenanyorderepsilon(a,b,c)   ((a) < (c) ? outlinerbetweenepsilon(a,b,c) :                        \
                                                             outlinerbetweenepsilon(c,b,a))
#define outlinerbetween(a,b,c)                  ((a) <= (b) && (b) <= (c))
#define outlinerbetweenepsilon(a,b,c)           ((a) <= ((b)+outlinerepsilon) && (b) <= ((c)+outlinerepsilon))
#define outlineroverlapanyorder(a,b,c,d)        (((a) <= (b) && (c) <= (d)) ? outlineroverlap(a,b,c,d) :            \
                                                 (((b) <= (a) && (c) <= (d)) ? outlineroverlap(b,a,c,d) :           \
                                                 (((b) <= (a) && (d) <= (c)) ? outlineroverlap(b,a,d,c) :           \
                                                                               outlineroverlap(a,b,d,c))))
#define outlineroverlap(a,b,c,d)                ((((a) <= (c)) && ((c) <= (b))) ||                                  \
                                                 (((c) <= (a)) && ((a) <= (d))))
#define outlineroverlapepsilon(a,b,c,d)         ((((a) <= (c)+outlinerepsilon) && ((c) <= (b)+outlinerepsilon)) ||  \
                                                 (((c) <= (a)+outlinerepsilon) && ((a) <= (d)+outlinerepsilon)))

#endif // OUTLINERTYPES_HH

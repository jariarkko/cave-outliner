
#ifndef OUTLINERTYPES_HH
#define OUTLINERTYPES_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Defines ////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define OUTLINER_PROG "cave-outliner"
#define OUTLINER_ERRPREFIX OUTLINER_PROG ": Error: "
#define OUTLINER_DEBUGPREFIX OUTLINER_PROG ": Debug: "

///////////////////////////////////////////////////////////////////////////////////////////////
// Enumerated types ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

enum outlinerdirection {
  dir_horizontal,
  dir_vertical
};

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

#define outlinermin(a,b)    (((a) < (b)) ? (a) : (b))
#define outlinermax(a,b)   (((a) > (b)) ? (a) : (b))

#endif // OUTLINERTYPES_HH

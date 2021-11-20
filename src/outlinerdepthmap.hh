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

#ifndef OUTLINERDEPTHMAP_HH
#define OUTLINERDEPTHMAP_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include "outlinersvg.hh"
class MaterialMatrix2D;
class Processor;

///////////////////////////////////////////////////////////////////////////////////////////////
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

typedef uint8_t outlinerdepth;

///
/// This object represents a "depth map", i.e., an object representing
/// how far from the viewer various parts of an image are. This image
/// is usually taken from a cave plan view perspective.
///
/// Note that the material matrix resolution is not the same as tile
/// resolution in the indexed mesh object. Typically, the indexed mesh
/// has low resolution, just sufficient for fast searches, while the
/// depth matrix needs to support the desired high resolution of
/// the output.
///

class DepthMap {

public:

  /// Create a Depthmap object.
  DepthMap(const unsigned int xIndexSizeIn,
           const unsigned int yIndexSizeIn,
           const MaterialMatrix2D& materialMatrixIn);
  
  /// Destruct the material matrix.
  ~DepthMap();

  /// Set the depth of the material in a given (x,y) index in the
  /// matrix.
  void setDepth(const unsigned int xIndex,
                const unsigned int yIndex,
                const outlinerdepth depth);

  /// Get the depth of the material in a given (x,y) index in the
  /// matrix.
  outlinerdepth getDepth(const unsigned int xIndex,
                         const unsigned int yIndex) const;
  
  /// Get a color code representing the depth of a given (x,y) index in
  /// the matrix.
  OutlinerSvgStyle depthToColor(const unsigned int xIndex,
                                const unsigned int yIndex) const;
  
  /// Get a color code representing the relative depth of a given
  /// (x,y) index compared to its neighbours in the matrix.
  OutlinerSvgStyle depthDiffToColor(const unsigned int xIndex,
                                    const unsigned int yIndex,
                                    const Processor& proc) const;
  
  /// Run unit tests for this module.
  static void test(void);

  /// Matrix size in x-coordinate direction.
  const unsigned int xIndexSize;
  
  /// Matrix size in y-coordinate direction.
  const unsigned int yIndexSize;

private:

  const unsigned int fullSize;
  outlinerdepth* data;
  const MaterialMatrix2D& materialMatrix;
  
};

#endif // OUTLINERDEPTHMAP_HH

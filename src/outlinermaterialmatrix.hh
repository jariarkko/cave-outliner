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

#ifndef OUTLINERMATERIALMATRIX_HH
#define OUTLINERMATERIALMATRIX_HH

#include "outlinerdirection.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This object represents a 2D view into a 3D cave model. Given a
/// desired resolution (N x M pixels), it provides a boolean
/// matrix. Each element (x,y) in the matrix is set to 1 if there's a
/// a face within the model in that pixel location. The resolution is
/// configurable, so for instance for a 1 x 1 resolution all faces
/// within the 3D model would be at the resulting single pixel.
///
/// Note that the material matrix resolution is not the same as tile
/// resolution in the indexed mesh object. Typically, the indexed mesh
/// has low resolution, just sufficient for fast searches, while the
/// material matrix needs to support the desired high resolution of
/// the output.
///

class MaterialMatrix {

public:

  /// Create a MaterialMatrix object.
  MaterialMatrix(OutlinerBox2D boundingbox,
                 outlinerreal stepx,
                 outlinerreal stepy);

  /// Destruct the material matrix.
  ~MaterialMatrix();

  /// Set the flag indicating that there is material in a given (x,y)
  /// index in the matrix.
  void setMaterialMatrix(unsigned int xIndex,
                         unsigned int yIndex);

  /// Get the flag indicating whether there is material in a given
  /// (x,y) index in the matrix.
  bool getMaterialMatrix(unsigned int xIndex,
                         unsigned int yIndex);

  /// Count the number of matrix elements with flags on.
  unsigned int count(void);

  /// Run unit tests for this module.
  static void test(void);

  /// Matrix size in x-coordinate direction.
  unsigned int xIndexSize;
  
  /// Matrix size in y-coordinate direction.
  unsigned int yIndexSize;

private:

  unsigned int nBits;
  unsigned int nChars;
  unsigned char* bitMatrix;

};

#endif // OUTLINERMATERIALMATRIX_HH

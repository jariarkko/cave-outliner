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

#ifndef OUTLINERMATERIALMATRIX2D_HH
#define OUTLINERMATERIALMATRIX2D_HH

#include "outlinerdirection.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This object represents a quantized 2D view into a 3D cave
/// model. Given a desired resolution (N x M pixels), it provides a
/// boolean matrix. Each element (x,y) in the matrix is set to 1 if
/// there's a a face within the model in that pixel location. The
/// resolution is configurable, so for instance for a 1 x 1 resolution
/// all faces within the 3D model would be at the resulting single
/// pixel.
///
/// Note that the material matrix resolution is not the same as tile
/// resolution in the indexed mesh object. Typically, the indexed mesh
/// has low resolution, just sufficient for fast searches, while the
/// material matrix needs to support the desired high resolution of
/// the output.
///

class MaterialMatrix2D {

public:

  //
  // Construction and destruction
  //
  
  /// Create a MaterialMatrix2D object.
  MaterialMatrix2D(const OutlinerBox2D& boundingBoxIn,
                   const outlinerreal stepxIn,
                   const outlinerreal stepyIn);

  /// Destruct the material matrix.
  ~MaterialMatrix2D();

  //
  // Basic operations to set and read the matrix contents
  //
  
  /// Set the flag indicating that there is material in a given (x,y)
  /// index in the matrix.
  void setMaterialMatrix(const unsigned int xIndex,
                         const unsigned int yIndex);

  /// Unset (reset) the flag indicating that there is material in a
  /// given (x,y) index in the matrix.
  void unsetMaterialMatrix(const unsigned int xIndex,
                           const unsigned int yIndex);

  /// Get the flag indicating whether there is material in a given
  /// (x,y) index in the matrix.
  bool getMaterialMatrix(const unsigned int xIndex,
                         const unsigned int yIndex) const;
  
  /// Get the flag indicating whether there is material in a given
  /// (xStart,yStart)..(xEnd,yEnd) rectangle in the matrix.
  bool getMaterialMatrixRange(const unsigned int xIndexStart,
                              const unsigned int yIndexStart,
                              const unsigned int xIndexEnd,
                              const unsigned int yIndexEnd) const;
  
  /// Find bounds of material at the y index range, on a given x index
  bool getMaterialYBounds(const unsigned int xIndex,
                          unsigned int& yIndexFrom,
                          unsigned int& yIndexTo) const;
  
  //
  // Coordinate management
  //
  
  /// Calculate size needed to represent from...to with increments of
  /// size step.
  static unsigned int calculateSize(const outlinerreal from,
                                    const outlinerreal to,
                                    const outlinerreal step,
                                    unsigned int multiplier,
                                    const char* what);
  
  /// Map an actual coordinate value to an index.
  unsigned int coordinateXToIndex(const outlinerreal x) const;
  
  /// Map an actual coordinate value to an index.
  unsigned int coordinateYToIndex(const outlinerreal y) const;
  
  /// Map a coordinate index to an actual coordinate value.
  outlinerreal indexToCoordinateX(const unsigned int xIndex) const;
  
  /// Map a coordinate index to an actual coordinate value.
  outlinerreal indexToCoordinateY(const unsigned int yIndex) const;
  
  //
  // Statistics
  //
  
  /// Count the number of matrix elements with flags on.
  unsigned int count(void) const;

  //
  // Tests
  //
  
  /// Run unit tests for this module.
  static void test(void);

  //
  //  Accessible variables
  //
  
  // Matrix bounding box in the actual coordinates
  const OutlinerBox2D boundingBox;
  
  /// Matrix size in x-coordinate direction.
  const unsigned int xIndexSize;
  
  /// Matrix size in y-coordinate direction.
  const unsigned int yIndexSize;
  
private:

  //
  // Private variables
  //
  
  const outlinerreal stepx;
  const outlinerreal stepy;
  const unsigned int nBits;
  const unsigned int nChars;
  unsigned char* bitMatrix;

};

#endif // OUTLINERMATERIALMATRIX2D_HH

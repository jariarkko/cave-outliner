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

#ifndef OUTLINERMATERIALMATRIX3D_HH
#define OUTLINERMATERIALMATRIX3D_HH

#include "outlinerdirection.hh"
#include "outlinermaterialmatrix2d.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class VerticalMatrix {

public:
  
  OutlinerBox2D matrixBoundingBox;
  unsigned int yIndexOffset;
  unsigned int zIndexOffset;
  MaterialMatrix2D* matrix;
  
  VerticalMatrix();
  ~VerticalMatrix();
};

///
/// This object represents a quantized 3D view into a 3D cave
/// model. Given a desired resolution (N x M x L pixels), it provides
/// a boolean matrix. Each element (x,y,z) in the matrix is set to 1
/// if there's a a face within the model in that pixel location. The
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

class MaterialMatrix3D {

public:

  //
  // Construction and destruction
  //
  
  /// Create a MaterialMatrix3D object.
  MaterialMatrix3D(const OutlinerBox3D& boundingbox,
                   const outlinerreal stepxIn,
                   const outlinerreal stepyIn,
                   const outlinerreal stepzIn,
                   const unsigned int multiplier);

  /// Destruct the material matrix.
  ~MaterialMatrix3D();

  //
  // Basic operations to set and read the matrix contents
  //
  
  /// Set the flag indicating that there is material in a given (x,y)
  /// index in the matrix.
  void setMaterialMatrix(const unsigned int xIndex,
                         const unsigned int yIndex,
                         const unsigned int zIndex);

  /// Set the entire z slice in one go
  void setMaterialMatrixSlice(const unsigned int xIndex,
                              const OutlinerBox2D& sliceBoundingBox,
                              MaterialMatrix2D* sliceMatrix);

  /// Get the flag indicating whether there is material in a given
  /// (x,y) index in the matrix.
  bool getMaterialMatrix(const unsigned int xIndex,
                         const unsigned int yIndex,
                         const unsigned int zIndex) const;

  //
  // Coordinate operations
  //
  
  /// Map an actual coordinate value to an index.
  unsigned int coordinateXToIndex(outlinerreal x) const;
  
  /// Map an actual coordinate value to an index.
  unsigned int coordinateYToIndex(outlinerreal y) const;
  
  /// Map an actual coordinate value to an index.
  unsigned int coordinateZToIndex(outlinerreal z) const;
  
  /// Map a coordinate index to an actual coordinate value.
  outlinerreal indexToCoordinateX(unsigned int xIndex) const;
  
  /// Map a coordinate index to an actual coordinate value.
  outlinerreal indexToCoordinateY(unsigned int yIndex) const;
  
  /// Map a coordinate index to an actual coordinate value.
  outlinerreal indexToCoordinateZ(unsigned int zIndex) const;

  //
  // Statistics
  //
  
  /// Count the number of matrix elements with flags on.
  unsigned int count(void) const;
  
  /// Percentage of the 3D matrix actually constructed (the rest
  /// consumes no memory).
  outlinerreal filledPercentage(unsigned int& memory,
                                unsigned int& theoretical) const;

  //
  // Testing
  //
  
  /// Run unit tests for this module.
  static void test(void);

  //
  // Public variables
  //
  
  // Matrix bounding box in the actual coordinates
  const OutlinerBox3D boundingBox;
  
  /// Matrix size in x-coordinate direction.
  const unsigned int xIndexSize;
  
  /// Matrix size in y-coordinate direction.
  const unsigned int yIndexSize;

  /// Matrix size in z-coordinate direction.
  const unsigned int zIndexSize;
  
private:
  
  //
  // Private variables
  //
  
  const outlinerreal stepx;
  const outlinerreal stepy;
  const outlinerreal stepz;
  class VerticalMatrix* verticalMatrixes;
};

#endif // OUTLINERMATERIALMATRIX3D_HH

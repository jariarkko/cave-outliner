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

#ifndef OUTLINERFORMMATRIX2D_HH
#define OUTLINERFORMMATRIX2D_HH

#include "outlinerdirection.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define outlinerform                   uint8_t // 4 bits
#define outlinerform_mainform              0x7
#define outlinerform_mainform_empty          0 // No material at all
#define outlinerform_mainform_tunnel         1 // Regular tunnel, i.e.,
                                               // roof and air and floor
#define outlinerform_mainform_tunnel_stalac  2 // Regular tunnel, with a detected 
                                               // stalactite or stalagmite
#define outlinerform_mainform_degenerate     3 // Some material, but only one
                                               // block of material (exit, border, etc)
#define outlinerform_mainform_complex        4 // More than two layers of material
                                               // and air (complex cave, multi-level tunnels, ...)
#define outlinerform_max                  0x04

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

class FormMatrix2D {

public:

  /// Create a Formmatrix2d object.
  FormMatrix2D(unsigned int xIndexSizeIn,
               unsigned int yIndexSizeIn);
  
  /// Destruct the material matrix.
  ~FormMatrix2D();

  /// Set the flag indicating that there is material in a given (x,y)
  /// index in the matrix.
  void setForm(unsigned int xIndex,
               unsigned int yIndex,
               outlinerform form);

  /// Get the flag indicating whether there is material in a given
  /// (x,y) index in the matrix.
  outlinerform getForm(unsigned int xIndex,
                       unsigned int yIndex);
  
  /// Run unit tests for this module.
  static void test(void);

  /// Matrix size in x-coordinate direction.
  unsigned int xIndexSize;
  
  /// Matrix size in y-coordinate direction.
  unsigned int yIndexSize;

private:

  unsigned int fullSizeNibbles;
  unsigned int fullSizeChars;
  uint8_t* data;
};

#endif // OUTLINERFORMMATRIX2D_HH

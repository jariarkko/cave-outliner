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

class MaterialMatrix {

public:
  MaterialMatrix(OutlinerVector2D boundingboxstart,
                 OutlinerVector2D boundingboxend,
                 outlinerreal stepx,
                 outlinerreal stepy);
  ~MaterialMatrix();
  void setMaterialMatrix(unsigned int xIndex,
                         unsigned int yIndex);
  bool getMaterialMatrix(unsigned int xIndex,
                         unsigned int yIndex);
  unsigned int count(void);
  static void test(void);
  
  unsigned int xIndexSize;
  unsigned int yIndexSize;

private:

  unsigned int nBits;
  unsigned int nChars;
  unsigned char* bitMatrix;

};

#endif // OUTLINERMATERIALMATRIX_HH

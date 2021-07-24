
#ifndef OUTLINERMATERIALMATRIX_HH
#define OUTLINERMATERIALMATRIX_HH

#include "outlinerdirection.hh"

class MaterialMatrix {

public:
  MaterialMatrix(HighPrecisionVector3D boundingboxstart,
                 HighPrecisionVector3D boundingboxend,
                 enum outlinerdirection directionIn,
                 float stepx,
                 float stepy);
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

  enum outlinerdirection direction;
  unsigned int nBits;
  unsigned int nChars;
  unsigned char* bitMatrix;

};

#endif // OUTLINERMATERIALMATRIX_HH

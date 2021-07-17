
#ifndef OUTLINERMATERIALMATRIX_HH
#define OUTLINERMATERIALMATRIX_HH

class MaterialMatrix {

public:
  MaterialMatrix(aiVector3D boundingboxstart,
                 aiVector3D boundingboxend,
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
  
  unsigned char* bitMatrix;

};

#endif // OUTLINERMATERIALMATRIX_HH

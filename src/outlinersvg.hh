
#ifndef SVG_HH
#define SVG_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class SvgCreator {
  
 public:
  
  SvgCreator(const char* fileName,
             unsigned int xSize, 
             unsigned int ySize,
             float xStart,
             float yStart,
             float xFactor,
             float yFactor,
             unsigned int linewidth);
  ~SvgCreator();
  void line(float fromX,
            float fromY,
            float toX,
            float toY);
  void pixel(float x,
             float y);
  bool ok();
  
 private:

  std::ofstream file;
  unsigned int xSize;
  unsigned int ySize;
  float xStart;
  float yStart;
  float xFactor;
  float yFactor;
  unsigned int linewidth;

  void preamble();
  void postamble();
  void coordinateNormalization(float x,
                               float y,
                               unsigned int& xInt,
                               unsigned int& yInt);
};

#endif // SVG_HH

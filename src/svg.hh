
#ifndef SVG_HH
#define SVG_HH

#include <iostream>
#include <fstream>

class SvgCreator {
  
 public:
  
  SvgCreator(const char* fileName,
             unsigned int xSize,
             unsigned int ySize);
  ~SvgCreator();
  void line(unsigned int fromX,
            unsigned int fromY,
            unsigned int toX,
            unsigned int toY);
  void pixel(unsigned int x,
             unsigned int y);
  bool ok();
  
 private:

  ofstream file;
  unsigned int xSize;
  unsigned int ySize;

  void preamble();
  void postamble();
  
};

#endif // SVG_HH


#include "svg.hh"

SvgCreator::SvgCreator(const char* fileName,
                       unsigned int xSize,
                       unsigned int ySize) {
  std::cerr << "SvgCreator::SvgCreator xs=" << xSize << " ys=" << ySize;
  this->file.open(fileName);
  this->xSize = xSize;
  this->ySize = ySize;
  preamble();
}

SvgCreator::~SvgCreator() {
  postamble();
  file.close();
}

void SvgCreator::line(unsigned int fromX,
                      unsigned int fromY,
                      unsigned int toX,
                      unsigned int toY) {
}
  
void SvgCreator::pixel(unsigned int x,
                       unsigned int y) {
  file << "<rect x=\"" << x << "\" y=\"" << y << "\" width=\"1\" height=\"1\" fill=\"none\" stroke=\"black\" />\n";
}

bool SvgCreator::ok() {
  return(xSize > 0 && ySize > 0 && file.good());
}
  
void SvgCreator::preamble() {
  // Basics for all SVGs
  file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n";
  file << "          \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
  file << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\"\n";
  file << "     width=\"" << xSize << "\" height=\"" << ySize << "\">\n";

  // Make the background white
  file << "<rect x=\"0\" y=\"0\" width=\"" << xSize << "\" height=\"" << ySize << "\" fill=\"white\" stroke=\"white\" stroke-width=\"0\"/>\n";
}

void SvgCreator::postamble() {
  // Just close the full SVG XML
  file << "</svg>\n";
}

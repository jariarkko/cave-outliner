
///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdebug.hh"
#include "outlinersvg.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class methods //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

SvgCreator::SvgCreator(const char* fileName,
                       unsigned int xSize,
                       unsigned int ySize,
                       float xStart,
                       float yStart,
                       float xFactor,
                       float yFactor) {
  this->file.open(fileName);
  this->xSize = xSize;
  this->ySize = ySize;
  this->xStart = xStart;
  this->yStart = yStart;
  this->xFactor = xFactor;
  this->yFactor = yFactor;
  debugf("coordinate normalization starts (%.2f,%.2f) factors (%f,%f)",
         xStart, yStart, xFactor, yFactor);
  preamble();
}

SvgCreator::~SvgCreator() {
  postamble();
  file.close();
}

void
SvgCreator::line(float fromX,
                 float fromY,
                 float toX,
                 float toY) {
  unsigned int fromXInt;
  unsigned int fromYInt;
  coordinateNormalization(fromX,fromY,fromXInt,fromYInt);
  
  unsigned int toXInt;
  unsigned int toYInt;
  coordinateNormalization(toX,toY,toXInt,toYInt);

  file << "<line x1=\"" << fromXInt << "\" y1=\"" << fromYInt << "\"";
  file << "<line x2=\"" << toXInt << "\" y2=\"" << toYInt << "\"";
  file << " style=\"stroke:black;stroke-width=1\" />\n";
}
  
void
SvgCreator::pixel(float x,
                  float y) {
  unsigned int xInt;
  unsigned int yInt;
  coordinateNormalization(x,y,xInt,yInt);
  file << "<rect x=\"" << xInt << "\" y=\"" << yInt << "\" width=\"1\" height=\"1\" fill=\"none\" stroke=\"black\" />\n";
}

void
SvgCreator::coordinateNormalization(float x,
                                    float y,
                                    unsigned int& xInt,
                                    unsigned int& yInt) {
  float xNormalized = (x - xStart) * xFactor;
  float yNormalized = (y - yStart) * yFactor;
  xInt = xNormalized;
  yInt = ySize - yNormalized;
  deepdebugf("coordinate normalization (%.2f,%.2f) to (%u,%u)",
             x, y,
             xInt, yInt);
}

bool
SvgCreator::ok() {
  return(xSize > 0 && ySize > 0 && file.good());
}
  
void
SvgCreator::preamble() {
  // Basics for all SVGs
  file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n";
  file << "          \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
  file << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\"\n";
  file << "     width=\"" << xSize << "\" height=\"" << ySize << "\">\n";

  // Make the background white
  file << "<rect x=\"0\" y=\"0\" width=\"" << xSize << "\" height=\"" << ySize << "\" fill=\"white\" stroke=\"white\" stroke-width=\"0\"/>\n";
}

void
SvgCreator::postamble() {
  // Just close the full SVG XML
  file << "</svg>\n";
}

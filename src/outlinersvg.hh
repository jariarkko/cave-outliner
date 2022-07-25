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

#ifndef SVG_HH
#define SVG_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include "outlinertypes.hh"
#include "outlinerhighprecision.hh"
#include "outlinersvgoptions.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define OutlinerSvgMaxLineSegments  511
#define OutlinerSvgMaxLinePoints    (OutlinerSvgMaxLineSegments+1)

#define OutlinerRgb                 uint8_t

#define OutlinerSvgStyle            uint32_t
#define outlinersvgstyle_none       0x00000
#define outlinersvgstyle_dashed     0x00100
#define outlinersvgstyle_stubs      0x00200
#define outlinersvgstyle_stubs_dirl 0x00400
#define outlinersvgstyle_stubs_dird 0x00800
#define outlinersvgstyle_ends       0x01000
#define outlinersvgstyle_grey       0x02000
#define outlinersvgstyle_greyval(x) (outlinersvgstyle_grey+((x)&0xff))
#define outlinersvgstyle_greyget(s) ((s)&0xff)
#define outlinersvgstyle_red        0x04000
#define outlinersvgstyle_blue       0x08000
#define outlinersvgstyle_green      0x10000
#define outlinersvgstyle_yellow     0x20000
#define outlinersvgstyle_cyan       0x40000
#define outlinersvgstyle_legal      (outlinersvgstyle_dashed +       \
                                     outlinersvgstyle_stubs +        \
                                     outlinersvgstyle_stubs_dirl +   \
                                     outlinersvgstyle_stubs_dird +   \
                                     outlinersvgstyle_ends +         \
                                     outlinersvgstyle_grey +         \
                                     outlinersvgstyle_red +          \
                                     outlinersvgstyle_blue +         \
                                     outlinersvgstyle_green +        \
                                     outlinersvgstyle_yellow +       \
                                     outlinersvgstyle_cyan)
#define outlinersvgstyle_basemask   0x000fff00
#define outlinersvgstyle_getbase(s) ((s)&outlinersvgstyle_basemask)
#define outlinersvgstyle_illegal    (~(outlinersvgstyle_legal))

struct OutlinerSvgCoord {
  unsigned int x;
  unsigned int y;
};

struct OutlinerSvgLine {
  unsigned int refCount;
  OutlinerSvgStyle style;
  unsigned int nPoints;
  bool printed;
  struct OutlinerSvgCoord points[OutlinerSvgMaxLinePoints];
};

struct OutlinerSvgLineList {
  struct OutlinerSvgLine* line;
  struct OutlinerSvgLineList* next;
};

class SvgReader;

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This object can be used to output SVG images to a file. For a
/// description of what an SVG image format is, see
/// https://www.w3schools.com/graphics/svg_intro.asp.
///

class SvgCreator {
  
 public:

  /// Create an SVG file with given file name and size.
  SvgCreator(const char* fileNameIn,
             const unsigned int xSizeIn, 
             const unsigned int ySizeIn,
             const outlinerreal xStartIn,
             const outlinerreal yStartIn,
             const outlinerreal xFactorIn,
             const outlinerreal yFactorIn,
             const SvgOptions& optionsIn);

  /// Release all resources associated with the SVG creation object. And close the file, if still open.
  ~SvgCreator();

  /// Draw a line.
  void line(outlinerreal fromX,
            outlinerreal fromY,
            outlinerreal toX,
            outlinerreal toY,
            OutlinerSvgStyle style = outlinersvgstyle_none);

  /// Draw a pixel.
  void pixel(outlinerreal x,
             outlinerreal y,
             OutlinerSvgStyle style = outlinersvgstyle_none);

  /// Draw a triangle.
  void triangle(OutlinerTriangle2D triangle,
                OutlinerSvgStyle style = outlinersvgstyle_none);

  /// Write text to the image.
  void text(outlinerreal x,
            outlinerreal y,
            const char* string,
            unsigned int fontSize = outlinerdefaultfont,
            unsigned int rotate = 0);

  /// Get the x-direction size of each individual pixel, expressed in
  /// the 3D model coordinate system. For instance, if each pixel
  /// represents a 1 meter by 1 meter square, then return 1.0.
  outlinerreal getPixelXSize(void);
  
  /// Get the y-direction size of each individual pixel, expressed in
  /// the 3D model coordinate system. For instance, if each pixel
  /// represents a 1 meter by 1 meter square, then return 1.0.
  outlinerreal getPixelYSize(void);

  /// Get the actual x coordinate in the resulting SVG image based on
  /// the floating point coordinate value.
  void getCoordinateXPixel(const outlinerreal x,
			   unsigned int& xPixel) const;

  /// Get the actual y coordinate in the resulting SVG image based on
  /// the floating point coordinate value.
  void getCoordinateYPixel(const outlinerreal y,
			   unsigned int& yPixel) const;

  /// Verify that image creation and file write was successful.
  bool ok();

  /// Unit tests
  static void test(void);
  
 private:
  
  friend class SvgStacker;
  
  const char* fileName;
  std::ofstream file;
  unsigned int xSize;
  unsigned int ySize;
  const unsigned int xSizeMultiplied;
  const unsigned int ySizeMultiplied;
  const outlinerreal xStart;
  const outlinerreal yStart;
  const outlinerreal xFactor;
  const outlinerreal yFactor;
  const SvgOptions options;
  unsigned int pixels;
  unsigned int originalLines;
  unsigned int finalLines;
  unsigned int joins;
  unsigned int strings;
  unsigned int characters;
  
  unsigned int lineTableSize;
  struct OutlinerSvgLineList** lineTable;
  
  const char* colorBasedOnStyle(OutlinerSvgStyle style) const;
  static void preamble(std::ofstream& outputFile,
		       const unsigned int xSizeImage,
		       const unsigned int ySizeImage,
		       const SvgOptions& options,
		       const bool setBackground);
  static void postamble(std::ofstream& outputFile);
  void coordinateNormalization(const outlinerreal x,
                               const outlinerreal y,
                               unsigned int& xInt,
                               unsigned int& yInt) const;
  void addLine(const unsigned int x1In,
               const unsigned int y1In,
               const unsigned int x2In,
               const unsigned int y2In,
               const OutlinerSvgStyle style);
  void emitLine(const struct OutlinerSvgLine& line);
  void emitLineAux(const struct OutlinerSvgLine& line);
  void emitStubsLine(const struct OutlinerSvgLine& line);
  void smoothPoint(const unsigned int pointIndex,
                   struct OutlinerSvgLine& line) const;
  void smoothLine(struct OutlinerSvgLine& line) const;
  
  void lineTableInit(void);
  void lineTableDeinit(void);
  struct OutlinerSvgLine* matchingLine(unsigned int x1,
                                       unsigned int y1,
                                       unsigned int x2,
                                       unsigned int y2,
                                       OutlinerSvgStyle style,
                                       unsigned int& matchIndex,
                                       bool& matchesStart,
                                       bool& reverseOriginal);
  struct OutlinerSvgLine* matchingLineJoin(const struct OutlinerSvgLine* target,
                                           const unsigned int maxPoints,
                                           const unsigned int x,
                                           const unsigned int y,
                                           const bool fromStart);
  struct OutlinerSvgLine* matchingLineAux(const unsigned int x,
                                          const unsigned int y,
                                          const OutlinerSvgStyle style,
                                          const bool lookForTailMatch,
                                          const unsigned int index);
  struct OutlinerSvgLine* matchingLineAuxAvoid(const struct OutlinerSvgLine* avoid,
                                               const unsigned int maxPoints,
                                               const unsigned int x,
                                               const unsigned int y,
                                               const OutlinerSvgStyle style,
                                               const bool lookForTailMatch,
                                               const unsigned int index);
  void lineTableJoin(struct OutlinerSvgLine* entry,
                     struct OutlinerSvgLine* join,
                     bool fromStart,
                     unsigned int entryIndex);
  void lineTableEntryAdd(struct OutlinerSvgLine* entry);
  void lineTableEntryLink(struct OutlinerSvgLine* entry,
                          unsigned int index);
  void lineTableEntryUnlink(struct OutlinerSvgLine* entry,
                            unsigned int index);
  void lineTableEntryDelete(struct OutlinerSvgLineList*& entry);
  void lineTableIndexes(const struct OutlinerSvgLine& line,
                        unsigned int& head,
                        unsigned int& tail);
  unsigned int lineTableIndex(unsigned int x,
                              unsigned int y);
  void lineTableOutput(void);
  void lineTableInfos(void);
  
  void smoothingTest(void);
 };

#endif // SVG_HH

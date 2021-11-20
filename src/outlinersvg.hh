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

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define OutlinerSvgMaxLineSegments  511
#define OutlinerSvgMaxLinePoints    (OutlinerSvgMaxLineSegments+1)

#define OutlinerSvgStyle            uint16_t
#define outlinersvgstyle_none       0x0000
#define outlinersvgstyle_dashed     0x0100
#define outlinersvgstyle_ends       0x0200
#define outlinersvgstyle_grey       0x0400
#define outlinersvgstyle_greyval(x) (outlinersvgstyle_grey+((x)&0xff))
#define outlinersvgstyle_red        0x0800
#define outlinersvgstyle_blue       0x1000
#define outlinersvgstyle_green      0x2000
#define outlinersvgstyle_yellow     0x4000
#define outlinersvgstyle_legal      (outlinersvgstyle_dashed +  \
                                     outlinersvgstyle_ends +    \
                                     outlinersvgstyle_grey + \
                                     outlinersvgstyle_red +     \
                                     outlinersvgstyle_blue +    \
                                     outlinersvgstyle_green +   \
                                     outlinersvgstyle_yellow)
#define outlinersvgstyle_basemask   0xff00
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
  SvgCreator(const char* fileName,
             unsigned int xSize, 
             unsigned int ySize,
             unsigned int multiplier,
             outlinerreal xStart,
             outlinerreal yStart,
             outlinerreal xFactor,
             outlinerreal yFactor,
             bool smooth,
             bool mergedLines,
             outlinerreal linewidth);

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

  /// Verify that image creation and file write was successful.
  bool ok();
  
 private:

  std::ofstream file;
  unsigned int xSize;
  unsigned int ySize;
  unsigned int multiplier;
  unsigned int xSizeMultiplied;
  unsigned int ySizeMultiplied;
  outlinerreal xStart;
  outlinerreal yStart;
  outlinerreal xFactor;
  outlinerreal yFactor;
  bool smooth;
  bool mergedLines;
  outlinerreal linewidth;
  unsigned int pixels;
  unsigned int originalLines;
  unsigned int finalLines;
  unsigned int joins;
  unsigned int strings;
  unsigned int characters;
  
  unsigned int lineTableSize;
  struct OutlinerSvgLineList** lineTable;
  
  const char* colorBasedOnStyle(OutlinerSvgStyle style) const;
  void preamble();
  void postamble();
  void coordinateNormalization(outlinerreal x,
                               outlinerreal y,
                               unsigned int& xInt,
                               unsigned int& yInt);
  void addLine(unsigned int x1,
               unsigned int y1,
               unsigned int x2,
               unsigned int y2,
               OutlinerSvgStyle style);
  void emitLine(const struct OutlinerSvgLine& line);

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
  struct OutlinerSvgLine* matchingLineJoin(struct OutlinerSvgLine* target,
                                           unsigned int x,
                                           unsigned int y,
                                           bool fromStart);
  struct OutlinerSvgLine* matchingLineAux(unsigned int x,
                                          unsigned int y,
                                          OutlinerSvgStyle style,
                                          bool lookForTailMatch,
                                          unsigned int index);
  struct OutlinerSvgLine* matchingLineAuxAvoid(struct OutlinerSvgLine* avoid,
                                               unsigned int x,
                                               unsigned int y,
                                               OutlinerSvgStyle style,
                                               bool lookForTailMatch,
                                               unsigned int index);
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
 };

#endif // SVG_HH

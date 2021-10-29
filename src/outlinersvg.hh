
#ifndef SVG_HH
#define SVG_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define OutlinerSvgMaxLineSegments  511
#define OutlinerSvgMaxLinePoints    (OutlinerSvgMaxLineSegments+1)

struct OutlinerSvgCoord {
  unsigned int x;
  unsigned int y;
};

struct OutlinerSvgLine {
  unsigned int refCount;
  bool dashed;
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

class SvgCreator {
  
 public:
  
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
  ~SvgCreator();
  void line(outlinerreal fromX,
            outlinerreal fromY,
            outlinerreal toX,
            outlinerreal toY,
            bool dashed = 0);
  void pixel(outlinerreal x,
             outlinerreal y);
  void text(outlinerreal x,
            outlinerreal y,
            const char* string);
  outlinerreal getPixelXSize(void);
  outlinerreal getPixelYSize(void);
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
               bool dashed);
  void emitLine(const struct OutlinerSvgLine& line);

  void lineTableInit(void);
  void lineTableDeinit(void);
  struct OutlinerSvgLine* matchingLine(unsigned int x1,
                                       unsigned int y1,
                                       unsigned int x2,
                                       unsigned int y2,
                                       bool dashed,
                                       unsigned int& matchIndex,
                                       bool& matchesStart,
                                       bool& reverseOriginal);
  struct OutlinerSvgLine* matchingLineJoin(struct OutlinerSvgLine* target,
                                           unsigned int x,
                                           unsigned int y,
                                           bool fromStart);
  struct OutlinerSvgLine* matchingLineAux(unsigned int x,
                                          unsigned int y,
                                          bool dashed,
                                          bool lookForTailMatch,
                                          unsigned int index);
  struct OutlinerSvgLine* matchingLineAuxAvoid(struct OutlinerSvgLine* avoid,
                                               unsigned int x,
                                               unsigned int y,
                                               bool dashed,
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

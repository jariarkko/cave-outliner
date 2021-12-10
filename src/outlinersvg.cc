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

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdebug.hh"
#include "outlinersvg.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class methods //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

SvgCreator::SvgCreator(const char* fileNameIn,
                       const unsigned int xSizeIn,
                       const unsigned int ySizeIn,
                       const outlinerreal xStartIn,
                       const outlinerreal yStartIn,
                       const outlinerreal xFactorIn,
                       const outlinerreal yFactorIn,
                       const SvgOptions& optionsIn) :
  fileName(fileNameIn),
  xSize(xSizeIn),
  ySize(ySizeIn),
  xSizeMultiplied(xSize*optionsIn.multiplier),
  ySizeMultiplied(ySize*optionsIn.multiplier),
  xStart(xStartIn),
  yStart(yStartIn),
  xFactor(xFactorIn),
  yFactor(yFactorIn),
  options(optionsIn) {

  assert(xSize > 0);
  assert(ySize > 0);
  assert(options.linewidth > 0.0);
  file.open(fileName);
  pixels = 0;
  originalLines = 0;
  finalLines = 0;
  joins = 0;
  strings = 0;
  characters = 0;
  
  debugf("coordinate normalization starts (%.2f,%.2f) factors (%f,%f) linewidth %f",
         xStart, yStart, xFactor, yFactor, options.linewidth);

  lineTableInit();
  preamble();
}

SvgCreator::~SvgCreator() {
  lineTableOutput();
  postamble();
  unsigned long bytes = file.tellp();
  file.close();
  infof("  image statistics");
  infof("    file %s size %.1f KB", fileName, (bytes / 1000.0));
  infof("    image has %u pixels", pixels);
  infof("    image has %u lines (from originally %u line components)", finalLines, originalLines);
  infof("    image has %u strings (%u characters)", strings, characters);
  lineTableInfos();
  lineTableDeinit();
  lineTable = 0;
}

outlinerreal
SvgCreator::getPixelXSize(void) {
  return((1.0/xFactor)/options.multiplier);
}

outlinerreal
SvgCreator::getPixelYSize(void) {
  return((1.0/yFactor)/options.multiplier);
}

void
SvgCreator::line(outlinerreal fromX,
                 outlinerreal fromY,
                 outlinerreal toX,
                 outlinerreal toY,
                 OutlinerSvgStyle style) {

  deepdeepdebugf("line %.2f,%.2f to %.2f,%.2f", fromX, fromY, toX, toY);
  assert((style & outlinersvgstyle_illegal) == 0);
  if (originalLines == 0 && pixels == 0) {
    infof("  image size %u x %u", xSize, ySize);
  }
  originalLines++;
  
  unsigned int fromXInt;
  unsigned int fromYInt;
  coordinateNormalization(fromX,fromY,fromXInt,fromYInt);
  
  unsigned int toXInt;
  unsigned int toYInt;
  coordinateNormalization(toX,toY,toXInt,toYInt);

  deepdebugf("SvgCreator::line (%u,%u) to (%u,%u)",
             fromXInt,fromYInt,toXInt,toYInt);
  addLine(fromXInt,fromYInt,toXInt,toYInt,style);
}

void
SvgCreator::addLine(unsigned int x1,
                    unsigned int y1,
                    unsigned int x2,
                    unsigned int y2,
                    OutlinerSvgStyle style) {
  deepdebugf("addLine");
  assert((style & outlinersvgstyle_illegal) == 0);
  if (options.mergedLines) {
    unsigned int matchIndex;
    bool isStart;
    bool reverseOriginal;
    struct OutlinerSvgLine* match =
      matchingLine(x1,y1,
                   x2,y2,
                   style,
                   matchIndex,
                   isStart,
                   reverseOriginal);
    if (match == 0) {
      deepdebugf("no line match for (%u,%u)-(%u,%u)", x1, y1, x2, y2);
      struct OutlinerSvgLine* entry = new struct OutlinerSvgLine;
      if (entry == 0) {
        errf("Cannot allocate a line entry");
        return;
      }
      entry->refCount = 0;
      entry->printed = 0;
      entry->nPoints = 2;
      entry->style = style;
      entry->points[0].x = x1;
      entry->points[0].y = y1;
      entry->points[1].x = x2;
      entry->points[1].y = y2;
      lineTableEntryAdd(entry);
      assert(entry->refCount == 2);
    } else {
      assert(match->nPoints > 0);
      assert(match->nPoints < OutlinerSvgMaxLinePoints);
      assert(match->refCount == 2);
      assert(style == match->style);
      deepdebugf("line match for (%u,%u)-(%u,%u) in index %u start %u reverse %u",
                 x1, y1, x2, y2, matchIndex, isStart, reverseOriginal);
      lineTableEntryUnlink(match,matchIndex);
      assert(match->refCount == 1);
      if (reverseOriginal) {
        unsigned int tmp;
        tmp = x1; x1 = x2; x2 = tmp;
        tmp = y1; y1 = y2; y2 = tmp;
      }
      if (isStart) {
        for (unsigned int j = match->nPoints; j > 0; j--) {
          match->points[j] = match->points[j-1];
        }
        match->nPoints++;
        match->points[0].x = x1;
        match->points[0].y = y1;
        unsigned int newIndex = lineTableIndex(x1,y1);
        lineTableEntryLink(match,newIndex);
        // Check if the newly extended line can be joined with an existing one
        struct OutlinerSvgLine* tojoin = 0;
        if ((tojoin = matchingLineJoin(match,x1,y1,1)) != 0) {
          deepdebugf("going to merge start of two lines of %u and %u points",
                     match->nPoints, tojoin->nPoints);
          assert(match->nPoints + tojoin->nPoints <= OutlinerSvgMaxLinePoints);
          lineTableJoin(match,tojoin,1,newIndex);
        }
      } else {
        match->points[match->nPoints].x = x2;
        match->points[match->nPoints].y = y2;
        match->nPoints++;
        unsigned int newIndex = lineTableIndex(x2,y2);
        lineTableEntryLink(match,newIndex);
        // Check if the newly extended line can be joined with an existing one
        struct OutlinerSvgLine* tojoin = 0;
        if ((tojoin = matchingLineJoin(match,x2,y2,0)) != 0) {
          deepdebugf("going to merge two lines of %u and %u points",
                     match->nPoints, tojoin->nPoints);
          assert(match->nPoints + tojoin->nPoints <= OutlinerSvgMaxLinePoints);
          lineTableJoin(match,tojoin,0,newIndex);
        }
      }
      assert(match->refCount == 2);
    }
  } else {
    deepdebugf("emitting line directly");
    struct OutlinerSvgLine line;
    line.nPoints = 2;
    line.points[0].x = x1;
    line.points[0].y = y1;
    line.points[1].x = x2;
    line.points[1].y = y2;
    line.style = style;
    emitLine(line);
  }
}

void
SvgCreator::emitLine(const struct OutlinerSvgLine& line) {

  // Debugs and sanity checks
  deepdebugf("SvgCreator::emitLine");
  assert(line.nPoints >= 2);
  
  // Handle entrance lines
  if ((line.style & outlinersvgstyle_stubs) != 0) {
    emitStubsLine(line);
    return;
  }

  // Handle smoothing
  if (options.smooth) {
    struct OutlinerSvgLine smoothedLine = line;
    smoothLine(smoothedLine);
    emitLineAux(smoothedLine);
    return;
  }
  
  // Do the actual drawing
  emitLineAux(line);
}

void
SvgCreator::emitLineAux(const struct OutlinerSvgLine& line) {

  // Basic line drawing
  if (line.nPoints == 2) {
    file << "<line x1=\"" << line.points[0].x << "\" y1=\"" << line.points[0].y << "\"";
    file << " x2=\"" << line.points[1].x << "\" y2=\"" << line.points[1].y << "\"";
  } else {
    file << "<polyline points=\"";
    for (unsigned int i = 0; i < line.nPoints; i++) {
      unsigned int x = line.points[i].x;
      unsigned int y = line.points[i].y;
      file << x << "," << y <<  " ";
    }
    file << "\"";
  }

  // Handle color
  const char* color = colorBasedOnStyle(line.style);
  if ((line.style & outlinersvgstyle_stubs) != 0) {
    file << " stroke-dasharray=\"1 1\" ";
  }
  
  // Handle smoothing
  file << " fill=\"none\" stroke=\"" << color << "\" ";
  
  // Handle styles
  if ((line.style & outlinersvgstyle_illegal) != 0) {
    errf("Invalid line style %x", line.style);
    return;
  }
  if ((line.style & outlinersvgstyle_dashed) != 0) {
    file << " stroke-dasharray=\"1 1\" ";
  }

  // Postamble
  file << "stroke-width=\"" << options.linewidth << "\" />\n";

  // Styles that add something beyond the line itself
  assert((line.style & outlinersvgstyle_stubs) == 0);
  if ((line.style & outlinersvgstyle_ends) != 0) {

    unsigned int notch = 1 + options.multiplier/2;
    unsigned int xDiff;
    unsigned int yDiff;
    if (line.points[0].x == line.points[line.nPoints-1].x) {
      xDiff = notch;
      yDiff = 0;
    } else if (line.points[0].y == line.points[line.nPoints-1].y) {
      xDiff = 0;
      yDiff = notch;
    } else {
      errf("Line endpoints style not supported for non-vertical/horizontal lines");
      return;
    }
    
    struct OutlinerSvgLine end1line;
    end1line.refCount = 0;
    end1line.style = line.style - outlinersvgstyle_ends;
    end1line.printed = 0;
    end1line.nPoints = 2;
    end1line.points[0].x = line.points[0].x - xDiff;
    end1line.points[0].y = line.points[0].y - yDiff;
    end1line.points[1].x = line.points[0].x + xDiff;
    end1line.points[1].y = line.points[0].y + yDiff;
    emitLine(end1line);
    
    struct OutlinerSvgLine end2line;
    end2line.refCount = 0;
    end2line.style = line.style - outlinersvgstyle_ends;
    end2line.printed = 0;
    end2line.nPoints = 2;
    end2line.points[0].x = line.points[line.nPoints-1].x - xDiff;
    end2line.points[0].y = line.points[line.nPoints-1].y - yDiff;
    end2line.points[1].x = line.points[line.nPoints-1].x + xDiff;
    end2line.points[1].y = line.points[line.nPoints-1].y + yDiff;
    emitLine(end2line);
    
  }

  // Update statistics
  finalLines++;
}

void
SvgCreator::smoothPoint(const unsigned int pointIndex,
                        struct OutlinerSvgLine& line) const {
  assert(pointIndex > 0);
  assert(pointIndex + 1 < line.nPoints);
  unsigned int prevPoint = pointIndex - 1;
  unsigned int nextPoint = pointIndex + 1;
  line.points[pointIndex].x = outlineravg3(line.points[prevPoint].x,
                                           line.points[pointIndex].x,
                                           line.points[nextPoint].x);
  line.points[pointIndex].y = outlineravg3(line.points[prevPoint].y,
                                           line.points[pointIndex].y,
                                           line.points[nextPoint].y);
}

void
SvgCreator::smoothLine(struct OutlinerSvgLine& line) const {
  for (unsigned int pointIndex = 1; pointIndex + 1 < line.nPoints; pointIndex += 2) smoothPoint(pointIndex,line);
  for (unsigned int pointIndex = 2; pointIndex + 1 < line.nPoints; pointIndex += 2) smoothPoint(pointIndex,line);
}

void
SvgCreator::emitStubsLine(const struct OutlinerSvgLine& line) {

  //
  // Debugs
  //

  infof("emitStubsLine with %u points", line.nPoints);
  assert(line.nPoints >= 2);

  //
  // First apply some smoothing on points 2...n-1
  //

  OutlinerSvgLine newLine = line;
  smoothLine(newLine);
  smoothLine(newLine);
  smoothLine(newLine);
  smoothLine(newLine);
  smoothLine(newLine);
  
  //
  // Break up the longer line into segments, also skipping every
  // second entry to smooth it out. For every resulting two segments,
  // one will be a dash line and the stub, one will be the empty space
  // between dashes.
  //

  for (unsigned int segmentStart = 0; segmentStart + 2 < newLine.nPoints; segmentStart += 4) {

    // Emit one segment
    unsigned int segmentOther = segmentStart + 2;
    struct OutlinerSvgLine segment;
    segment.refCount = 1;
    segment.style = (newLine.style & ~(outlinersvgstyle_stubs+outlinersvgstyle_stubs_dirl+outlinersvgstyle_stubs_dird));
    segment.printed = 0;
    segment.nPoints = 2;
    segment.points[0] = newLine.points[segmentStart];
    segment.points[1] = newLine.points[segmentOther];
    emitLine(segment);

    // Emit the stub line that is perpendicular to the segment
    struct OutlinerSvgLine stub;
    unsigned int stubLength = 1 + options.multiplier/1;
    stub.refCount = 1;
    stub.style = (newLine.style & ~(outlinersvgstyle_stubs+outlinersvgstyle_stubs_dirl+outlinersvgstyle_stubs_dird));
    //stub.style |= outlinersvgstyle_red;
    stub.printed = 0;
    stub.nPoints = 2;
    unsigned int stubstartx = (newLine.points[segmentStart].x + newLine.points[segmentOther].x)/2;
    unsigned int stubstarty = (newLine.points[segmentStart].y + newLine.points[segmentOther].y)/2;
    unsigned int xdiff = outlinerunsignedabsdiff(newLine.points[segmentOther].x,newLine.points[segmentStart].x);
    unsigned int ydiff = outlinerunsignedabsdiff(newLine.points[segmentOther].y,newLine.points[segmentStart].y);
    unsigned int diffsum = xdiff + ydiff;
    outlinerreal xportion = (1.0*xdiff) / (1.0*diffsum);
    outlinerreal yportion = (1.0*ydiff) / (1.0*diffsum);
    if (xportion < 0.25) stubLength += stubLength/4;
    else if (xportion < 0.50) stubLength += stubLength/2;
    else if (xportion < 0.75) stubLength += stubLength/4;
    if ((newLine.style & outlinersvgstyle_stubs_dirl)) xportion = -xportion;
    if ((newLine.style & outlinersvgstyle_stubs_dird)) yportion = -yportion;
    int stubx = stubLength * yportion;
    int stuby = stubLength * -xportion;
    infof("stub diffs %u %u => portions %.2f %.2f => stub %d %d",
          xdiff, ydiff, xportion, yportion, stubx, stuby);
    stub.points[0].x = stubstartx;
    stub.points[0].y = stubstarty;
    stub.points[1].x = stubstartx + stubx;
    stub.points[1].y = stubstarty + stuby;
    emitLine(stub);
  }
}

void
SvgCreator::pixel(outlinerreal x,
                  outlinerreal y,
                  OutlinerSvgStyle style) {
  deepdeepdebugf("pixel %.2f,%.2f", x, y);
  unsigned int xInt;
  unsigned int yInt;
  coordinateNormalization(x,y,xInt,yInt);
  const char* color = colorBasedOnStyle(style);
  deepdebugf("     pixel %u %s", style, color);
  deepdebugf("SvgCreator::pixel %.2f,%.2f to %u,%u", x, y, xInt, yInt);
  file << "<rect x=\"" << xInt << "\" y=\"" << yInt << "\"";
  file << " width=\"" << options.multiplier << "\" height=\"" <<  options.multiplier << "\"";
  file << " fill=\"" << color << "\"";
  file << " stroke-width=\"0\" stroke=\"" << color << "\" />\n";
  pixels++;
}

void
SvgCreator::triangle(OutlinerTriangle2D triangle,
                     OutlinerSvgStyle style) {

  // Debugs
  assert((style & outlinersvgstyle_illegal) == 0);
  deepdeepdebugf("triangle");
  
  // Convert coordinates
  unsigned int xIntA, yIntA;
  unsigned int xIntB, yIntB;
  unsigned int xIntC, yIntC;
  coordinateNormalization(triangle.a.x,triangle.a.y,xIntA,yIntA);
  coordinateNormalization(triangle.b.x,triangle.b.y,xIntB,yIntB);
  coordinateNormalization(triangle.c.x,triangle.c.y,xIntC,yIntC);

  // Construct a 4-segment line that draws the triangle
  struct OutlinerSvgLine line;
  line.nPoints = 4;
  line.points[0].x = xIntA;
  line.points[0].y = yIntA;
  line.points[1].x = xIntB;
  line.points[1].y = yIntB;
  line.points[2].x = xIntC;
  line.points[2].y = yIntC;
  line.points[3].x = xIntA;
  line.points[3].y = yIntA;
  line.style = style;
  
  // Emit the SVG code to draw that line
  emitLine(line);
}

void
SvgCreator::text(outlinerreal x,
                 outlinerreal y,
                 const char* string,
                 unsigned int fontSize,
                 unsigned int rotate) {
  // Debugs
  assert(string != 0);
  deepdeepdebugf("SvgCreator::text %.2f,%.2f: %s", x, y, string);

  // Coordinate conversions
  unsigned int xInt;
  unsigned int yInt;
  coordinateNormalization(x,y,xInt,yInt);
  unsigned int translateX = rotate != 0 ? xInt : 0;
  unsigned int translateY = rotate != 0 ? yInt : 0;
  if (rotate) xInt = yInt = 0;

  // SVG output
  file << "<text x=\"" << xInt << "\" y=\"" << yInt << "\"";
  file << " fill=\"black\"";
  if (fontSize != outlinerdefaultfont) {
    file << " font-size=\"" << fontSize << "px\"";
  }
  if (rotate != 0) {
    file << " transform=\"translate(" << translateX << "," << translateY << ") rotate(" << rotate << ")\"";
   }
  file << ">";
  file << string;
  file << "</text>\n";

  // Statistics updates
  strings++;
  characters += strlen(string);
  infof("text to (%.2f,%.2f) which is (%u,%u)", x, y, xInt, yInt);
}

void
SvgCreator::coordinateNormalization(outlinerreal x,
                                    outlinerreal y,
                                    unsigned int& xInt,
                                    unsigned int& yInt) {
  outlinerreal xNormalized = (x - xStart) * xFactor;
  outlinerreal yNormalized = (y - yStart) * yFactor;
  if (xNormalized > (outlinerreal)xSize) xInt = xSizeMultiplied;
  else xInt = (xNormalized*options.multiplier);
  if (yNormalized > (outlinerreal)ySize) yInt = options.ySwap ? 0 : ySizeMultiplied;
  else yInt = options.ySwap ? (ySizeMultiplied - (yNormalized*options.multiplier)) : yNormalized*options.multiplier;
  deepdebugf("coordinate normalization (%.2f,%.2f) to (%u,%u) with yNormalized %.2f ySize %u yStart %.2f and yFactor %.2f",
             x, y,
             xInt, yInt,
             yNormalized, ySize, yStart, yFactor);
  if (xInt > xSizeMultiplied || yInt > ySizeMultiplied) {
    infof("xInt %u (/%u) from x %.2f start %.2f factor %.2f multiplier %.2f",
          xInt, xSizeMultiplied, x, xStart, xFactor, options.multiplier);
    infof("yInt %u (/%u) from y %.2f start %.2f factor %.2f multiplier %.2f",
          yInt, ySizeMultiplied, y, yStart, yFactor, options.multiplier);
  }
  assert(xInt <= xSizeMultiplied);
  assert(yInt <= ySizeMultiplied);
}

bool
SvgCreator::ok() {
  deepdeepdebugf("SvgCreator::ok %lx", this);
  deepdeepdebugf("SvgCreator::ok %u %u", xSize, ySize);
  return(xSize > 0 && ySize > 0 && file.good());
}

static const char* greyscale[256] = {
  "rgb(0,0,0)",
  "rgb(1,1,1)",
  "rgb(2,2,2)",
  "rgb(3,3,3)",
  "rgb(4,4,4)",
  "rgb(5,5,5)",
  "rgb(6,6,6)",
  "rgb(7,7,7)",
  "rgb(8,8,8)",
  "rgb(9,9,9)",
  "rgb(10,10,10)",
  "rgb(11,11,11)",
  "rgb(12,12,12)",
  "rgb(13,13,13)",
  "rgb(14,14,14)",
  "rgb(15,15,15)",
  "rgb(16,16,16)",
  "rgb(17,17,17)",
  "rgb(18,18,18)",
  "rgb(19,19,19)",
  "rgb(20,20,20)",
  "rgb(21,21,21)",
  "rgb(22,22,22)",
  "rgb(23,23,23)",
  "rgb(24,24,24)",
  "rgb(25,25,25)",
  "rgb(26,26,26)",
  "rgb(27,27,27)",
  "rgb(28,28,28)",
  "rgb(29,29,29)",
  "rgb(30,30,30)",
  "rgb(31,31,31)",
  "rgb(32,32,32)",
  "rgb(33,33,33)",
  "rgb(34,34,34)",
  "rgb(35,35,35)",
  "rgb(36,36,36)",
  "rgb(37,37,37)",
  "rgb(38,38,38)",
  "rgb(39,39,39)",
  "rgb(40,40,40)",
  "rgb(41,41,41)",
  "rgb(42,42,42)",
  "rgb(43,43,43)",
  "rgb(44,44,44)",
  "rgb(45,45,45)",
  "rgb(46,46,46)",
  "rgb(47,47,47)",
  "rgb(48,48,48)",
  "rgb(49,49,49)",
  "rgb(50,50,50)",
  "rgb(51,51,51)",
  "rgb(52,52,52)",
  "rgb(53,53,53)",
  "rgb(54,54,54)",
  "rgb(55,55,55)",
  "rgb(56,56,56)",
  "rgb(57,57,57)",
  "rgb(58,58,58)",
  "rgb(59,59,59)",
  "rgb(60,60,60)",
  "rgb(61,61,61)",
  "rgb(62,62,62)",
  "rgb(63,63,63)",
  "rgb(64,64,64)",
  "rgb(65,65,65)",
  "rgb(66,66,66)",
  "rgb(67,67,67)",
  "rgb(68,68,68)",
  "rgb(69,69,69)",
  "rgb(70,70,70)",
  "rgb(71,71,71)",
  "rgb(72,72,72)",
  "rgb(73,73,73)",
  "rgb(74,74,74)",
  "rgb(75,75,75)",
  "rgb(76,76,76)",
  "rgb(77,77,77)",
  "rgb(78,78,78)",
  "rgb(79,79,79)",
  "rgb(80,80,80)",
  "rgb(81,81,81)",
  "rgb(82,82,82)",
  "rgb(83,83,83)",
  "rgb(84,84,84)",
  "rgb(85,85,85)",
  "rgb(86,86,86)",
  "rgb(87,87,87)",
  "rgb(88,88,88)",
  "rgb(89,89,89)",
  "rgb(90,90,90)",
  "rgb(91,91,91)",
  "rgb(92,92,92)",
  "rgb(93,93,93)",
  "rgb(94,94,94)",
  "rgb(95,95,95)",
  "rgb(96,96,96)",
  "rgb(97,97,97)",
  "rgb(98,98,98)",
  "rgb(99,99,99)",
  "rgb(100,100,100)",
  "rgb(101,101,101)",
  "rgb(102,102,102)",
  "rgb(103,103,103)",
  "rgb(104,104,104)",
  "rgb(105,105,105)",
  "rgb(106,106,106)",
  "rgb(107,107,107)",
  "rgb(108,108,108)",
  "rgb(109,109,109)",
  "rgb(110,110,110)",
  "rgb(111,111,111)",
  "rgb(112,112,112)",
  "rgb(113,113,113)",
  "rgb(114,114,114)",
  "rgb(115,115,115)",
  "rgb(116,116,116)",
  "rgb(117,117,117)",
  "rgb(118,118,118)",
  "rgb(119,119,119)",
  "rgb(120,120,120)",
  "rgb(121,121,121)",
  "rgb(122,122,122)",
  "rgb(123,123,123)",
  "rgb(124,124,124)",
  "rgb(125,125,125)",
  "rgb(126,126,126)",
  "rgb(127,127,127)",
  "rgb(128,128,128)",
  "rgb(129,129,129)",
  "rgb(130,130,130)",
  "rgb(131,131,131)",
  "rgb(132,132,132)",
  "rgb(133,133,133)",
  "rgb(134,134,134)",
  "rgb(135,135,135)",
  "rgb(136,136,136)",
  "rgb(137,137,137)",
  "rgb(138,138,138)",
  "rgb(139,139,139)",
  "rgb(140,140,140)",
  "rgb(141,141,141)",
  "rgb(142,142,142)",
  "rgb(143,143,143)",
  "rgb(144,144,144)",
  "rgb(145,145,145)",
  "rgb(146,146,146)",
  "rgb(147,147,147)",
  "rgb(148,148,148)",
  "rgb(149,149,149)",
  "rgb(150,150,150)",
  "rgb(151,151,151)",
  "rgb(152,152,152)",
  "rgb(153,153,153)",
  "rgb(154,154,154)",
  "rgb(155,155,155)",
  "rgb(156,156,156)",
  "rgb(157,157,157)",
  "rgb(158,158,158)",
  "rgb(159,159,159)",
  "rgb(160,160,160)",
  "rgb(161,161,161)",
  "rgb(162,162,162)",
  "rgb(163,163,163)",
  "rgb(164,164,164)",
  "rgb(165,165,165)",
  "rgb(166,166,166)",
  "rgb(167,167,167)",
  "rgb(168,168,168)",
  "rgb(169,169,169)",
  "rgb(170,170,170)",
  "rgb(171,171,171)",
  "rgb(172,172,172)",
  "rgb(173,173,173)",
  "rgb(174,174,174)",
  "rgb(175,175,175)",
  "rgb(176,176,176)",
  "rgb(177,177,177)",
  "rgb(178,178,178)",
  "rgb(179,179,179)",
  "rgb(180,180,180)",
  "rgb(181,181,181)",
  "rgb(182,182,182)",
  "rgb(183,183,183)",
  "rgb(184,184,184)",
  "rgb(185,185,185)",
  "rgb(186,186,186)",
  "rgb(187,187,187)",
  "rgb(188,188,188)",
  "rgb(189,189,189)",
  "rgb(190,190,190)",
  "rgb(191,191,191)",
  "rgb(192,192,192)",
  "rgb(193,193,193)",
  "rgb(194,194,194)",
  "rgb(195,195,195)",
  "rgb(196,196,196)",
  "rgb(197,197,197)",
  "rgb(198,198,198)",
  "rgb(199,199,199)",
  "rgb(200,200,200)",
  "rgb(201,201,201)",
  "rgb(202,202,202)",
  "rgb(203,203,203)",
  "rgb(204,204,204)",
  "rgb(205,205,205)",
  "rgb(206,206,206)",
  "rgb(207,207,207)",
  "rgb(208,208,208)",
  "rgb(209,209,209)",
  "rgb(210,210,210)",
  "rgb(211,211,211)",
  "rgb(212,212,212)",
  "rgb(213,213,213)",
  "rgb(214,214,214)",
  "rgb(215,215,215)",
  "rgb(216,216,216)",
  "rgb(217,217,217)",
  "rgb(218,218,218)",
  "rgb(219,219,219)",
  "rgb(220,220,220)",
  "rgb(221,221,221)",
  "rgb(222,222,222)",
  "rgb(223,223,223)",
  "rgb(224,224,224)",
  "rgb(225,225,225)",
  "rgb(226,226,226)",
  "rgb(227,227,227)",
  "rgb(228,228,228)",
  "rgb(229,229,229)",
  "rgb(230,230,230)",
  "rgb(231,231,231)",
  "rgb(232,232,232)",
  "rgb(233,233,233)",
  "rgb(234,234,234)",
  "rgb(235,235,235)",
  "rgb(236,236,236)",
  "rgb(237,237,237)",
  "rgb(238,238,238)",
  "rgb(239,239,239)",
  "rgb(240,240,240)",
  "rgb(241,241,241)",
  "rgb(242,242,242)",
  "rgb(243,243,243)",
  "rgb(244,244,244)",
  "rgb(245,245,245)",
  "rgb(246,246,246)",
  "rgb(247,247,247)",
  "rgb(248,248,248)",
  "rgb(249,249,249)",
  "rgb(250,250,250)",
  "rgb(251,251,251)",
  "rgb(252,252,252)",
  "rgb(253,253,253)",
  "rgb(254,254,254)",
  "rgb(255,255,255)"
};

const char*
SvgCreator::colorBasedOnStyle(OutlinerSvgStyle style) const {
  const char* color = "black";
  if ((style & outlinersvgstyle_grey) != 0) {
    color = greyscale[(style & 0x00FF)];
    debugf("      grey %s from %04x", color, style);
  } else if ((style & outlinersvgstyle_red) != 0) {
    color = "red";
  } else if ((style & outlinersvgstyle_green) != 0) {
    color = "green";
  } else if ((style & outlinersvgstyle_blue) != 0) {
    color = "blue";
  } else if ((style & outlinersvgstyle_yellow) != 0) {
    color = "yellow";
  }
  return(color);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Post and preambles /////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
SvgCreator::preamble() {

  // Debugs
  debugf("preamble");
  
  // Basics for all SVGs
  file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
  file << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n";
  file << "          \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
  file << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\"\n";
  file << "     width=\"" << (xSize+1)*options.multiplier << "\" height=\"" << (ySize+1)*options.multiplier << "\">\n";

  // Make the background white
  file << "<rect x=\"0\" y=\"0\" width=\"" << (xSize+1)*options.multiplier;
  file << "\" height=\"" << (ySize+1)*options.multiplier << "\"";
  file << " fill=\"white\" stroke=\"white\" stroke-width=\"0\"/>\n";
 
  // Done
  debugf("preamble done");
}

void
SvgCreator::postamble() {
  // Just close the full SVG XML
  file << "</svg>\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Line table management //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

struct OutlinerSvgLine*
SvgCreator::matchingLine(unsigned int x1,
                         unsigned int y1,
                         unsigned int x2,
                         unsigned int y2,
                         OutlinerSvgStyle style,
                         unsigned int& matchIndex,
                         bool& matchesStart,
                         bool& reverseOriginal) {

  deepdebugf("matchingLine");
  assert((style & outlinersvgstyle_basemask & outlinersvgstyle_illegal) == 0);
  
  unsigned int head = lineTableIndex(x1,y1);
  unsigned int tail = lineTableIndex(x2,y2);
  struct OutlinerSvgLine* result = 0;

  // See if we can find a line that ends in (x1,y1)
  if ((result = matchingLineAux(x1,y1,style,1,head)) != 0) {
    matchIndex = head;
    matchesStart = 0;
    reverseOriginal = 0;
    return(result);
  }

  // See if we can find a line that starts with (x2,y2)
  if ((result = matchingLineAux(x2,y2,style,0,tail)) != 0) {
    matchIndex = tail;
    matchesStart = 1;
    reverseOriginal = 0;
    return(result);
  }
  
  // See if we can find a line that ends in (x2,y2)
  if ((result = matchingLineAux(x2,y2,style,1,tail)) != 0) {
    matchIndex = tail;
    matchesStart = 0;
    reverseOriginal = 1;
    return(result);
  }

  // See if we can find a line that starts with (x1,y1)
  if ((result = matchingLineAux(x1,y1,style,0,head)) != 0) {
    matchIndex = head;
    matchesStart = 1;
    reverseOriginal = 1;
    return(result);
  }
  
  // Nothing found.
  matchesStart = 0;
  matchIndex = 0;
  reverseOriginal = 0;
  return(0);
}

struct OutlinerSvgLine*
SvgCreator::matchingLineAux(unsigned int x,
                            unsigned int y,
                            OutlinerSvgStyle style,
                            bool lookForTailMatch,
                            unsigned int index) {
  assert((style & outlinersvgstyle_basemask & outlinersvgstyle_illegal) == 0);
  return(matchingLineAuxAvoid(0,x,y,style,lookForTailMatch,
                              index));
}

struct OutlinerSvgLine*
SvgCreator::matchingLineAuxAvoid(struct OutlinerSvgLine* avoid,
                                 unsigned int x,
                                 unsigned int y,
                                 OutlinerSvgStyle style,
                                 bool lookForTailMatch,
                                 unsigned int index) {
  assert((style & outlinersvgstyle_basemask & outlinersvgstyle_illegal) == 0);
  struct OutlinerSvgLineList* list = lineTable[index];
  deepdeepdebugf("matchingLineAux loop");
  while (list != 0) {

    // Sanity checks
    assert(list != 0);
    assert(list->line != 0);
    
    // Pick up the entry
    struct OutlinerSvgLine* line = list->line;
    assert(line->nPoints >= 2);
    assert(line->nPoints <= OutlinerSvgMaxLinePoints);
    assert(line->refCount > 0);
    assert(line->refCount <= 2);
    unsigned int first = 0;
    unsigned int last = line->nPoints - 1;

    // Bail out if
    //
    //    1/ the line is already at max capacity or
    //    2/ we are trying to avoid the line object in this entry
    //    3/ style does not match
    //
    
    if (line != avoid && line->nPoints < OutlinerSvgMaxLinePoints && style == line->style) {
    
      // Is there a match here?
      if (!lookForTailMatch && line->points[first].x == x && line->points[first].y == y) return(line);
      if (lookForTailMatch && line->points[last].x == x && line->points[last].y == y) return(line);

    }
    
    // Move to the next
    assert(list != 0);
    assert(list != list->next);
    assert(list->next == 0 || list != list->next->next);
    assert(list->next == 0 || list->next->next == 0 || list != list->next->next->next);
    list = list->next;
    //deepdeepdebugf("moved to next %lx", list);
    
  }

  // Not found
  return(0);
}

struct OutlinerSvgLine*
SvgCreator::matchingLineJoin(struct OutlinerSvgLine* target,
                             unsigned int x,
                             unsigned int y,
                             bool fromStart) {
  deepdebugf("matchingLineJoin");
  assert(target != 0);
  assert(fromStart == 0 || fromStart == 1);
  unsigned int searchIndex = lineTableIndex(x,y);
  assert(searchIndex < lineTableSize);
  struct OutlinerSvgLine* result = 0;
  if (fromStart) {
    // See if we can find a line that ends with (x,y)
    if ((result = matchingLineAuxAvoid(target,x,y,target->style,1,searchIndex)) != 0) {
      return(result);
    }
  } else {
    // See if we can find a line that starts with (x,y)
    if ((result = matchingLineAuxAvoid(target,x,y,target->style,0,searchIndex)) != 0) {
      return(result);
    }
  }

  // Not found
  return(0);
}

void
SvgCreator::lineTableJoin(struct OutlinerSvgLine* entry,
                          struct OutlinerSvgLine* join,
                          bool fromStart,
                          unsigned int entryIndex) {

  // Sanity checks
  deepdebugf("lineTableJoin");
  assert(entry != 0);
  assert(join != 0);
  assert(entry != join);
  assert(fromStart == 0 || fromStart == 1);
  assert(entryIndex < lineTableSize);
  assert(entry->nPoints + join->nPoints <= OutlinerSvgMaxLinePoints);
  assert(entry->style == join->style);
  assert(entry->refCount == 2);
  deepdebugf("join point 1 entry ref count %u", entry->refCount);
  assert(join->refCount == 2);
  
  // Add the entries
  if (fromStart) {
    deepdebugf("join entry adds from start");
    for (unsigned int j = entry->nPoints; j > 0; j--) {
      entry->points[j-1+join->nPoints] = entry->points[j-1];
    }
    for (unsigned int k = 0; k < join->nPoints; k++) {
      entry->points[k] = join->points[k];
    }
  } else {
    deepdebugf("join entry adds to end");
    for (unsigned int i = 0; i < join->nPoints; i++) {
      entry->points[entry->nPoints + i] = join->points[i];
    }
  }
  entry->nPoints += join->nPoints;
  deepdebugf("join point 2 entry ref count %u", entry->refCount);
  
  // Unlink the join that has now been copied to the entry
  deepdebugf("join unlink join twice");
  deepdebugf("join point 3 entry ref count %u", entry->refCount);
  lineTableEntryUnlink(join,lineTableIndex(join->points[0].x,join->points[0].y));
  deepdebugf("join point 4 entry ref count %u", entry->refCount);
  lineTableEntryUnlink(join,lineTableIndex(join->points[join->nPoints-1].x,join->points[join->nPoints-1].y));
  deepdebugf("join point 5 entry ref count %u", entry->refCount);
  assert(join->refCount == 0);
  delete join;
  deepdebugf("join point 6 entry ref count %u", entry->refCount);
  
  // Unlink the old index in the entry and link to the new one
  deepdebugf("join point 7 entry ref count %u", entry->refCount);
  deepdebugf("join unlink entry");
  assert(entry->refCount == 2);
  lineTableEntryUnlink(entry,entryIndex);
  assert(entry->refCount == 1);
  unsigned int newIndex = (fromStart ?
                           lineTableIndex(entry->points[0].x,entry->points[0].y) :
                           lineTableIndex(entry->points[entry->nPoints-1].x,entry->points[entry->nPoints-1].y));
  deepdebugf("join link entry");
  lineTableEntryLink(entry,newIndex);
  deepdebugf("join point 8 entry ref count %u", entry->refCount);
  assert(entry->refCount == 2);
  deepdebugf("join point 9 entry ref count %u", entry->refCount);

  // Statistics
  joins++;
}

void
SvgCreator::lineTableEntryAdd(struct OutlinerSvgLine* entry) {
  deepdebugf("lineTableEntryAdd");
  assert(entry != 0);
  assert(entry->nPoints >= 2);
  assert(entry->nPoints <= OutlinerSvgMaxLinePoints);
  assert(entry->refCount == 0);
  assert(entry->refCount <= 2);
  unsigned int head;
  unsigned int tail;
  lineTableIndexes(*entry,head,tail);
  lineTableEntryLink(entry,head);
  lineTableEntryLink(entry,tail);
  assert(entry->refCount == 2);
}

void
SvgCreator::lineTableEntryLink(struct OutlinerSvgLine* entry,
                               unsigned int index) {
  deepdebugf("lineTableEntryLink");
  assert(index < lineTableSize);
  struct OutlinerSvgLineList* listItem = new struct OutlinerSvgLineList;
  if (listItem == 0) {
    errf("Cannot allocate a list item");
    return;
  }
  listItem->line = entry;
  listItem->next = lineTable[index];
  lineTable[index] = listItem;
  entry->refCount++;
  assert(entry->refCount <= 2);
}

void
SvgCreator::lineTableEntryUnlink(struct OutlinerSvgLine* entry,
                                 unsigned int index) {
  deepdebugf("lineTableEntryUnlink");
  assert(entry != 0);
  assert(entry->refCount > 0);
  assert(entry->refCount <= 2);
  assert(index < lineTableSize);
  struct OutlinerSvgLineList** list = &lineTable[index];
  deepdeepdebugf("unlink loop");
  while ((*list)->line != entry) {
    list = &(*list)->next;
    assert(*list != 0);
  }
  struct OutlinerSvgLineList* item = *list;
  assert(item->line == entry);
  *list = item->next;
  entry->refCount--;
  delete item;
}

void
SvgCreator::lineTableIndexes(const struct OutlinerSvgLine& line,
                             unsigned int& head,
                             unsigned int& tail) {
  deepdebugf("lineTableIndexes");
  assert(line.nPoints >= 2);
  assert(line.nPoints <= OutlinerSvgMaxLinePoints);
  assert(line.refCount <= 2);
  unsigned int x1 = line.points[0].x;
  unsigned int y1 = line.points[0].y;
  unsigned int xe = line.points[line.nPoints-1].x;
  unsigned int ye = line.points[line.nPoints-1].y;
  assert(x1 <= xSizeMultiplied);
  assert(y1 <= ySizeMultiplied);
  assert(xe <= xSizeMultiplied);
  assert(ye <= ySizeMultiplied);
  head = lineTableIndex(x1,y1);
  tail = lineTableIndex(xe,ye);
}

unsigned int
SvgCreator::lineTableIndex(unsigned int x,
                           unsigned int y) {
  deepdeepdebugf("lineTableIndex(%u/%u,%u/%u)", x, xSizeMultiplied, y, ySizeMultiplied);
  assert(x <= xSizeMultiplied);
  assert(y <= ySizeMultiplied);
  unsigned int result = x + y;
  assert(result < lineTableSize);
  return(result);
}

void
SvgCreator::lineTableEntryDelete(struct OutlinerSvgLineList*& entry) {
  deepdebugf("lineTableEntryDelete");
  assert(entry != 0);
  assert(entry->line != 0);
  assert(entry->line->nPoints >= 2);
  assert(entry->line->nPoints <= OutlinerSvgMaxLinePoints);
  assert(entry->line->refCount > 0);
  struct OutlinerSvgLineList* oldPointer = entry;
  entry = entry->next;
  oldPointer->line->refCount--;
  if (oldPointer->line->refCount == 0) {
    delete oldPointer->line;
  }
  oldPointer->line = 0;
  delete oldPointer;
}

void
SvgCreator::lineTableInit(void) {
  deepdebugf("line table init");
  lineTableSize = (xSizeMultiplied+1) + (ySizeMultiplied+1);
  lineTable = new struct OutlinerSvgLineList* [lineTableSize];
  if  (lineTable == 0) {
    errf("Cannot allocate line table of size %u", lineTableSize);
    return;
  }
  for (unsigned int i = 0; i < lineTableSize; i++) {
    lineTable[i] = 0;
  }
  deepdebugf("line table init done");
}

void
SvgCreator::lineTableDeinit(void) {
  deepdebugf("line table deinit");
  for (unsigned int i = 0; i < lineTableSize; i++) {
    assert(lineTable != 0);
    deepdeepdebugf("deinit loop");
    while (lineTable[i] != 0) {
      lineTableEntryDelete(lineTable[i]);
    }
  }
  deepdebugf("line table deinit done");
}

void
SvgCreator::lineTableInfos(void) {
  deepdebugf("lineTableInfos");
  unsigned int lineLines = 0;
  unsigned int lineSumPoints = 0;
  unsigned int lineLongest = 0;
  unsigned int lineTableEntries = 0;
  unsigned int lineTableMaxListLength = 0;
  unsigned int lineTableMaxListLengthIndex = 0;
  assert(lineTable != 0);
  for (unsigned int i = 0; i < lineTableSize; i++) {
    unsigned int length = 0;
    struct OutlinerSvgLineList* ptr = lineTable[i];
    deepdeepdebugf("infos loop");
    while (ptr != 0) {
      lineTableEntries++;
      lineLines++;
      lineSumPoints += ptr->line->nPoints;
      if (ptr->line->nPoints > lineLongest) lineLongest = ptr->line->nPoints;
      length++;
      ptr = ptr->next;
    }
    if (length > lineTableMaxListLength) {
      lineTableMaxListLength = length;
      lineTableMaxListLengthIndex = i;
    }
  }
  infof("    longest line has %u points (on average %.1f points)",
        lineLongest,
        (lineSumPoints / 2.0)  / (lineLines / 2.0));
  infof("    line table has %u entries", lineTableEntries);
  infof("    line table entries longest list has %u entries (%.2f%% of all, at index %u)",
        lineTableMaxListLength,
        (lineTableMaxListLength * 100.0) / (lineTableEntries * 1.0),
        lineTableMaxListLengthIndex);
  infof("    long line joins: %u", joins);
}

void
SvgCreator::lineTableOutput(void) {
  deepdebugf("line table output");
  assert(lineTable != 0);
  for (unsigned int i = 0; i < lineTableSize; i++) {
    struct OutlinerSvgLineList* ptr = lineTable[i];
    deepdeepdebugf("output loop");
    while (ptr != 0) {
      assert(ptr->line != 0);
      if (!ptr->line->printed) {
        emitLine(*(ptr->line));
        ptr->line->printed = 1;
      }
      ptr = ptr->next;
    }
  }
  deepdebugf("line table output done");
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Unit tests /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
SvgCreator::test(void) {
  SvgOptions opt(2,
                 0,1,1,0);
  SvgCreator svg("/tmp/foo.svg",
                 10,10,
                 0,0,
                 1,1,
                 opt);
  svg.smoothingTest();
}

void
SvgCreator::smoothingTest(void) {
  
  // Smoothing that should not affect anything
  struct OutlinerSvgLine line1;
  line1.refCount = 1;
  line1.style = 0;
  line1.printed = 0;
  line1.nPoints = 3;
  line1.points[0].x = 0;
  line1.points[0].y = 0;
  line1.points[1].x = 1;
  line1.points[1].y = 1;
  line1.points[2].x = 2;
  line1.points[2].y = 2;
  smoothLine(line1);
  infof("smoothing 1st result %u,%u", line1.points[1].x, line1.points[1].y);
  assert(line1.points[1].x == 1);
  assert(line1.points[1].y == 1);
  
  // Smoothing that should have an effect
  struct OutlinerSvgLine line2;
  line2.refCount = 1;
  line2.style = 0;
  line2.printed = 0;
  line2.nPoints = 3;
  line2.points[0].x = 0;
  line2.points[0].y = 0;
  line2.points[1].x = 0;
  line2.points[1].y = 2;
  line2.points[2].x = 2;
  line2.points[2].y = 2;
  smoothLine(line2);
  infof("smoothing 2nd result %u,%u", line2.points[1].x, line2.points[1].y);
  assert(line2.points[1].x == 2/3);
  assert(line2.points[1].y == 4/3);
}

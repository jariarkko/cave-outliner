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

SvgCreator::SvgCreator(const char* fileName,
                       unsigned int xSize,
                       unsigned int ySize,
                       unsigned int multiplier,
                       outlinerreal xStart,
                       outlinerreal yStart,
                       outlinerreal xFactor,
                       outlinerreal yFactor,
                       bool smooth,
                       bool mergedLines,
                       outlinerreal linewidth) {
  assert(xSize > 0);
  assert(ySize > 0);
  assert(linewidth > 0.0);
  this->file.open(fileName);
  this->xSize = xSize;
  this->ySize = ySize;
  this->multiplier = multiplier;
  this->xSizeMultiplied = xSize*multiplier;
  this->ySizeMultiplied = ySize*multiplier;
  this->xStart = xStart;
  this->yStart = yStart;
  this->xFactor = xFactor;
  this->yFactor = yFactor;
  this->smooth = smooth;
  this->mergedLines = mergedLines;
  this->linewidth = linewidth;
  pixels = 0;
  originalLines = 0;
  finalLines = 0;
  joins = 0;
  strings = 0;
  characters = 0;
  
  debugf("coordinate normalization starts (%.2f,%.2f) factors (%f,%f) linewidth %f",
         xStart, yStart, xFactor, yFactor, linewidth);

  lineTableInit();
  preamble();
}

SvgCreator::~SvgCreator() {
  lineTableOutput();
  postamble();
  unsigned long bytes = file.tellp();
  file.close();
  infof("  image statistics");
  infof("    file size %.1f KB", (bytes / 1000.0));
  infof("    image has %u pixels", pixels);
  infof("    image has %u lines (from originally %u line components)", finalLines, originalLines);
  infof("    image has %u strings (%u characters)", strings, characters);
  lineTableInfos();
  lineTableDeinit();
  lineTable = 0;
}

outlinerreal
SvgCreator::getPixelXSize(void) {
  return((1.0/xFactor)/multiplier);
}

outlinerreal
SvgCreator::getPixelYSize(void) {
  return((1.0/yFactor)/multiplier);
}

void
SvgCreator::line(outlinerreal fromX,
                 outlinerreal fromY,
                 outlinerreal toX,
                 outlinerreal toY,
                 bool dashed) {

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
  addLine(fromXInt,fromYInt,toXInt,toYInt,dashed);
}

void
SvgCreator::addLine(unsigned int x1,
                    unsigned int y1,
                    unsigned int x2,
                    unsigned int y2,
                    bool dashed) {
  deepdebugf("addLine");
  if (mergedLines) {
    unsigned int matchIndex;
    bool isStart;
    bool reverseOriginal;
    struct OutlinerSvgLine* match =
      matchingLine(x1,y1,
                   x2,y2,
                   dashed,
                   matchIndex,
                   isStart,
                   reverseOriginal);
    if (match == 0) {
      deepdebugf("no line match for (%u,%u)-(%u,%u)", x1, y1, x2, y2);
      struct OutlinerSvgLine* entry = new struct OutlinerSvgLine;
      if (entry == 0) {
        errf("Cannot allocate a line entry");
        exit(1);
      }
      entry->refCount = 0;
      entry->printed = 0;
      entry->nPoints = 2;
      entry->dashed = dashed;
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
      assert(dashed == match->dashed);
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
    line.dashed = dashed;
    emitLine(line);
  }
}

void
SvgCreator::emitLine(const struct OutlinerSvgLine& line) {
  deepdebugf("SvgCreator::emitLine");
  assert(line.nPoints >= 2);
  if (line.nPoints == 2) {
    file << "<line x1=\"" << line.points[0].x << "\" y1=\"" << line.points[0].y << "\"";
    file << " x2=\"" << line.points[1].x << "\" y2=\"" << line.points[1].y << "\"";
  } else {
    if (smooth) {
      file << "<path d=\"";
    } else {
      file << "<polyline points=\"";
    }
    for (unsigned int i = 0; i < line.nPoints; i++) {
      unsigned int x = line.points[i].x;
      unsigned int y = line.points[i].y;
      if (smooth) {
        if (i == 0) {
          file << x << " " << y <<  " ";
        } else {
          file << "S" << x << " " << y <<  " ";
        }
      } else {
        file << x << "," << y <<  " ";
      }
    }
    file << "\"";
  }
  if (smooth) {
    file << " fill=\"none\" stroke=\"black\" ";
  } else {
    file << " fill=\"none\" stroke=\"black\" ";
  }
  if (line.dashed) {
    file << " stroke-dasharray=\"1 1\" ";
  }
  file << "stroke-width=\"" << linewidth << "\" />\n";
  finalLines++;
}

void
SvgCreator::pixel(outlinerreal x,
                  outlinerreal y) {
  unsigned int xInt;
  unsigned int yInt;
  coordinateNormalization(x,y,xInt,yInt);
  debugf("SvgCreator::pixel %.2f,%.2f to %u,%u", x, y, xInt, yInt);
  file << "<rect x=\"" << xInt << "\" y=\"" << yInt << "\"";
  file << " width=\"" << multiplier << "\" height=\"" << multiplier << "\"";
  file << " fill=\"black\"";
  file << " stroke-width=\"0\" stroke=\"black\" />\n";
  pixels++;
}

void
SvgCreator::triangle(OutlinerTriangle2D triangle,
                     bool dashed) {

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
  line.dashed = dashed;
  
  // Emit the SVG code to draw that line
  emitLine(line);
}

void
SvgCreator::text(outlinerreal x,
                 outlinerreal y,
                 const char* string) {
  deepdebugf("SvgCreator::text");
  assert(string != 0);
  unsigned int xInt;
  unsigned int yInt;
  coordinateNormalization(x,y,xInt,yInt);
  file << "<text x=\"" << xInt << "\" y=\"" << yInt << "\"";
  file << " fill=\"black\">";
  file << string;
  file << "</text>\n";
  strings++;
  characters += strlen(string);
  debugf("text to (%.2f,%.2f) which is (%u,%u)", x, y, xInt, yInt);
}

void
SvgCreator::coordinateNormalization(outlinerreal x,
                                    outlinerreal y,
                                    unsigned int& xInt,
                                    unsigned int& yInt) {
  outlinerreal xNormalized = (x - xStart) * xFactor;
  outlinerreal yNormalized = (y - yStart) * yFactor;
  if (xNormalized > (outlinerreal)xSize) xInt = xSizeMultiplied; else xInt = (xNormalized*multiplier);
  if (yNormalized > ySize) yInt = ySizeMultiplied; else yInt = ySizeMultiplied - (yNormalized*multiplier);
  deepdebugf("coordinate normalization (%.2f,%.2f) to (%u,%u) with yNormalized %.2f ySize %u yStart %.2f and yFactor %.2f",
             x, y,
             xInt, yInt,
             yNormalized, ySize, yStart, yFactor);
  if (xInt > xSizeMultiplied || yInt > ySizeMultiplied) {
    infof("xInt %u (/%.2f) from x %.2f start %.2f factor %.2f multiplier %.2f",
          xInt, xSizeMultiplied, x, xStart, xFactor, multiplier);
    infof("yInt %u (/%.2f) from y %.2f start %.2f factor %.2f multiplier %.2f",
          yInt, ySizeMultiplied, y, yStart, yFactor, multiplier);
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
  file << "     width=\"" << (xSize+1)*multiplier << "\" height=\"" << (ySize+1)*multiplier << "\">\n";

  // Make the background white
  file << "<rect x=\"0\" y=\"0\" width=\"" << (xSize+1)*multiplier << "\" height=\"" << (ySize+1)*multiplier << "\"";
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
                         bool dashed,
                         unsigned int& matchIndex,
                         bool& matchesStart,
                         bool& reverseOriginal) {

  deepdebugf("matchingLine");
  unsigned int head = lineTableIndex(x1,y1);
  unsigned int tail = lineTableIndex(x2,y2);
  struct OutlinerSvgLine* result = 0;

  // See if we can find a line that ends in (x1,y1)
  if ((result = matchingLineAux(x1,y1,dashed,1,head)) != 0) {
    matchIndex = head;
    matchesStart = 0;
    reverseOriginal = 0;
    return(result);
  }

  // See if we can find a line that starts with (x2,y2)
  if ((result = matchingLineAux(x2,y2,dashed,0,tail)) != 0) {
    matchIndex = tail;
    matchesStart = 1;
    reverseOriginal = 0;
    return(result);
  }
  
  // See if we can find a line that ends in (x2,y2)
  if ((result = matchingLineAux(x2,y2,dashed,1,tail)) != 0) {
    matchIndex = tail;
    matchesStart = 0;
    reverseOriginal = 1;
    return(result);
  }

  // See if we can find a line that starts with (x1,y1)
  if ((result = matchingLineAux(x1,y1,dashed,0,head)) != 0) {
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
                            bool dashed,
                            bool lookForTailMatch,
                            unsigned int index) {
  return(matchingLineAuxAvoid(0,x,y,dashed,lookForTailMatch,
                              index));
}

struct OutlinerSvgLine*
SvgCreator::matchingLineAuxAvoid(struct OutlinerSvgLine* avoid,
                                 unsigned int x,
                                 unsigned int y,
                                 bool dashed,
                                 bool lookForTailMatch,
                                 unsigned int index) {
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
    //    3/ dashing does not match
    //
    
    if (line != avoid && line->nPoints < OutlinerSvgMaxLinePoints && dashed == line->dashed) {
    
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
    if ((result = matchingLineAuxAvoid(target,x,y,target->dashed,1,searchIndex)) != 0) {
      return(result);
    }
  } else {
    // See if we can find a line that starts with (x,y)
    if ((result = matchingLineAuxAvoid(target,x,y,target->dashed,0,searchIndex)) != 0) {
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
  assert(entry->dashed == join->dashed);
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
    exit(1);
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
    exit(1);
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


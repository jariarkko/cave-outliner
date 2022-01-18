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

#include <stdlib.h>
#include <cassert>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerhighprecision.hh"
#include "outlinermath.hh"
#include "outlinerdebug.hh"
#include "outlinersvg.hh"
#include "outlinermaterialmatrix2d.hh"
#include "outlinermaterialmatrix3d.hh"
#include "outlinerprocessorforms.hh"
#include "outlinerdepthmap.hh"
#include "outlineroutlineanalyzer.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Form analysis -- Construction //////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

OutlineAnalyzer::OutlineAnalyzer(const MaterialMatrix2D& matrix2In,
                                 const MaterialMatrix3D& matrix3In,
                                 ProcessorForms& formAnalyzerIn) :
  matrix2(matrix2In),
  matrix3(matrix3In),
  formAnalyzer(formAnalyzerIn),
  nTunnelSegments(0),
  nFailedZScans(0),
  descriptors(new struct OutlineSliceDescriptor [matrix2.xIndexSize]),
  floorDepthMap(matrix2.xIndexSize,
                matrix2.yIndexSize,
                matrix2),
  roofDepthMap(matrix2.xIndexSize,
               matrix2.yIndexSize,
               matrix2)
{
  if (matrix3.zIndexSize > outlinerdepth_maxvalue) {
    fatalf("Depth %u exceeds analyzer ability to store depth values (max %u)",
	   matrix3.zIndexSize, outlinerdepth_maxvalue);
    return;
  }
  if (descriptors == 0) {
    errf("Cannot allocate %u outline descriptors", matrix2.xIndexSize);
    return;
  }
}

OutlineAnalyzer::~OutlineAnalyzer() {
  debugf("OutlineAnalyzer::~OutlineAnalyzer start");
  if (descriptors != 0) {
    delete [] descriptors;
    descriptors = 0;
  }
  debugf("OutlineAnalyzer::~OutlineAnalyzer done");
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Functions to access results ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

const DepthMap&
OutlineAnalyzer::getFloorDepthMap(void) {
  return(floorDepthMap);
}

const DepthMap&
OutlineAnalyzer::getRoofDepthMap(void) {
  return(roofDepthMap);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Outline analysis -- Main program ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
OutlineAnalyzer::analyze(void) {
  infof("Outline analysis starting...");
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    if (!analyzeOneSlice(xIndex)) return(0);
  }
  infof("  outline analysis found %u tunnel segments in %u slices (and %u failed z scans)",
        nTunnelSegments, matrix3.xIndexSize, nFailedZScans);
  createDepthMaps();
  infof("Outline analysis done");
  return(1);
}

bool
OutlineAnalyzer::analyzeOneSlice(unsigned int xIndex) {
  infof("OutlineAnalyzer::analyzeOneSlice(%u)", xIndex);
  assert(xIndex < matrix3.xIndexSize);
  struct OutlineSliceDescriptor& slice = descriptors[xIndex];
  memset(&slice,0,sizeof(slice));
  slice.xIndex = xIndex;
  slice.nTunnels = 0;
  for (unsigned int yIndex = 0; yIndex < matrix3.yIndexSize - 2; yIndex++) {
    unsigned int matrix2xIndexStart;
    unsigned int matrix2yIndexStart;
    unsigned int matrix2xIndexEnd;
    unsigned int matrix2yIndexEnd;
    if (!formAnalyzer.condensedIndexesToIndexes(xIndex,yIndex,
                                                matrix2xIndexStart,
                                                matrix2yIndexStart,
                                                matrix2xIndexEnd,
                                                matrix2yIndexEnd)) continue;
    if (matrix2.getMaterialMatrixRange(matrix2xIndexStart,
                                       matrix2yIndexStart,
                                       matrix2xIndexEnd,
                                       matrix2yIndexEnd)) {
      struct OutlineSliceTunnelDescriptor* tunnel = allocateTunnel(xIndex,yIndex,slice);
      if (tunnel == 0) break;
      if (!analyzeOneTunnelSlice(*tunnel,
                                 xIndex,yIndex,
                                 matrix2xIndexStart,
                                 matrix2yIndexStart,
                                 matrix2xIndexEnd,
                                 matrix2yIndexEnd)) {
        deallocateTunnel(tunnel,slice);
      }
    }
  }
  return(1);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Tunnel analysis ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
OutlineAnalyzer::analyzeOneTunnelSlice(struct OutlineSliceTunnelDescriptor& tunnel,
                                       const unsigned int matrix3xIndex,
                                       unsigned int& matrix3yIndex,
                                       const unsigned int matrix2xIndexStart,
                                       const unsigned int matrix2yIndexStart,
                                       const unsigned int matrix2xIndexEnd,
                                       const unsigned int matrix2yIndexEnd) {

  //
  // Add more information to the tunnel descriptor
  //

  tunnel.zFound = 1;
  bool ans = ((matrix3yIndex < matrix3.yIndexSize - 1) ?
              findZMidPointAlternatives(matrix3xIndex,matrix3yIndex,matrix3yIndex+1,tunnel.startZ) :
              findZMidPoint(matrix3xIndex,matrix3yIndex,tunnel.startZ));
  if (!ans) {
    infof("  Failed to find begin z at %u", matrix3xIndex);
    nFailedZScans++;
    tunnel.zFound = 0;
  }
  tunnel.emptySpace = 0;
  
  //
  // Initialize search
  //
  
  unsigned int matrix2yIndexStartVar = matrix2yIndexStart;
  unsigned int matrix2yIndexEndVar = matrix2yIndexEnd;

  //
  // Search in y-direction
  //
  
  for (;;) {
    
    //
    // Jump to the next step in y-direction
    //

    if (matrix3yIndex >= matrix3.yIndexSize - 1) break;
    matrix3yIndex++;
    if (!formAnalyzer.condensedXIndexIncrease(matrix2yIndexStartVar)) break;
    if (!formAnalyzer.condensedYIndexIncrease(matrix2yIndexEndVar)) break;

    //
    // If the tunnel is marked as an tunnel place in the forms matrix,
    // we know there's empty space in the tunnel
    //

    assert(matrix2xIndexStart < matrix2.xIndexSize);
    assert(matrix2yIndexStartVar < matrix2.yIndexSize);
    if (outlinerform_istunnel(formAnalyzer.getForm(matrix2xIndexStart,matrix2yIndexStartVar))) {
      infof("      found empty space at %u", matrix3yIndex);
      tunnel.emptySpace = 1;
    }
    
    //
    // If we hit a no-material spot, we've completed the tunnel
    //
    
    assert(matrix2yIndexEndVar < matrix2.yIndexSize);
     if (!matrix2.getMaterialMatrixRange(matrix2xIndexStart,
                                         matrix2yIndexStartVar,
                                         matrix2xIndexEnd,
                                         matrix2yIndexEndVar)) {
      matrix3yIndex--;
      break;
    }
  }

  //
  // Done. Finalize the y range in the tunnel description.
  //

  tunnel.yIndexRange.end = matrix3yIndex;
  tunnel.yMidPoint = (tunnel.yIndexRange.start + tunnel.yIndexRange.end) / 2;
  ans = ((matrix3yIndex > 0) ?
         findZMidPointAlternatives(matrix3xIndex,matrix3yIndex,matrix3yIndex-1,tunnel.endZ) :
         findZMidPoint(matrix3xIndex,matrix3yIndex,tunnel.endZ));
  if (!ans) {
    infof("  Failed to find end z at %u", matrix3xIndex);
    nFailedZScans++;
    tunnel.zFound = 0;
  }
  infof("  tunnel midpoint at x %u = y %u (z %u..%u)",
        matrix3xIndex, tunnel.yMidPoint,
        tunnel.startZ, tunnel.endZ);
  return(1);
}

bool
OutlineAnalyzer::findZMidPoint(const unsigned int matrix3xIndex,
                               const unsigned int matrix3yIndex,
                               unsigned int& matrix3zIndex) const {

  //
  // Scan to the first place in z-dimension that has material at this
  // (x,y) position.
  //

  debugf("      finding a z at (%u,%u)", matrix3xIndex, matrix3yIndex);
  unsigned int zIndex = matrix3.zIndexSize - 1;
  while (!matrix3.getMaterialMatrix(matrix3xIndex,matrix3yIndex,zIndex)) {
    if (zIndex == 0) {
      debugf("      no z material at (%u,%u)", matrix3xIndex, matrix3yIndex);
      return(0);
    }
    zIndex--;
  }

  //
  // We've found material. Now see how long it continues (in case
  // there's more than one pixel).
  //

  unsigned int zStart = zIndex;
  while (zIndex > 0) {
    zIndex--;
    if (!matrix3.getMaterialMatrix(matrix3xIndex,matrix3yIndex,zIndex)) {
      zIndex++;
      break;
    }
  }
  
  //
  // We found the start and end of the stretch of z pixels turned on. Take the midpoint.
  //

  matrix3zIndex = ((zStart + zIndex) / 2);
  debugf("      z material at (%u,%u) found at level %u",
         matrix3xIndex, matrix3yIndex, matrix3zIndex);
  return(1);
}

bool
OutlineAnalyzer::findZMidPointAlternatives(const unsigned int matrix3xIndex,
                                           const unsigned int matrix3yIndexAlt1,
                                           const unsigned int matrix3yIndexAlt2,
                                           unsigned int& matrix3zIndex) const {
  if (findZMidPoint(matrix3xIndex,matrix3yIndexAlt1,matrix3zIndex)) return(1);
  else return(findZMidPoint(matrix3xIndex,matrix3yIndexAlt2,matrix3zIndex));
}

struct OutlineSliceTunnelDescriptor*
OutlineAnalyzer::allocateTunnel(const unsigned int matrix3xIndex,
                                const unsigned int matrix3yIndex,
                                struct OutlineSliceDescriptor& slice) {
  assert(matrix3xIndex < matrix3.xIndexSize);
  assert(matrix3yIndex < matrix3.yIndexSize);
  if (slice.nTunnels == outlinermaxtunnelsinoneslice) {
    warnf("Too complex tunnel, cannot allocacte more than %u tunnels in one slice",
          outlinermaxtunnelsinoneslice);
    return(0);
  } else {
    
    //
    // Initialize tunnel descriptor
    //

    struct OutlineSliceTunnelDescriptor* tunnel = &slice.tunnels[slice.nTunnels++];
    tunnel->xIndex = matrix3xIndex;
    tunnel->yIndexRange.start = matrix3yIndex;
    
    //
    // Statistics
    //
    
    nTunnelSegments++;

    //
    // Debugs
    //
    
    debugf("    allocated a new tunnel at x %u, starting from y %u", matrix3xIndex, matrix3yIndex);
    
    //
    // Return result
    //
    
    return(tunnel);
  }
}

void
OutlineAnalyzer::deallocateTunnel(struct OutlineSliceTunnelDescriptor* tunnel,
                                  struct OutlineSliceDescriptor& slice) {
  assert(slice.nTunnels > 0);
  assert(tunnel == &slice.tunnels[slice.nTunnels-1]);
  slice.nTunnels--;
  nTunnelSegments--;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Depth maps /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
OutlineAnalyzer::createDepthMaps(void) {
  infof("Creating a floor depth map...");
  createDepthMap(floorDepthMap,1);
  infof("Creating a roof depth map...");
  createDepthMap(roofDepthMap,0);
}

void
OutlineAnalyzer::createDepthMap(DepthMap& map,
                                const bool floor) {
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    const struct OutlineSliceDescriptor& slice = descriptors[xIndex];
    for (unsigned int t = 0; t < slice.nTunnels; t++) {
      const struct OutlineSliceTunnelDescriptor& tunnel = slice.tunnels[t];
      createDepthMapTunnel(map,tunnel,floor);
    }
  }
}

void
OutlineAnalyzer::createDepthMapTunnel(DepthMap& map,
                                      const struct OutlineSliceTunnelDescriptor& tunnel,
                                      const bool floor) {
  outlinerdepth def = (floor ? 0 : (matrix3.yIndexSize-2));
  outlinerreal border = tunnel.startZ;
  outlinerreal steps = tunnel.yIndexRange.end - tunnel.yIndexRange.start;
  outlinerreal diff = ((outlinerreal)tunnel.endZ) - ((outlinerreal)tunnel.startZ);
  outlinerreal borderStep;
  if (steps <= 1) {
    borderStep = 0;
  } else {
    borderStep = diff / steps;
  }
  debugf("depth map tunnel at x %u, y %u..%u (n=%.2f), border %.2f (%u..%u), step %.2f",
        tunnel.xIndex,
        tunnel.yIndexRange.start, tunnel.yIndexRange.end,
        steps,
        border,
        tunnel.startZ, tunnel.endZ,
        borderStep);
  for (unsigned int yIndex = tunnel.yIndexRange.start;
       yIndex <= tunnel.yIndexRange.end;
       yIndex++, border += borderStep) {
    unsigned int matrix2xIndexStart;
    unsigned int matrix2yIndexStart;
    unsigned int matrix2xIndexEnd;
    unsigned int matrix2yIndexEnd;
    if (!formAnalyzer.condensedIndexesToIndexes(tunnel.xIndex,yIndex,
                                                matrix2xIndexStart,
                                                matrix2yIndexStart,
                                                matrix2xIndexEnd,
                                                matrix2yIndexEnd)) continue;
    outlinerdepth thisDepth = def;
    if (matrix2.getMaterialMatrixRange(matrix2xIndexStart,
                                       matrix2yIndexStart,
                                       matrix2xIndexEnd,
                                       matrix2yIndexEnd)) {
      outlinerdepth depth;
      if (tunnel.zFound) {
        bool succ = getDepth(tunnel.xIndex,
                             yIndex,
                             (unsigned int)border,
                             floor,
                             depth);
        thisDepth = depth;
        if (!succ) {
          debugf("  depth %s (%u,%u) no success, continuing",
                floor ? "floor" : "roof", matrix2xIndexStart, matrix2yIndexStart);
          thisDepth = def;
        } else {
          debugf("  depth %s map tunnel border now %.2f (%u,%u) at y %u = %u",
                floor ? "floor" : "roof",
                border,
                matrix2xIndexStart, matrix2yIndexStart,
                yIndex,
                depth);
        }
      } else {
        debugf("  depth %s (%u,%u) no z success earlier, continuing",
                floor ? "floor" : "roof", matrix2xIndexStart, matrix2yIndexStart);
        thisDepth = def;
      }
    } else {
      debugf("  depth %s tunnel (%u,%u) no material, continuing",
            floor ? "floor" : "roof", matrix2xIndexStart, matrix2yIndexStart);
    }
    debugf("  depth %s range set (%u,%u)..(%u,%u) = %u",
          floor ? "floor" : "roof",
          matrix2xIndexStart,
          matrix2yIndexStart,
          matrix2xIndexEnd,
          matrix2yIndexEnd,
          thisDepth);
    map.setDepthRange(matrix2xIndexStart,
                      matrix2yIndexStart,
                      matrix2xIndexEnd,
                      matrix2yIndexEnd,
                      thisDepth);
  }
}

bool
OutlineAnalyzer::getDepth(const unsigned int xIndex,
                          const unsigned int yIndex,
                          const unsigned int zIndexFloorRoofBorder,
                          const bool floor,
                          outlinerdepth& result) const {
  assert(xIndex < matrix3.xIndexSize);
  assert(yIndex < matrix3.yIndexSize);
  assert(zIndexFloorRoofBorder < matrix3.zIndexSize);

  unsigned int zIndex = zIndexFloorRoofBorder;
  for (;;) {
    if (matrix3.getMaterialMatrix(xIndex,yIndex,zIndex)) {
      result = zIndex;
      return(1);
    }
    if (floor) {
      if (zIndex == 0) return(0);
      else zIndex--;
    } else {
      if (zIndex >= matrix3.zIndexSize - 2) return(0);
      else zIndex++;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Drawing results ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
OutlineAnalyzer::drawSpines(SvgCreator& svg) const {
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    const struct OutlineSliceDescriptor& slice = descriptors[xIndex];
    for (unsigned int t = 0; t < slice.nTunnels; t++) {
      const struct OutlineSliceTunnelDescriptor& tunnel = slice.tunnels[t];
      drawSpineElement(slice,tunnel,svg);
    }
  }
}

void
OutlineAnalyzer::drawSpineElement(const struct OutlineSliceDescriptor& slice,
                                  const struct OutlineSliceTunnelDescriptor& tunnel,
                                  SvgCreator& svg) const {
    unsigned int matrix2xIndexStart;
    unsigned int matrix2yIndexStart;
    unsigned int matrix2xIndexEnd;
    unsigned int matrix2yIndexEnd;
    if (!formAnalyzer.condensedIndexesToIndexes(tunnel.xIndex,tunnel.yMidPoint,
                                                matrix2xIndexStart,
                                                matrix2yIndexStart,
                                                matrix2xIndexEnd,
                                                matrix2yIndexEnd)) return;
    outlinerreal x = matrix2.indexToCoordinateX(matrix2xIndexStart);
    outlinerreal y = matrix2.indexToCoordinateY(matrix2yIndexStart);
    svg.pixel(x,y,outlinersvgstyle_red);
}


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
  descriptors(new struct OutlineSliceDescriptor [matrix2.xIndexSize])
{
  if (descriptors == 0) {
    errf("Cannot allocate %u outline descriptors", matrix2.xIndexSize);
    exit(1);
  }
}

OutlineAnalyzer::~OutlineAnalyzer() {
  if (descriptors != 0) {
    delete [] descriptors;
    descriptors = 0;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Outline analysis -- Main program ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
OutlineAnalyzer::analyze(void) {
  infof("OutlineAnalyzer::analyze");
  for (unsigned int xIndex = 0; xIndex < matrix3.xIndexSize - 2; xIndex++) {
    if (!analyzeOneSlice(xIndex)) return(0);
  }
  infof("  outline analysis found %u tunnel segments in %u slices (and %u failed z scans)",
        nTunnelSegments, matrix3.xIndexSize, nFailedZScans);
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
    formAnalyzer.condensedIndexesToIndexes(xIndex,yIndex,
                                           matrix2xIndexStart,
                                           matrix2yIndexStart,
                                           matrix2xIndexEnd,
                                           matrix2yIndexEnd);
    if (matrix2.getMaterialMatrix(matrix2xIndexStart,
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

  bool ans = findZMidPointAlternatives(matrix3xIndex,matrix3yIndex,matrix3yIndex+1,tunnel.startZ);
  if (!ans) {
    infof("  Failed to find begin z at %u", matrix3xIndex);
    nFailedZScans++;
    return(0);
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
    formAnalyzer.condensedIndexIncrease(matrix2yIndexStartVar);
    formAnalyzer.condensedIndexIncrease(matrix2yIndexEndVar);

    //
    // If the tunnel is marked as an tunnel place in the forms matrix,
    // we know there's empty space in the tunnel
    //
    
    if (outlinerform_istunnel(formAnalyzer.getForm(matrix2xIndexStart,matrix2yIndexStartVar))) {
      infof("      found empty space at %u", matrix3yIndex);
      tunnel.emptySpace = 1;
    }
    
    //
    // If we hit a no-material spot, we've completed the tunnel
    //
    
    assert(matrix2yIndexEndVar < matrix2.yIndexSize);
    if (!matrix2.getMaterialMatrix(matrix2xIndexStart,
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

  tunnel.yIndexRange.end = matrix3yIndex-1;
  tunnel.yMidPoint = (tunnel.yIndexRange.start + tunnel.yIndexRange.end) / 2;
  ans = findZMidPointAlternatives(matrix3xIndex,matrix3yIndex,matrix3yIndex-1,tunnel.endZ);
  if (!ans) {
    infof("  Failed to find end z at %u", matrix3xIndex);
    nFailedZScans++;
    return(0);
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

  infof("      finding a z at (%u,%u)", matrix3xIndex, matrix3yIndex);
  unsigned int zIndex = matrix3.zIndexSize - 1;
  while (!matrix3.getMaterialMatrix(matrix3xIndex,matrix3yIndex,zIndex)) {
    infof("      no z material at (%u,%u,%u)", matrix3xIndex, matrix3yIndex, zIndex);
    if (zIndex == 0) return(0);
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
    } else {
      infof("      still z material at %u", zIndex);
    }
  }
  
  //
  // We found the start and end of the stretch of z pixels turned on. Take the midpoint.
  //

  matrix3zIndex = ((zStart + zIndex) / 2);
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
    
    infof("    allocated a new tunnel at x %u, starting from y %u", matrix3xIndex, matrix3yIndex);
    
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
    formAnalyzer.condensedIndexesToIndexes(tunnel.xIndex,tunnel.yMidPoint,
                                           matrix2xIndexStart,
                                           matrix2yIndexStart,
                                           matrix2xIndexEnd,
                                           matrix2yIndexEnd);
    outlinerreal x = matrix2.indexToCoordinateX(matrix2xIndexStart);
    outlinerreal y = matrix2.indexToCoordinateY(matrix2yIndexStart);
    svg.pixel(x,y,outlinersvgstyle_red);
}


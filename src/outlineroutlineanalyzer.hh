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

#ifndef OUTLINEANALYZER_HH
#define OUTLINEANALYZER_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"
#include "outlinerdepthmap.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

struct OutlinerIndexRange {
  unsigned int start;
  unsigned int end;
};

struct OutlineSliceTunnelDescriptor {
  unsigned int xIndex;
  OutlinerIndexRange yIndexRange;
  unsigned int yMidPoint;
  bool zFound;
  unsigned int startZ;
  unsigned int endZ;
  bool emptySpace;
};

struct OutlineSliceDescriptor {
  unsigned int xIndex;
  unsigned int nTunnels;
  struct OutlineSliceTunnelDescriptor tunnels[outlinermaxtunnelsinoneslice];
};

class SvgCreator;
class ProcessorForms;
class MaterialMatrix2D;
class MaterialMatrix3D;
class DepthMap;

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class OutlineAnalyzer {

public:
  
  OutlineAnalyzer(const MaterialMatrix2D& matrix2In,
                  const MaterialMatrix3D& matrix3In,
                  ProcessorForms& formAnalyzerIn);
  ~OutlineAnalyzer();
  bool analyze(void);
  const DepthMap& getFloorDepthMap(void);
  const DepthMap& getRoofDepthMap(void);
  void drawSpines(SvgCreator& svg) const;
  
private:
  
  //
  // Private variables
  //

  const MaterialMatrix2D& matrix2;
  const MaterialMatrix3D& matrix3;
  ProcessorForms& formAnalyzer;
  unsigned int nTunnelSegments;
  unsigned int nFailedZScans;
  struct OutlineSliceDescriptor* descriptors;
  DepthMap floorDepthMap;
  DepthMap roofDepthMap;
  
  //
  // Internal functions
  //

  bool analyzeOneSlice(unsigned int xIndex);
  bool analyzeOneTunnelSlice(struct OutlineSliceTunnelDescriptor& tunnel,
                             const unsigned int matrix3xIndex,
                             unsigned int& matrix3yIndex,
                             const unsigned int matrix2xIndexStart,
                             const unsigned int matrix2yIndexStart,
                             const unsigned int matrix2xIndexEnd,
                             const unsigned int matrix2yIndexEnd);
  bool findZMidPoint(const unsigned int matrix3xIndex,
                     const unsigned int matrix3yIndex,
                     unsigned int& matrix3zIndex) const;
  bool findZMidPointAlternatives(const unsigned int matrix3xIndex,
                                 const unsigned int matrix3yIndexAlt1,
                                 const unsigned int matrix3yIndexAlt2,
                                 unsigned int& matrix3zIndex) const;

  //
  // Tunnel structure allocation/deallocation
  //

  struct OutlineSliceTunnelDescriptor* allocateTunnel(const unsigned int matrix3xIndex,
                                                      const unsigned int matrix3yIndex,
                                                      struct OutlineSliceDescriptor& slice);
  void deallocateTunnel(struct OutlineSliceTunnelDescriptor* tunnel,
                        struct OutlineSliceDescriptor& slice);

  //
  // Depth maps
  //

  void createDepthMaps(void);
  void createDepthMap(DepthMap& map,
                      const bool floor);
  void createDepthMapTunnel(DepthMap& map,
                            const struct OutlineSliceTunnelDescriptor& tunnel,
                            const bool floor);
  bool getDepth(const unsigned int xIndex,
                const unsigned int yIndex,
                const unsigned int zIndexFloorRoofBorder,
                const bool floor,
                outlinerdepth& result) const;
  
  //
  // Drawing
  //

  void drawSpineElement(const struct OutlineSliceDescriptor& slice,
                        const struct OutlineSliceTunnelDescriptor& tunnel,
                        SvgCreator& svg) const;
};

#endif // OUTLINEANALYZER_HH

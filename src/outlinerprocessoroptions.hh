
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

#ifndef PROCESSOROPTIONS_HH
#define PROCESSOROPTIONS_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include "outlinerdirection.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// The ProcessorOptions class represents an object of all configuration
/// information related to the cave-outliner program. It is
/// initialized to default values, and the main program can read
/// options and other command line arguments to set specific values in
/// the configuration. Once the processing of the actual function in
/// cave-outliner begins, the configuration data is read from this
/// object to decide how to process.
///

class ProcessorOptions {

public:

  /// Create the configuration object, with default values.
  ProcessorOptions();

  /// Create the configuration object, with given values.
  ProcessorOptions(const unsigned int holeThresholdIn,
                   const unsigned int lineHoleThresholdIn,
                   const unsigned int dustThresholdIn,
                   const char* floorDepthMapIn,
                   const char* roofDepthMapIn,
                   const bool floorStyleDiffIn,
                   const bool tunnelSpineIn,
                   const bool labelsIn,
                   const bool formAnalysisIn,
                   const unsigned int formCondenseIn,
                   const bool dimensionsIn);
  
  /// Assign the configuration object, as a copy from another object.
  ProcessorOptions& operator=(const ProcessorOptions& input);

  /// Release all resources for the configuration object.
  ~ProcessorOptions();
  
  /// If the model has holes, e.g., missing faces for various reasons, should the software try to fill them? 
  unsigned int holeThreshold;

  /// If the cross section lines have holes, e.g., missing faces for various reasons, should the software try to fill them? 
  unsigned int lineHoleThreshold;

  /// Can remove small isolated pieces of model?
  unsigned int dustThreshold;
  
  /// Should we output the cave floor depth map to a separate file?
  const char* floorDepthMap;
  
  /// Should we output the cave roof depth map to a separate file?
  const char* roofDepthMap;

  /// Floor style, either depth map (0) or differential depth  map (1).
  bool floorStyleDiff;

  /// Should we show tunnel 'spines', i.e., tunnel midpoints?
  bool tunnelSpine;
  
  /// Should we label cross sections in the images?
  bool labels;
  
  /// Should cave outliner perform form analysis of cave entrances,
  /// stones, drops, stalactites, stalagmites, etc?
  bool formAnalysis;
  
  /// If forms are analysed, what's the compression factor with
  /// respect to the picture size?
  unsigned int formCondense;
  
  /// Should we show dimensions?
  bool dimensions;
};

#endif // PROCESSOROPTIONS_HH

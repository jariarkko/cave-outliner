
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

#ifndef MAINCONFIG_HH
#define MAINCONFIG_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include "outlinerdirection.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// The MainConfig class represents an object of all configuration
/// information related to the cave-outliner program. It is
/// initialized to default values, and the main program can read
/// options and other command line arguments to set specific values in
/// the configuration. Once the processing of the actual function in
/// cave-outliner begins, the configuration data is read from this
/// object to decide how to process.
///

class MainConfig {

public:

  /// Create the configuration object, with default values.
  MainConfig();

  /// Release all resources for the configuration object.
  ~MainConfig();

  /// Get a label suitable for a cross section ("A", "B", etc.)
  const char* getCrossSectionLabel(void);

  /// Input 3D model file name
  char* inputFile;

  /// Output SVG image file name
  char* outputFile;

  /// True if we should run unit tests
  bool test;

  /// True if informational messages should be displayed
  bool info;

  /// True if level 1 debugging should be provided
  bool debug;
  
  /// True if level 2 debugging should be provided
  bool deepdebug;
  
  /// True if level 3 debugging should be provided
  bool deepdeepdebug;

  /// In how small steps the model should be scanned in x coordinate direction.
  float stepx;
  
  /// In how small steps the model should be scanned in y coordinate direction.
  float stepy;
  
  /// In how small steps the model should be scanned in z coordinate direction.
  float stepz;

  /// True if the bounding box has been specified
  bool boundingBoxSet;

  /// The bounding box we should be looking at.
  OutlinerBox3D boundingBox;

  /// Direction of view for the plan view (usually along z axis).
  enum outlinerdirection direction;

  /// What drawing algorithm should be used.
  enum outlineralgorithm algorithm;

  /// For lines, how thick should the lines be?
  float linewidth;

  /// Should one inspected step in the model result in one pixel, or multiplied to a number of pixels in the image?
  unsigned int multiplier;

  /// True if lines should be smoothed.
  bool smooth;

  /// True if short line segments should be merged in an SVG image.
  bool mergedLines;

  /// How many tiles should we have in each of the two (x,y)
  /// dimensions? Each tile represents a fast index to the 3D model
  /// faces within that space.
  unsigned int tiles;

  /// If the model has holes, e.g., missing faces for various reasons, should the software try to fill them? 
  unsigned int holethreshold;

  /// If the cross section lines have holes, e.g., missing faces for various reasons, should the software try to fill them? 
  unsigned int lineholethreshold;

  /// Should we create a number of automatic cross sections?
  bool automaticCrossSections;

  /// Direction of the automatic cross sections
  enum outlinerdirection automaticCrossSectionsDirection;
  
  /// How many automatic cross sections do we wish?
  unsigned int nAutomaticCrossSections;

  /// What is the name of the cross section files? Use '%' to
  /// represent the cross section counter, so that each image gets a
  /// different file name.
  const char* automaticCrossSectionFilenamePattern;

  /// How many cross sections did we in the end have? (Manual + automatic).
  unsigned int nCrossSections;

  /// Specification of where the cross sections should be drawn, what
  /// file names and labels they should use, etc.
  struct ProcessorCrossSectionInfo crossSections[outlinermaxcrosssections];
  enum outlinerdirection crossSectionDirections[outlinermaxcrosssections];
  outlinerreal crossSectionPoints[outlinermaxcrosssections];
  
  /// Width of cross sections (in unit of one step, e.g., 2.0 means
  /// analysis of a cross slice looks at two steps of 3D model data).
  outlinerreal crossSectionWidth;

  /// Should we label cross sections in the images?
  bool labelCrossSections;

  /// Should cave outliner perform form analysis of cave entrances,
  /// stones, drops, stalactites, stalagmites, etc?
  bool formanalysis;
  
  /// Should we show dimensions?
  bool dimensions;
  
private:
  
  unsigned int crossSectionLabelCount;
  
};

#endif // MAINCONFIG_HH

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

#ifndef SVGSTACKER_HH
#define SVGSTACKER_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include "outlinertypes.hh"
#include "outlinerhighprecision.hh"
#include "outlinersvgoptions.hh"
#include "outlinersvg.hh"
#include "outlinersvgreader.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This object can be used to read SVG images from a file.
///

class SvgStacker {
  
 public:

  /// Create an SVG file with given file name, by tiling a number of
  /// input SVG images on top of each other (vertically).
  SvgStacker(const char* fileNameIn,
	     const unsigned int nImageFileNames,
	     const char** imageFileNames,
	     const unsigned int* imageXIndents, // array can be 0 => no indent
	     const SvgOptions& optionsIn,
	     const unsigned int verticalSpacing);
  
  /// Release all resources associated with the SVG stacker object. And close the file, if still open.
  ~SvgStacker();
  
  /// Verify that image creation and file write was successful.
  bool ok();

  /// Unit tests
  static void test(void);
  
 private:

  const char* fileName;
  const SvgOptions options;
  bool good;
  std::ofstream file;
  
  bool importSvgContents(SvgReader& reader,
			 unsigned int xOffset,
			 unsigned int yOffset);
  bool addXYOffsetRectangle(SvgReader& reader,
			    const char* statement,
			    unsigned int xOffset,
			    unsigned int yOffset);
  bool addXYOffsetLine(SvgReader& reader,
		       const char* statement,
		       unsigned int xOffset,
		       unsigned int yOffset);
  bool addXYOffsetPolyline(SvgReader& reader,
			   const char* statement,
			   unsigned int xOffset,
			   unsigned int yOffset);
  bool addXYOffsetText(SvgReader& reader,
		       const char* statement,
		       unsigned int xOffset,
		       unsigned int yOffset);
  static bool pointsStringAddXYOffset(const char* optionValue,
				      unsigned int optionValueLength,
				      unsigned int xOffset,
				      unsigned int yOffset,
				      char* newValue,
				      unsigned int newValueSize,
				      const char* fileNameRead,
				      const unsigned int lineNoRead);
  const char* findchar(const char* input,
		       unsigned int inputLength,
		       char search) const;
  void outputCopiedText(const char* ptr,
			unsigned int n);
};

#endif // SVGSTACKER_HH

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

#ifndef SVGREADER_HH
#define SVGREADER_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include "outlinertypes.hh"
#include "outlinerhighprecision.hh"
#include "outlinersvgoptions.hh"
#include "outlinersvg.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

enum SvgStatement {
  svgstatement_xml,
  svgstatement_doctype,
  svgstatement_svgstart,
  svgstatement_rect,
  svgstatement_line,
  svgstatement_polyline,
  svgstatement_text,
  svgstatement_svgend,
  svgstatement_eof
};

struct SvgReaderOptionParser {
  const char* ptr;
  const char* fileName;
  unsigned int lineNo;
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This object can be used to read SVG images from a file.
///

class SvgReader {
  
 public:

  /// Create an SVG file with given file name and size.
  SvgReader(const char* fileNameIn);

  /// Release all resources associated with the SVG reader object. And close the file, if still open.
  ~SvgReader();

  /// Find out the size of the SVG image. Return 1 upon success and set
  /// output parameters, otherwise return 0.
  bool getSize(unsigned int& xSize,
	       unsigned int& ySize);

  /// Read the SVG file statement-by-statement. Return 1 upon success,
  /// and set the output parameters, otherwise return 0. End of file
  /// is reported as output parameter statementType set to
  /// svgstatement_eof.
  bool getStatement(enum SvgStatement& statementType,
		    unsigned int& lineno,
		    const char*& statement);

  /// Initialize the parsing of a statement and its options.
  static bool iterateStatementOptionsInit(const char* statement,
					  const char* fileName,
					  const unsigned int lineNo,
					  struct SvgReaderOptionParser& iter);

  /// Move to the next iterated option
  static bool iterateStatementOptionsNext(struct SvgReaderOptionParser& iter,
					  bool& end,
					  const char*& optionName,
					  unsigned int& optionNameLength,
					  const char*& optionValue,
					  unsigned int& optionValueLength);

  /// Peek if there's a given option with a given string in the option
  /// value later in the input
  static bool iterateStatementOptionsPeek(const struct SvgReaderOptionParser& iter,
					  const char* optionName,
					  unsigned int optionNameLength,
					  const char* optionValue,
					  unsigned int optionValueLength);
  
  /// Get the current line we are on.
  unsigned int getLine() const;
  
  /// Get the file name we are reading.
  const char* getFileName() const;
  
  /// Unit tests
  static void test(void);
  
 private:

  const char* fileName;
  std::ifstream* file;
  unsigned int curline;
# define statementSpaceMax (10*1024)
  char statementSpace[statementSpaceMax];
  
  bool parseStatement(enum SvgStatement& statementType,
		      unsigned int lineno,
		      const char* statement) const;
  static bool getIntComponent(const char* statement,
			      const char* field,
			      unsigned int& value);
};

#endif // SVGREADER_HH

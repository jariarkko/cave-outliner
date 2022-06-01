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

#include <ctype.h>
#include <stdlib.h>
#include <string>
#include <cassert>
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdebug.hh"
#include "outlinersvgoptions.hh"
#include "outlinersvg.hh"
#include "outlinersvgreader.hh"
#include "outlinerdebug.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

struct SvgStatementEntry {
  const char* beginsWith;
  unsigned int length;
  enum SvgStatement type;
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Constants //////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#define nStatementEntries 8
static const SvgStatementEntry statementEntries[nStatementEntries] {
  { "<?xml ", 6, svgstatement_xml },
  { "<!DOCTYPE svg ", 14, svgstatement_doctype },
  { "<svg ", 5, svgstatement_svgstart },
  { "<rect ", 6, svgstatement_rect },
  { "<line ", 6, svgstatement_line },
  { "<polyline ", 10, svgstatement_polyline },
  { "<text ", 6, svgstatement_text },
  { "</svg>", 6,svgstatement_svgend }
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Construction ///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

SvgReader::SvgReader(const char* fileNameIn) :
  fileName(fileNameIn),
  file(0),
  curline(0) {
}

SvgReader::~SvgReader() {
  if (file != 0) {
    file->close();
    delete file;
    file = 0;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Actual functionality ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
SvgReader::getSize(unsigned int& xSize,
		   unsigned int& ySize) {
  enum SvgStatement statementType;
  unsigned int lineno;
  const char* statement;
  
  while (getStatement(statementType,lineno,statement)) {
    
    switch (statementType) {
      
    case svgstatement_eof:
      // Cannot parse
      errf("Cannot parse SVG file %s, missing <svg> statement", fileName);
      return(0);
      
    case svgstatement_svgstart:
      deepdebugf("statement = %s", statement);
      if (!getIntComponent(statement,"width",xSize)) {
	errf("Cannot parse SVG file <svg> statement in %s line %u, missing width field", fileName);
	return(0);
      }
      if (!getIntComponent(statement,"height",ySize)) {
	errf("Cannot parse SVG file <svg> statement in %s line %u, missing height field", fileName);
	return(0);
      }
      // Success!
      return(1);
      
    default: continue;
      
    }
  }
  
  errf("Cannot open SVG file %s", fileName);
  return(0);
}

bool
SvgReader::getStatement(enum SvgStatement& statementType,
			unsigned int& lineno,
			const char*& statement) {
  // Is the file open? If not, open it
  if (file == 0) {
    file = new std::ifstream(fileName);
    curline = 1;
  }

  // Is the file now open, or closed/in error?
  if (!file->is_open()) {
    errf("Cannot open SVG file %s", fileName);
    file->close();
    delete file;
    file = 0;
    lineno = 0;
    return(0);
  }

  // It is open and OK, now read one line
  std::string line;
  if (std::getline(*file,line)) {
    if (line.length() + 1 > statementSpaceMax) {
      errf("Line too long on %s:%u, %u characters",
	   fileName, curline, line.length());
      return(0);
    }
    strcpy(statementSpace,line.c_str());
    statement = &statementSpace[0];
    deepdebugf("converted statement = %s", statement);
    lineno = curline++;
    return(parseStatement(statementType,lineno,statement));
  } else {
    // EOF. Close the file.
    file->close();
    delete file;
    file = 0;
    statementType = svgstatement_eof;
    lineno = curline;
    statement = "";
    return(1);
  }
}

unsigned int
SvgReader::getLine() const {
  return(curline);
}
  
const char*
SvgReader::getFileName() const {
  return(fileName);
}

bool
SvgReader::parseStatement(enum SvgStatement& statementType,
			  unsigned int lineno,
			  const char* statement) const {
  if (*statement != '<') {
    errf("Error parsing SVG: invalid line %s:%u that does not begin a directive", fileName, lineno);
    return(0);
  }
  
  for (unsigned int i = 0; i < nStatementEntries; i++) {
    if (strncmp(statementEntries[i].beginsWith,
		statement,
		statementEntries[i].length) == 0) {
      statementType = statementEntries[i].type;
      return(1);
    }
  }
  
  errf("Error parsing SVG: unrecognised directive in line %s:%u", fileName, lineno);
  return(0);
}

bool
SvgReader::getIntComponent(const char* statement,
			   const char* field,
			   unsigned int& value) {
  assert(statement != 0);
  assert(field != 0);
  const unsigned int fieldn = strlen(field);
  while (*statement != 0) {
    if (strncmp(statement,field,fieldn) == 0 &&
	statement[fieldn + 0] == '=' &&
	statement[fieldn + 1] == '"' &&
	isdigit(statement[fieldn + 2])) {
      deepdeepdebugf("looking at %s", statement);
      char* endPtr = 0;
      long longValue = strtol(statement+fieldn+2,&endPtr,10);
      deepdeepdebugf("endPtr = %s", endPtr);
      if (endPtr == statement+fieldn+2 || *endPtr != '"' || longValue < 0) return(0);
      value = (unsigned int)longValue;
      return(1);
    }
    statement++;
  }
  return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Option parser //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
SvgReader::iterateStatementOptionsInit(const char* statement,
				       const char* fileName,
				       const unsigned int lineNo,
				       struct SvgReaderOptionParser& iter) {
  // Initialize
  iter.ptr = statement;
  iter.fileName = fileName;
  iter.lineNo = lineNo;
  
  // Skip whitespace
  while (*(iter.ptr) != 0 && isspace(*(iter.ptr))) iter.ptr++;

  // Do we have a '<' in front?
  if (*(iter.ptr) != '<') {
    errf("SVG statement does not begin with a '<' in %s:%u", iter.fileName, iter.lineNo);
    return(0);
  }
  iter.ptr++;

  // Skip the directive name
  while (*(iter.ptr) != 0 && isalpha(*(iter.ptr))) iter.ptr++;

  // We're done for now, for the initialization of the iterator
  return(1);
  
}

bool
SvgReader::iterateStatementOptionsNext(struct SvgReaderOptionParser& iter,
				       bool& end,
				       const char*& optionName,
				       unsigned int& optionNameLength,
				       const char*& optionValue,
				       unsigned int& optionValueLength) {
  
  // We have either possible option, or the end of the directive
  
  // Skip whitespace
  while (*(iter.ptr) != 0 && isspace(*(iter.ptr))) iter.ptr++;

  // Test for premature end
  if (*(iter.ptr) == 0) {
    errf("SVG statement does not begin with a '>' in %s:%u", iter.fileName, iter.lineNo);
    return(0);
  }

  // Test for ending the first part of the directive, e.g., as in
  // <text x="1" y="2">Some text</text>.
  if (*(iter.ptr) == '>') {
    iter.ptr++;
    optionName = iter.ptr;
    optionNameLength = 0;
    optionValue = iter.ptr;
    optionValueLength = 0;
    while (*(iter.ptr) != 0 && (*(iter.ptr) != '<' || *(iter.ptr + 1) != '/')) {
      iter.ptr++;
      optionValueLength++;
    }
    if (*(iter.ptr) == 0) {
      errf("Premature end of two-part SVG statement in %s:%u", iter.fileName, iter.lineNo);
      return(0);
    }
    end = 0;
    return(1);
  }
  
  // Test for final end of the directive, as in ...</text>.
  if (*(iter.ptr) != 0 && (*(iter.ptr) == '<' && *(iter.ptr + 1) == '/')) {
    end = 1;
    return(1);
  }
  
  // Test for end of the directive
  if (*(iter.ptr) == '/') {
    // This is the full end of the directive.
    iter.ptr++;
    if (*(iter.ptr) == '>') {
      end = 1;
      return(1);
    } else {
      errf("Slash appeared before the end in SVG directive in %s:%u", iter.fileName, iter.lineNo);
      return(0);
    }
  }
  
  // Do we have a directive?
  if (!isalpha(*(iter.ptr))) {
    errf("SVG statement option does not begin with an alphabetic character in %s:%u: %s", iter.fileName, iter.lineNo, iter.ptr);
    return(0);
  }
  
  // We have a directive.
  optionName = iter.ptr;
  optionNameLength = 1;
  iter.ptr++;
  
  // Find the '='
  while (*(iter.ptr) != 0 && outlinerisalphaordigitordash(*(iter.ptr))) {
    optionNameLength++;
    iter.ptr++;
  }
  if (*(iter.ptr) != '=') {
    errf("Missing = in SVG option in %s:%u", iter.fileName, iter.lineNo);
    return(0);
  }
  iter.ptr++;

  // Now take the value
  if (*(iter.ptr) != '"') {
    errf("Missing opening quote in SVG option value in %s:%u", iter.fileName, iter.lineNo);
    return(0);
  }
  iter.ptr++;
  optionValue = iter.ptr;
  optionValueLength = 1;
  iter.ptr++;
  while (*(iter.ptr) != 0 && *(iter.ptr) != '"') {
    optionValueLength++;
    iter.ptr++;
  }
  if (*(iter.ptr) != '"') {
    errf("Missing closing quote in SVG option value in %s:%u", iter.fileName, iter.lineNo);
    return(0);
  }
  iter.ptr++;
  
  // Done, we have found an option and a value!
  end = 0;
  return(1);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Unit tests //////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
SvgReader::test(void) {
  
  infof("  running SvgReader unit tests");
  {
    unsigned int value = 0;
    bool ans = getIntComponent("","width",value);
    assert(!ans);
    ans = getIntComponent("width=17","width",value);
    assert(!ans);
    ans = getIntComponent("width=\"17","width",value);
    assert(!ans);
    ans = getIntComponent("width=\"-17\"","width",value);
    assert(!ans);
    ans = getIntComponent("width=\"17\"","width",value);
    assert(ans);
    assert(value == 17);
    SvgReader reader1("test/cave1-line.svg");
    unsigned int width;
    unsigned int height;
    ans = reader1.getSize(width,height);
    assert(ans);
    assert(width == 235);
    assert(height == 74);
  }
    
  infof("Running SvgReader option parsing tests");
  {
    const char* statement = "<polyline points=\"13,61 14,61 14,62 \" fill=\"none\" stroke=\"black\" stroke-width=\"1\" />";
    struct SvgReaderOptionParser iter;
    bool ans = iterateStatementOptionsInit(statement,"sample.svg",20,iter);
    assert(ans);
    bool end;
    const char* optionName = 0;
    unsigned int optionNameLength = 0;
    const char* optionValue = 0;
    unsigned int optionValueLength = 0;
    ans = iterateStatementOptionsNext(iter,end,optionName,optionNameLength,optionValue,optionValueLength);
    assert(ans);
    assert(end == 0);
    assert(optionNameLength == 6);
    assert(strncmp(optionName,"points",optionNameLength) == 0);
    assert(optionValueLength == 18);
    assert(strncmp(optionValue,"13,61 14,61 14,62 ",optionValueLength) == 0);
    ans = iterateStatementOptionsNext(iter,end,optionName,optionNameLength,optionValue,optionValueLength);
    assert(ans);
    assert(end == 0);
    assert(optionNameLength == 4);
    assert(strncmp(optionName,"fill",optionNameLength) == 0);
    assert(optionValueLength == 4);
    assert(strncmp(optionValue,"none",optionValueLength) == 0);
    ans = iterateStatementOptionsNext(iter,end,optionName,optionNameLength,optionValue,optionValueLength);
    assert(ans);
    assert(end == 0);
    assert(optionNameLength == 6);
    assert(strncmp(optionName,"stroke",optionNameLength) == 0);
    assert(optionValueLength == 5);
    assert(strncmp(optionValue,"black",optionValueLength) == 0);
    ans = iterateStatementOptionsNext(iter,end,optionName,optionNameLength,optionValue,optionValueLength);
    assert(ans);
    assert(end == 0);
    assert(optionNameLength == 12);
    assert(strncmp(optionName,"stroke-width",optionNameLength) == 0);
    assert(optionValueLength == 1);
    assert(strncmp(optionValue,"1",optionValueLength) == 0);
    ans = iterateStatementOptionsNext(iter,end,optionName,optionNameLength,optionValue,optionValueLength);
    assert(ans);
    assert(end == 1);
  }
  
  infof("Running SvgReader option parsing tests for two-part directives");
  {
    const char* statement = "<text x=\"1\" y=\"2\">Example</text>";
    struct SvgReaderOptionParser iter;
    bool ans = iterateStatementOptionsInit(statement,"sample.svg",30,iter);
    assert(ans);
    bool end;
    const char* optionName = 0;
    unsigned int optionNameLength = 0;
    const char* optionValue = 0;
    unsigned int optionValueLength = 0;
    ans = iterateStatementOptionsNext(iter,end,optionName,optionNameLength,optionValue,optionValueLength);
    assert(ans);
    assert(end == 0);
    assert(optionNameLength == 1);
    assert(strncmp(optionName,"x",optionNameLength) == 0);
    assert(optionValueLength == 1);
    assert(strncmp(optionValue,"1",optionValueLength) == 0);
    ans = iterateStatementOptionsNext(iter,end,optionName,optionNameLength,optionValue,optionValueLength);
    assert(ans);
    assert(end == 0);
    assert(optionNameLength == 1);
    assert(strncmp(optionName,"y",optionNameLength) == 0);
    assert(optionValueLength == 1);
    assert(strncmp(optionValue,"2",optionValueLength) == 0);
    ans = iterateStatementOptionsNext(iter,end,optionName,optionNameLength,optionValue,optionValueLength);
    assert(ans);
    assert(end == 0);
    debugf("returned %s=%s (%u)", optionName, optionValue, optionValueLength);
    assert(optionNameLength == 0);
    assert(optionValueLength == 7);
    assert(strncmp(optionValue,"Example",optionValueLength) == 0);
    ans = iterateStatementOptionsNext(iter,end,optionName,optionNameLength,optionValue,optionValueLength);
    assert(ans);
    assert(end == 1);
  }
}

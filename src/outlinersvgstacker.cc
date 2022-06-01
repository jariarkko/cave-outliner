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
#include "outlinersvgreader.hh"
#include "outlinersvgstacker.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Data types /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

struct svgcreator_composite_inputentry {
  const char* imageFileName;
  unsigned int xSize;
  unsigned int ySize;
  unsigned int yOffset;
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Construction (image composition) ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

SvgStacker::SvgStacker(const char* fileNameIn,
		       const unsigned int nImageFileNames,
		       const char** imageFileNames,
		       const SvgOptions& optionsIn,
		       const unsigned int verticalSpacing) :
  fileName(fileNameIn),
  options(optionsIn),
  good(0) {
  
  // Debugs
  assert(nImageFileNames > 0);

  // Calculate resulting image size
  unsigned int compositeXSize = 0;
  unsigned int compositeYSize = 0;
  unsigned int yOffset = 0;
  struct svgcreator_composite_inputentry* entries = new struct svgcreator_composite_inputentry[nImageFileNames];
  if (entries == 0) {
    errf("Cannot allocate %u composite input files", nImageFileNames);
    return;
  }
  for (unsigned int i = 0; i < nImageFileNames; i++) {
    struct svgcreator_composite_inputentry& entry = entries[i];
    entry.imageFileName = imageFileNames[i];
    entry.yOffset = yOffset;
    SvgReader reader(entry.imageFileName);
    if (!reader.getSize(entry.xSize,entry.ySize)) {
      errf("Cannot read SVG file %s", entry.imageFileName);
      return;
    }
    if (entry.xSize > compositeXSize) compositeXSize = entry.xSize;
    unsigned int thisSize = entry.ySize + verticalSpacing;
    compositeYSize += thisSize;
    yOffset += thisSize;
  }
  
  // Open the output file
  file.open(fileName);
  if (file.good()) {
  
    SvgCreator::preamble(file,compositeXSize,compositeYSize,options,0);
    
    // Add each image to the output
    for (unsigned int i = 0; i < nImageFileNames; i++) {
      struct svgcreator_composite_inputentry& entry = entries[i];
      SvgReader reader(entry.imageFileName);
      if (!importSvgContents(reader,entry.yOffset)) {
	errf("Cannot import SVG file %s", entry.imageFileName);
	return;
      }
    }
    
    // Finalize the file
    SvgCreator::postamble(file);

    // Final check that everything is ok. If we don't get here, the ok
    // flag is already set to 0 upon initialization. This is the only
    // place where it can be set to 1.
    good = 1;
  }
  
}

SvgStacker::~SvgStacker() {
  debugf("SvgStacker::~SvgStacker start");
  unsigned long bytes = file.tellp();
  file.close();
  infof("  Image statistics");
  infof("    File %s size %.1f KB", fileName, (bytes / 1000.0));
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Testing for success ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
SvgStacker::ok() {
  if (!good) {
    errf("Unable to write SVG file due to an earlier processing error");
    return(0);
  }
  if (file.fail()) {
    errf("Unable to write SVG file due to a logical I/O error");
    return(0);
  }
  if (file.bad()) {
    errf("Unable to write SVG file due to a read/write I/O error");
    return(0);
  }
  return(1);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Auxiliary routines /////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
SvgStacker::importSvgContents(SvgReader& reader,
			      unsigned int yOffset) {
  enum SvgStatement statementType;
  unsigned int lineno;
  const char* statement = 0;
  while (reader.getStatement(statementType,lineno,statement)) {
    
    switch (statementType) {
      
    case svgstatement_eof:
      // Done
      return(1);

    case svgstatement_xml:
    case svgstatement_doctype:
    case svgstatement_svgstart:
    case svgstatement_svgend:
      // Ignore
      continue;
      
    case svgstatement_rect:
      if (!addYOffsetRectangle(reader,statement,yOffset)) {
	errf("SVG processing failed for %s", statement);
	return(0);
      }
      continue;
      
    case svgstatement_line:
      if (!addYOffsetLine(reader,statement,yOffset)) {
	errf("SVG processing failed for %s", statement);
	return(0);
      }
      continue;
      
    case svgstatement_polyline:
      if (!addYOffsetPolyline(reader,statement,yOffset)) {
	errf("SVG processing failed for %s", statement);
	return(0);
      }
      continue;
      
    case svgstatement_text:
      if (!addYOffsetText(reader,statement,yOffset)) {
	errf("SVG processing failed for %s", statement);
	return(0);
      }
      continue;
      
    default:
      errf("Invalid svgstatement enum type");
      return(0);
    }
  }
  
  // Statement acquisition failed in some other error than EOF. Fail.
  errf("Scanning SVG statements failed");
  return(0);
}

bool
SvgStacker::addYOffsetRectangle(SvgReader& reader,
				const char* statement,
				unsigned int yOffset) {
  file << "<rect ";
  SvgReaderOptionParser optionParser;
  if (!reader.iterateStatementOptionsInit(statement,
					  reader.getFileName(),
					  reader.getLine(),
					  optionParser)) {
    errf("SVG rect options scanning start failed for %s", statement);
    return(0);
  } 
  for (;;) {
    bool end;
    const char* optionName;
    unsigned int optionNameLength;
    const char* optionValue;
    unsigned int optionValueLength;
    if (!reader.iterateStatementOptionsNext(optionParser,
					    end,
					    optionName,optionNameLength,
					    optionValue,optionValueLength)) {
      errf("SVG rect options scanning next operation failed for %s", statement);
      return(0);
    } 
    if (end) break;
    if (optionNameLength == 1 && strncmp(optionName,"y",optionNameLength) == 0) {
      // Modify the value
      outputCopiedText(optionName,optionNameLength);
      file << "=\"";
      long oldValue = strtol(optionValue,0,10);
      long newValue = oldValue + yOffset;
      file << newValue;
      file << "\" ";
    } else if (optionNameLength == 0) {
      errf("Body not allowed in SVG rect statement");
      return(0);
    } else {
      // Just copy the option
      outputCopiedText(optionName,optionNameLength);
      file << "=\"";
      outputCopiedText(optionValue,optionValueLength);
      file << "\" ";
    }
  }
  file << " />\n";
  return(1);
}

bool
SvgStacker::addYOffsetLine(SvgReader& reader,
			   const char* statement,
			   unsigned int yOffset) {
  file << "<line ";
  SvgReaderOptionParser optionParser;
  if (!reader.iterateStatementOptionsInit(statement,
					  reader.getFileName(),
					  reader.getLine(),
					  optionParser)) {
    errf("SVG line options scanning start failed for %s", statement);
    return(0);
  } 
  for (;;) {
    bool end;
    const char* optionName;
    unsigned int optionNameLength;
    const char* optionValue;
    unsigned int optionValueLength;
    if (!reader.iterateStatementOptionsNext(optionParser,
					    end,
					    optionName,optionNameLength,
					    optionValue,optionValueLength)) {
      errf("SVG line options scanning next operation failed for %s", statement);
      return(0);
    } 
    if (end) break;
    if (optionNameLength == 2 &&
	(strncmp(optionName,"y1",optionNameLength) == 0 ||
	 strncmp(optionName,"y2",optionNameLength) == 0)) {
      // Modify the value
      outputCopiedText(optionName,optionNameLength);
      file << "=\"";
      long oldValue = strtol(optionValue,0,10);
      long newValue = oldValue + yOffset;
      file << newValue;
      file << "\" ";
    } else if (optionNameLength == 0) {
      errf("Body not allowed in SVG line statement");
      return(0);
    } else {
      // Just copy the option
      outputCopiedText(optionName,optionNameLength);
      file << "=\"";
      outputCopiedText(optionValue,optionValueLength);
      file << "\" ";
    }
  }
  file << " />\n";
  return(1);
}

bool
SvgStacker::addYOffsetPolyline(SvgReader& reader,
			       const char* statement,
			       unsigned int yOffset) {
# define polylinepoints_maxlen (10*1024)
# define polylinepoints_maxlen_extra (2048*1)
  file << "<polyline ";
  SvgReaderOptionParser optionParser;
  if (!reader.iterateStatementOptionsInit(statement,
					  reader.getFileName(),
					  reader.getLine(),
					  optionParser)) {
    errf("SVG polyline options scanning start failed for %s", statement);
    return(0);
  } 
  for (;;) {
    bool end;
    const char* optionName;
    unsigned int optionNameLength;
    const char* optionValue;
    unsigned int optionValueLength;
    if (!reader.iterateStatementOptionsNext(optionParser,
					    end,
					    optionName,optionNameLength,
					    optionValue,optionValueLength)) {
      errf("SVG polyline options scanning next operation failed for %s", statement);
      return(0);
    } 
    if (end) break;
    if (optionNameLength == 6 && strncmp(optionName,"points",optionNameLength) == 0) {
      // Modify the value
      outputCopiedText(optionName,optionNameLength);
      file << "=\"";
      if (optionValueLength > polylinepoints_maxlen) {
	errf("SVG polyline too long (%u vs. %u)", optionValueLength, polylinepoints_maxlen);
	return(0);
      }
      char newValue[polylinepoints_maxlen+polylinepoints_maxlen_extra];
      memset(newValue,0,sizeof(newValue));
      if (!pointsStringAddYOffset(optionValue,optionValueLength,
				  yOffset,
				  newValue,sizeof(newValue)-1,
				  reader.getFileName(),
				  reader.getLine())) {
	errf("SVG polyline offset operation failed");
	return(0);
      }
      file << newValue;
      file << "\" ";
    } else if (optionNameLength == 0) {
      errf("Body not allowed in SVG polyline statement");
      return(0);
    } else {
      // Just copy the option
      outputCopiedText(optionName,optionNameLength);
      file << "=\"";
      outputCopiedText(optionValue,optionValueLength);
      file << "\" ";
    }
  }
  file << " />\n";
  return(1);
}

bool
SvgStacker::addYOffsetText(SvgReader& reader,
			   const char* statement,
			   unsigned int yOffset) {
  file << "<text ";
  SvgReaderOptionParser optionParser;
  if (!reader.iterateStatementOptionsInit(statement,
					  reader.getFileName(),
					  reader.getLine(),
					  optionParser)) {
    errf("SVG text options scanning start failed for %s", statement);
    return(0);
  }
  bool seenBody = 0;
  for (;;) {
    bool end;
    const char* optionName;
    unsigned int optionNameLength;
    const char* optionValue;
    unsigned int optionValueLength;
    if (!reader.iterateStatementOptionsNext(optionParser,
					    end,
					    optionName,optionNameLength,
					    optionValue,optionValueLength)) {
      errf("SVG text options scanning next operation failed for %s", statement);
      return(0);
    } 
    if (end) break;
    if (optionNameLength == 1 && strncmp(optionName,"y",optionNameLength) == 0) {
      // Modify the value
      outputCopiedText(optionName,optionNameLength);
      file << "=\"";
      long oldValue = strtol(optionValue,0,10);
      long newValue = oldValue + yOffset;
      file << newValue;
      file << "\" ";
    } else if (optionNameLength == 0) {
      // Body, copy it as is
      seenBody = 1;
      file << ">";
      outputCopiedText(optionValue,optionValueLength);
    } else {
      // Just copy the option
      outputCopiedText(optionName,optionNameLength);
      file << "=\"";
      outputCopiedText(optionValue,optionValueLength);
      file << "\" ";
    }
  }
  if (seenBody) {
    file << "</text>\n";
  } else {
    file << " />\n";
  }
  return(1);
}

bool
SvgStacker::pointsStringAddYOffset(const char* optionValue,
				   unsigned int optionValueLength,
				   unsigned int yOffset,
				   char* newValue,
				   unsigned int newValueSize,
				   const char* fileNameRead,
				   const unsigned int lineNoRead) {
  for (;;) {

    // Debugs
    deepdebugf("SVG polyline points looking at '%s' (%u chars)", optionValue, optionValueLength);
    
    // Check there's buffer left
    if (newValueSize == 0) {
      errf("SVG polyline processing encountered a too long string for the points attribute in %s:u", fileNameRead, lineNoRead);
      return(0);
    }

    // Check if we're at the end
    if (optionValueLength == 0) {
      *(newValue++) = 0;
      deepdebugf("Successful processing, returning 1");
      return(1);
    }

    // Check what character we are looking at
    switch (*optionValue) {
      
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case ' ':
    case '"':
      // Keep copying
      *(newValue++) = (*optionValue++);
      optionValueLength--;
      newValueSize--;
      continue;

    case ',':
      // Now we see a Y value in front of us. Need to change it.
      {
	
	// First move the comma to the result.
	*(newValue++) = (*optionValue++);
	optionValueLength--;
	newValueSize--;
	if (optionValueLength == 0) {
	  errf("SVG polyline syntax error for the points attribute after comma in %s:%u: option value ends", fileNameRead, lineNoRead);
	  return(0);
	}
	if (newValueSize == 0) {
	  errf("SVG polyline processing for the points attribute runs out of space to store modified value in %s:%u", fileNameRead, lineNoRead);
	  return(0);
	}
	if (!isdigit(*optionValue)) {
	  errf("SVG polyline syntax error for the points attribute after comma in %s:%u: %c is not a digit - %s",
	       fileNameRead, lineNoRead,
	       *optionValue,
	       optionValue);
	  return(0);
	}

	// Read the number
	unsigned int currentValue = 0;
	while (optionValueLength > 0 && isdigit(*optionValue)) {
	  currentValue *= 10;
	  char thisChar = (*(optionValue++)) - '0';
	  unsigned int thisCharValue = ((unsigned int)(unsigned char)thisChar) & 0xFF;
	  assert(thisCharValue <= 9);
	  currentValue += thisCharValue;
	  optionValueLength--;
	  newValueSize--;
	}

	// Add the offset and store the number
	unsigned int newIntValue = currentValue + yOffset;
	char newIntValueBuf[20];
	memset(newIntValueBuf,0,sizeof(newIntValueBuf));
	snprintf(newIntValueBuf,sizeof(newIntValueBuf)-1,"%u",newIntValue);
	unsigned int newIntValueLength = strlen(newIntValueBuf);
	if (newValueSize < newIntValueLength + 1) {
	  errf("SVG polyline processing for the points attribute out of space in %s:%u", fileNameRead, lineNoRead);
	  return(0);
	}
	strcpy(newValue,newIntValueBuf);
	newValue += newIntValueLength;
	newValueSize -= newIntValueLength;

	// Check that the next thing is a space or a quote
	if (*optionValue != ' ' && *optionValue != '"') {
	  errf("SVG polyline syntax error for the points attribute after x,y entry in %s:%u", fileNameRead, lineNoRead);
	  return(0);
	}
	
	// Done
	continue;
      }
      
    default:
      errf("SVG polyline syntax error for the points attribute in %s:%u: %s", fileNameRead, lineNoRead, optionValue);
      return(0);
    }
  }

  fatalf("Should not get here");
  return(0);
}

void
SvgStacker::outputCopiedText(const char* ptr,
			     unsigned int n) {
  for (unsigned int i = 0; i < n; i++) {
    file << ptr[i];
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Unit tests /////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
SvgStacker::test(void) {

  // Simple polyline attribute test
  {
    const char* optionValue = "1,2 ";
    char buf[100];
    memset(buf,0,sizeof(buf));
    bool ans = pointsStringAddYOffset(optionValue,strlen(optionValue),
				      10,
				      buf,
				      sizeof(buf)-1,
				      "sample.svg",
				      10);
    assert(ans);
    debugf("result = '%s'", buf);
    const char* expected = "1,12 ";
    assert(strlen(buf) == strlen(expected));
    assert(strcmp(buf,expected) == 0);
  }
  
  // More complex test
  {
    const char* optionValue = "1,2 30,40 500,600 ";
    char buf[100];
    memset(buf,0,sizeof(buf));
    bool ans = pointsStringAddYOffset(optionValue,strlen(optionValue),
				      1000,
				      buf,
				      sizeof(buf)-1,
				      "sample.svg",
				      20);
    assert(ans);
    const char* expected = "1,1002 30,1040 500,1600 ";
    debugf("expect = '%s'", expected);
    debugf("result = '%s'", buf);
    assert(strlen(buf) == strlen(expected));
    assert(strcmp(buf,expected) == 0);
  }
}


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

#ifndef MAINOPTIONS_HH
#define MAINOPTIONS_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include "mainconfig.hh"
#include "outlinertemp.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// The MainOptions class represents the functionalit to process all end-user configuration
/// information, from the command line. It updates the MainConfig object as it processes.
///

class MainOptions {

public:

  /// Create an options processor.
  MainOptions(MainConfig& configIn,
	      TempFiler& tempFiler);

  /// Release all resources associated with the options processor.
  ~MainOptions();

  /// Process options and arguments. Return 1 if successful, 0 upon
  /// error.
  bool processCommandLine(int argc, char** argv);
  
private:
  
  MainConfig& config;
  TempFiler& tempFiler;
  
  bool processCommandLineOptions(int& argc,
                                 char**& argv);
  bool processCommandLineArguments(int& argc,
                                   char**& argv);
  bool processDefaults(void);
  void processVersion(void);
  void processHelp(void);
  bool checkFileExtension(const char* filename,
                          const char* extension);
  bool parseDirection(const char* string,
                      enum outlinerdirection& direction);
  bool getFileCreation(const char* file,
		       time_t* p_result) const;
};

#endif // MAINCONFIG_HH


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
#include <sys/stat.h>
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"
#include "outlinerhighprecision.hh"
#include "outlinerprocessor.hh"
#include "outlinerversion.hh"
#include "outlinerdebug.hh"
#include "mainconfig.hh"
#include "mainoptions.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class functions ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

MainOptions::MainOptions(MainConfig& configIn,
			 TempFiler& tempFilerIn) :
  config(configIn),
  tempFiler(tempFilerIn) {
}

MainOptions::~MainOptions() {
  debugf("MainOptions::~MainOptions");
}

bool
MainOptions::processCommandLine(int argc,
                                char** argv) {
  if (!processCommandLineOptions(argc,argv)) {
    return(0);
  } else if (!processCommandLineArguments(argc,argv)) {
    return(0);
  } else {
    processDefaults();
    return(1);
  }
}

bool
MainOptions::processCommandLineOptions(int& argc,
                                       char**& argv) {
  while (argc > 1 && argv[1][0] == '-') {
    if (!processCommandLineOption(argc,argv)) {
      return(0);
    }   
    argc--;
    argv++;
  }

  return(1);
}

bool
MainOptions::processCommandLineOption(int& argc,
                                      char**& argv) {
  assert(argc > 1);
  assert(argv[1][0] == '-');
  
  if (strcmp(argv[1],"-") == 0 ||
      strcmp(argv[1],"--") == 0) {
    // Moving right along
  } else if (strcmp(argv[1],"--quiet") == 0) {
    config.info = 0;
  } else if (strcmp(argv[1],"--debug") == 0) {
    config.debug = 1;
  } else if (strcmp(argv[1],"--deepdebug") == 0) {
    config.debug = 1;
    config.deepdebug = 1;
  } else if (strcmp(argv[1],"--deepdeepdebug") == 0) {
    config.debug = 1;
    config.deepdebug = 1;
    config.deepdeepdebug = 1;
  } else if (strcmp(argv[1],"--z") == 0) {
    config.direction = dir_z;
  } else if (strcmp(argv[1],"--x") == 0) {
    config.direction = dir_x;
  } else if (strcmp(argv[1],"--y") == 0) {
    config.direction = dir_y;
  } else if (strcmp(argv[1],"--pixel") == 0) {
    config.algorithm = alg_pixel;
  } else if (strcmp(argv[1],"--pixelform") == 0) {
    if (!config.formAnalysis) {
      errf("The use of --pixelform requires a preceding --formanalysis option");
      return(0);
    }
    config.algorithm = alg_pixelform;
  } else if (strcmp(argv[1],"--triangle") == 0) {
    config.algorithm = alg_triangle;
  } else if (strcmp(argv[1],"--depthmap") == 0) {
    config.algorithm = alg_depthmap;
  } else if (strcmp(argv[1],"--depthdiffmap") == 0) {
    config.algorithm = alg_depthdiffmap;
  } else if (strcmp(argv[1],"--borderpixel") == 0) {
    config.algorithm = alg_borderpixel;
  } else if (strcmp(argv[1],"--borderline") == 0) {
    config.algorithm = alg_borderline;
  } else if (strcmp(argv[1],"--borderactual") == 0) {
    config.algorithm = alg_borderactual;
  } else if (strcmp(argv[1],"--crosssections") == 0 && argc > 3) {
    if (config.nCrossSections == outlinermaxcrosssections) {
      errf("Maximum number of cross sections (%u) reached", outlinermaxcrosssections);
      return(0);
    }
    enum outlinerdirection linedirection = dir_x;
    if (!parseDirection(argv[2],linedirection))  {
      return(0);
    }
    int num = atoi(argv[3]);
    if (num < 1) {
      errf("A number of cross sections must be 1 or greater, %s given", argv[3]);
      return(0);
    }
    const char* filenamePattern = 0;
    if (argc > 4 && argv[4][0] != '-') {
      filenamePattern = strdup(argv[4]);
      if (filenamePattern == 0) {
	errf("Cannot allocate file name pattern string");
	exit(1);
      }
      argc--;argv++;
      argc--;argv++;
      argc--;argv++;
    } else {
      filenamePattern = tempFiler.createTempFile("auto-cross-section-%","svg");
      argc--;argv++;
      argc--;argv++;
    }
    if (strchr(filenamePattern,'%') == 0) {
      errf("File name pattern should include the %% sign, %s given", filenamePattern);
      return(0);
    }
    config.automaticCrossSections = 1;
    config.automaticCrossSectionsDirection = linedirection;
    config.nAutomaticCrossSections = num;
    config.automaticCrossSectionFilenamePattern = filenamePattern;
  } else if (strcmp(argv[1],"--crosssection") == 0 && argc > 3) {
    if (config.nCrossSections == outlinermaxcrosssections) {
      errf("Maximum number of cross sections (%u) reached", outlinermaxcrosssections);
      return(0);
    }
    enum outlinerdirection linedirection = dir_x;
    if (!parseDirection(argv[2],linedirection))  {
      return(0);
    }
    if (!isdigit(argv[3][0]) && argv[3][0] != '+' && argv[3][0] != '-') {
      errf("Cross section coordinate value needs to be a number, %s given", argv[3]);
      return(0);
    }
    float num = atof(argv[3]);
    const char* file = 0;
    if (argc > 4 && argv[4][0] != '-') {
      file = strdup(argv[4]);
      if (file == 0) {
	fatalf("Cannot allocate file name string for %s", argv[4]);
	return(0);
      }
      argc--;argv++;
      argc--;argv++;
      argc--;argv++;
    } else {
      file = tempFiler.createTempFile("cross-section-#-%","svg");
      argc--;argv++;
      argc--;argv++;
    }
    if (strlen(file) < 1) {
      errf("Cross section file name cannot be empty, %s given", file);
      return(0);
    }
    config.crossSectionDirections[config.nCrossSections] = linedirection;
    config.crossSectionPoints[config.nCrossSections] = num;
    debugf("  cross section point %.2f", num);
    config.crossSections[config.nCrossSections].line.start.x = 0;
    config.crossSections[config.nCrossSections].line.start.y = 0;
    config.crossSections[config.nCrossSections].line.end.x = 0;
    config.crossSections[config.nCrossSections].line.end.y = 0;
    config.crossSections[config.nCrossSections].filename = file;
    config.crossSections[config.nCrossSections].width = config.crossSectionWidth;
    config.crossSections[config.nCrossSections].label = config.getCrossSectionLabel();
    config.nCrossSections++;
  } else if (strcmp(argv[1],"--crosssectionwidth") == 0 && argc > 2) {
    float num = atof(argv[2]);
    if (num < 0.0) {
      errf("Cross section width value needs to be a non-negative number, %s given", argv[2]);
      return(0);
    }
    config.crossSectionWidth = num;
    argc--;argv++;
  } else if (strcmp(argv[1],"--formanalysis") == 0 && argc > 2) {
    int num = atoi(argv[2]);
    if (num < 1) {
      errf("Form compression value needs to be larger than or equal to 1, %s given", argv[2]);
      return(0);
    }
    config.formAnalysis = 1;
    config.formCondense = num;
    argc--;argv++;
  } else if (strcmp(argv[1],"--composite") == 0) {
    config.compositeMap = 1;
  } else if (strcmp(argv[1],"--name") == 0 && argc > 2) {
    config.name = argv[2];
    argc--;argv++;
  } else if (strcmp(argv[1],"--surveyer") == 0 && argc > 2) {
    config.surveyer = argv[2];
    argc--;argv++;
  } else if (strcmp(argv[1],"--surveytool") == 0 && argc > 2) {
    config.surveyTool = argv[2];
    argc--;argv++;
  } else if (strcmp(argv[1],"--surveydate") == 0 && argc > 2) {
    config.surveyDate = argv[2];
    argc--;argv++;
  } else if (strcmp(argv[1],"--mapdate") == 0 && argc > 2) {
    config.mapDate = argv[2];
    argc--;argv++;
  } else if (strcmp(argv[1],"--location") == 0 && argc > 2) {
    config.location = argv[2];
    argc--;argv++;
  } else if (strcmp(argv[1],"--coordinates") == 0 && argc > 2) {
    config.coordinates = argv[2];
    argc--;argv++;
  } else if (strcmp(argv[1],"--floordepthmap") == 0 && argc > 2) {
    config.floorDepthMap = argv[2];
    argc--;argv++;
  } else if (strcmp(argv[1],"--roofdepthmap") == 0 && argc > 2) {
    config.roofDepthMap = argv[2];
    argc--;argv++;
  } else if (strcmp(argv[1],"--floorstyle") == 0 && argc > 2) {
    if (strcmp(argv[2],"depth") == 0) {
      config.floorStyleDiff = 0;
    } else if (strcmp(argv[2],"diff") == 0) {
      config.floorStyleDiff = 1;
    } else {
      errf("Invalid floor style %s, expected either depth or diff", argv[2]);
      return(0);
    }
    argc--;argv++;
  } else if (strcmp(argv[1],"--tunnelspine") == 0) {
    config.tunnelSpine = 1;
  } else if (strcmp(argv[1],"--label") == 0) {
    config.labelCrossSections = 1;
  } else if (strcmp(argv[1],"--dimensions") == 0) {
    config.dimensions = 1;
  } else if (strcmp(argv[1],"--linewidth") == 0 && argc > 2) {
    float num = atof(argv[2]);
    if (num <= 0.0) {
      errf("Line width value needs to be a positive number, %s given", argv[2]);
      return(0);
    }
    config.linewidth = num;
    argc--;argv++;
  } else if (strcmp(argv[1],"--multiplier") == 0 && argc > 2) {
    int num = atoi(argv[2]);
    if (num <= 0) {
      errf("Multiplier must be positive, %s given", argv[2]);
      return(0);
    }
    config.multiplier = num;
    argc--;argv++;
  } else if (strcmp(argv[1],"--smooth") == 0) {
    config.smooth = 1;
  } else if (strcmp(argv[1],"--jagged") == 0) {
    config.smooth = 0;
  } else if (strcmp(argv[1],"--svgyreverse") == 0) {
    config.svgYSwap = 0;
  } else if (strcmp(argv[1],"--holethreshold") == 0 && argc > 2) {
    int num = atoi(argv[2]);
    if (num < 0 || num > outlinermaxholethreshold) {
      errf("Hole threshold value needs to be non-negative and max %u, %s given", outlinermaxholethreshold, argv[2]);
      return(0);
    }
    config.holeThreshold = num;
    argc--;argv++;
  } else if (strcmp(argv[1],"--lineHoleThreshold") == 0 && argc > 2) {
    int num = atoi(argv[2]);
    if (num < 0 || num > 100) {
      errf("Line hole threshold value needs to be non-negative and max 100, %s given", argv[2]);
      return(0);
    }
    config.lineHoleThreshold = num;
    argc--;argv++;
  } else if (strcmp(argv[1],"--dustthreshold") == 0 && argc > 2) {
    int num = atoi(argv[2]);
    if (num < 0 || num > 100) {
      errf("Dust threshold value needs to be non-negative and max 100, %s given", argv[2]);
      return(0);
    }
    config.dustThreshold = num;
    argc--;argv++;
  } else if (strcmp(argv[1],"--step") == 0 && argc > 2) {
    config.stepz = config.stepx = config.stepy = atof(argv[2]);
    if (config.stepx < 0.0001) {
      errf("Invalid step value");
      return(0);
    }
    argc--;argv++;
  } else if (strcmp(argv[1],"--bounding") == 0 && argc > 1 + 2*3) {
    if (!outlinerisnumber(argv[2])) { errf("bounding element #1 is not a number: %s", argv[2]); return(0); }
    if (!outlinerisnumber(argv[3])) { errf("bounding element #2 is not a number: %s", argv[3]); return(0); }
    if (!outlinerisnumber(argv[4])) { errf("bounding element #3 is not a number: %s", argv[4]); return(0); }
    if (!outlinerisnumber(argv[5])) { errf("bounding element #4 is not a number: %s", argv[5]); return(0); }
    if (!outlinerisnumber(argv[6])) { errf("bounding element #5 is not a number: %s", argv[6]); return(0); }
    if (!outlinerisnumber(argv[7])) { errf("bounding element #6 is not a number: %s", argv[7]); return(0); }
    argc--;argv++; float startx = atof(argv[1]);
    argc--;argv++; float endx = atof(argv[1]);
    argc--;argv++; float starty = atof(argv[1]);
    argc--;argv++; float endy = atof(argv[1]);
    argc--;argv++; float startz = atof(argv[1]);
    argc--;argv++; float endz = atof(argv[1]);
    if (endx - startx < 0.0001) {
      errf("Invalid bounding box x range");
      return(0);
    }
    if (endy - starty < 0.0001) {
      errf("Invalid bounding box y range");
      return(0);
    }
    if (endz - startz < 0.0001) {
      errf("Invalid bounding box z range");
      return(0);
    }
    config.boundingBoxSet = 1;
    config.boundingBox.start = OutlinerVector3D(startx,starty,startz);
    config.boundingBox.end = OutlinerVector3D(endx,endy,endz);
  } else if (strcmp(argv[1],"--tiling") == 0 && argc > 2) {
    if (atoi(argv[2]) < 1 || atoi(argv[2]) > 10000) {
      errf("Invalid tile count, must be at least one and a not too big for memory, %s given", argv[2]);
      return(0);
    }
    config.tiles = atoi(argv[2]);
    argc--;argv++;
  } else if (strcmp(argv[1],"--test") == 0) {
    config.test = 1;
  } else if (strcmp(argv[1],"--parallel") == 0 && argc > 2) {
    int num = atoi(argv[2]);
    if (num < 1 || num > 100) {
      errf("Number of threads needs to a positive integer and under 100, %s given", argv[2]);
      return(0);
    }
    config.nThreads = num;
    argc--;argv++;
  } else if (strcmp(argv[1],"--auto") == 0) {
    return(processCommandLineOptionAuto());
  } else if (strcmp(argv[1],"--version") == 0) {
    processVersion();
    return(0);
  } else if (strcmp(argv[1],"--help") == 0) {
    processHelp();
    return(0);
  } else {
    errf("Unrecognised option: %s", argv[1]);
    return(0);
  }

  // Success
  return(1);
}

bool
MainOptions::processCommandLineOptionAuto(void) {
# define maxAutoOptions 20
  int nAutomaticArgs = 0;
  char** automaticArgs = (char**)malloc(maxAutoOptions * sizeof(char*));
  if (automaticArgs == 0) {
    errf("Cannot allocate option table");
    return(0);
  }
# define addAutoOption(s) \
  automaticArgs[nAutomaticArgs++] = strdup(s);  \
  if (automaticArgs[nAutomaticArgs-1] == 0) {   \
    errf("Cannot allocate option string");      \
    return(0);                                  \
  }
# define addAutoOptionArg(s,a)			\
  addAutoOption(s);				\
  addAutoOption(a)
  addAutoOption(""); // dummy entry for the 0th option (which would usually be the program name)
  addAutoOption("--label");
  addAutoOption("--dimensions");
  addAutoOption("--borderline");
  addAutoOptionArg("--linewidth","3");
  addAutoOptionArg("--multiplier","2");
  addAutoOptionArg("--step","0.06");
  addAutoOptionArg("--holethreshold","20");
  addAutoOptionArg("--crosssectionwidth","3");
  addAutoOptionArg("--formanalysis","1");
  addAutoOption("--composite");
  bool ret = processCommandLineOptions(nAutomaticArgs,automaticArgs);
  for (unsigned int i = 0; i < nAutomaticArgs; i++) {
    free(automaticArgs[i]);
  }
  return(ret);
}

bool
MainOptions::processCommandLineArguments(int& argc,
                                         char**& argv) {
  
  // Check input and output file names
  if (argc == 1 && config.test) {
    return(1);
  } else if (argc != 3) {
    errf("Expected two arguments, an input and output file name, got %u", argc-1);
    if (argc > 1) {
      errf("First argument was %s", argv[1]);
    }
    return(0);
  }
  config.inputFile = strdup(argv[1]);
  if (!config.compositeMap) {
    config.planViewOutputFile = strdup(argv[2]);
  } else {
    config.planViewOutputFile = tempFiler.createTempFile("planview","svg");
    config.compositeOutputFile = strdup(argv[2]);
  }
  if (config.inputFile == 0 || config.planViewOutputFile == 0) {
    fatalf("Cannot allocate input/output file names");
    return(0);
  }
  if (outlineralgorithm_generatespicture(config.algorithm)) {
    if (!checkFileExtension(config.planViewOutputFile,"svg")) {
      errf("Output file must be an SVG file, %s given", config.planViewOutputFile);
      return(0);
    }
  }

  return(1);
}

bool
MainOptions::processDefaults(void) {
  if (config.surveyDate == 0 && config.inputFile != 0) {
    time_t fileCreation;
    if (!getFileCreation(config.inputFile,&fileCreation)) {
      warnf("Cannot stat input file %s", config.inputFile);
      return(1);
    }
    const unsigned int dateSize = 30;
    char* buf = (char*)malloc(dateSize);
    if (buf == 0) {
      fatalf("Cannot allocate %u bytes", dateSize);
      return(0);
    }
    config.convertToDate(fileCreation,buf,dateSize);
    config.surveyDate = buf;
  }
  return(1);
}

bool
MainOptions::getFileCreation(const char* file,
			     time_t* p_result) const {
  assert(file != 0);
  assert(p_result != 0);
  struct stat info;
  if (stat(file,&info) < 0) {
    return(0);
  }
#ifdef _DARWIN_FEATURE_64_BIT_INODE
  *p_result = info.st_birthtime;
#else
  *p_result = info.st_mtimespec;
#endif
  return(1);
}
  
///////////////////////////////////////////////////////////////////////////////////////////////
// Version ////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
MainOptions::processVersion(void) {
  std::cout << "\n";
  std::cout << OUTLINER_PROG " version ";
  std::cout << outlinerVersion;
  std::cout << "\n";
  std::cout << "\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Utility functions //////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

bool
MainOptions::checkFileExtension(const char* filename,
                                const char* extension) {
  assert(filename != 0);
  assert(extension != 0);
  const char* foundExtension = rindex(filename,'.');
  if (foundExtension == 0) return(0);
  foundExtension++;
  if (strcasecmp(foundExtension,extension) != 0) return(0);
  else return(1);
}

bool
MainOptions::parseDirection(const char* string,
                            enum outlinerdirection& direction) {
  if (strcmp(string,"x") == 0) {
    direction = dir_x;
    return(1);
  } else if (strcmp(string,"y") == 0) {
    direction = dir_y;
    return(1);
  } else if (strcmp(string,"z") == 0) {
    direction = dir_z;
    return(1);
  } else {
    direction = dir_x;
    errf("Unrecognised line direction %s, expecting x, y, or z", string);
    return(0);
  }
}

  
  

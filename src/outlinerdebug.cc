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

#include <stdarg.h>
#include <cassert>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdebug.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Local variables ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

static bool info = 0;
static bool debug = 0;
static bool deepdebug = 0;
static bool deepdeepdebug = 0;

///////////////////////////////////////////////////////////////////////////////////////////////
// Initialization /////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void
debuginit(bool infoSet,
          bool debugSet,
          bool deepdebugSet,
          bool deepdeepdebugSet) {
  info = infoSet;
  debug = debugSet;
  deepdebug = deepdebugSet;
  deepdeepdebug = deepdeepdebugSet;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Debug and output functions /////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

__attribute__((__format__ (__printf__, 1, 0)))
void
debugf(const char* format, ...) {

  assert(format != 0);

  if (debug) {

    va_list args;
    char buf[500];
    memset(buf,0,sizeof(buf));
    va_start (args, format);
    vsnprintf(buf,sizeof(buf)-1,format,args);
    va_end (args);
    std::cerr << OUTLINER_DEBUGPREFIX;
    std::cerr << buf;
    std::cerr << "\n";
    std::cerr.flush();
    
  }
  
}

__attribute__((__format__ (__printf__, 1, 0)))
void
deepdebugf(const char* format, ...) {

  assert(format != 0);

  if (deepdebug) {

    va_list args;
    char buf[500];
    memset(buf,0,sizeof(buf));
    va_start (args, format);
    vsnprintf(buf,sizeof(buf)-1,format,args);
    va_end (args);
    std::cerr << OUTLINER_DEBUGPREFIX;
    std::cerr << buf;
    std::cerr << "\n";
    std::cerr.flush();
    
  }
  
}

__attribute__((__format__ (__printf__, 1, 0)))
void
deepdeepdebugf(const char* format, ...) {

  assert(format != 0);

  if (deepdeepdebug) {

    va_list args;
    char buf[500];
    memset(buf,0,sizeof(buf));
    va_start (args, format);
    vsnprintf(buf,sizeof(buf)-1,format,args);
    va_end (args);
    std::cerr << OUTLINER_DEBUGPREFIX;
    std::cerr << buf;
    std::cerr << "\n";
    std::cerr.flush();
    
  }
  
}

__attribute__((__format__ (__printf__, 1, 0)))
void
warnf(const char* format, ...) {

  assert(format != 0);

  va_list args;
  char buf[500];
  memset(buf,0,sizeof(buf));
  va_start (args, format);
  vsnprintf(buf,sizeof(buf)-1,format,args);
  va_end (args);
  std::cerr << OUTLINER_WARNPREFIX;
  std::cerr << buf;
  std::cerr << " -- exit\n";
  std::cerr.flush();
  
}

__attribute__((__format__ (__printf__, 1, 0)))
void
errf(const char* format, ...) {

  assert(format != 0);

  va_list args;
  char buf[500];
  memset(buf,0,sizeof(buf));
  va_start (args, format);
  vsnprintf(buf,sizeof(buf)-1,format,args);
  va_end (args);
  std::cerr << OUTLINER_ERRPREFIX;
  std::cerr << buf;
  std::cerr << " -- exit\n";
  std::cerr.flush();
  
}

__attribute__((__format__ (__printf__, 1, 0)))
void
fatalf(const char* format, ...) {

  assert(format != 0);

  va_list args;
  char buf[500];
  memset(buf,0,sizeof(buf));
  va_start (args, format);
  vsnprintf(buf,sizeof(buf)-1,format,args);
  va_end (args);
  std::cerr << OUTLINER_ERRPREFIX;
  std::cerr << buf;
  std::cerr << " -- exit\n";
  std::cerr.flush();
  exit(1);
  
}

__attribute__((__format__ (__printf__, 1, 0)))
void
infof(const char* format, ...) {

  assert(format != 0);

  if (info) {
    
    va_list args;
    char buf[500];
    memset(buf,0,sizeof(buf));
    va_start (args, format);
    vsnprintf(buf,sizeof(buf)-1,format,args);
    va_end (args);
    std::cout << OUTLINER_INFOPREFIX;
    std::cout << buf;
    std::cout << "\n";
    std::cout.flush();

  }
}

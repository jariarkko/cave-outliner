
///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <stdarg.h>
#include <assert.h>
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
    
  }
  
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

  }
}

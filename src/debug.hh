
#ifndef DEBUG_HH
#define DEBUG_HH

void debuginit(bool debug,
               bool deepdebug,
               bool deepdeepdebug);
void errf(const char* format, ...);
void debugf(const char* format, ...);
void deepdebugf(const char* format, ...);
void deepdeepdebugf(const char* format, ...);

#endif // DEBUG_HH

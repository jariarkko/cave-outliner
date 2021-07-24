
#ifndef OUTLINERDEBUG_HH
#define OUTLINERDEBUG_HH

void debuginit(bool debug,
               bool deepdebug,
               bool deepdeepdebug);
void infof(const char* format, ...);
void errf(const char* format, ...);
void debugf(const char* format, ...);
void deepdebugf(const char* format, ...);
void deepdeepdebugf(const char* format, ...);

#endif // OUTLINERDEBUG_HH

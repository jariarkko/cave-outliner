
#ifndef OUTLINERDEBUG_HH
#define OUTLINERDEBUG_HH

void debuginit(bool infoSet,
               bool debugSet,
               bool deepdebugSet,
               bool deepdeepdebugSet);
void infof(const char* format, ...);
void errf(const char* format, ...);
void debugf(const char* format, ...);
void deepdebugf(const char* format, ...);
void deepdeepdebugf(const char* format, ...);

#endif // OUTLINERDEBUG_HH

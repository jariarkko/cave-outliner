
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"
#include "outlinerdebug.hh"

float
DirectionOperations::outputx(enum outlinerdirection direction,
                             const aiVector3D& point) {
  switch (direction) {
  case dir_z:
    return(point.x);
  case dir_x:
    return(point.z);
  case dir_y:
    return(point.x);
  default:
    errf("Invalid internal variable: direction %u", direction);
    exit(1);
  }
}

float
DirectionOperations::outputy(enum outlinerdirection direction,
                             const aiVector3D& point) {
  switch (direction) {
  case dir_z:
    return(point.y);
  case dir_x:
    return(point.y);
  case dir_y:
    return(point.z);
  default:
    errf("Invalid internal variable: direction %u", direction);
    exit(1);
  }
}


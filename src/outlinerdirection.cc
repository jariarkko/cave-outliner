
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"
#include "outlinerdebug.hh"

enum outlinerdirection
DirectionOperations::screenx(enum outlinerdirection direction) {
  switch (direction) {
  case dir_z:
    return(dir_y);
  case dir_x:
    return(dir_y);
  case dir_y:
    return(dir_z);
  default:
    errf("Invalid internal variable: direction %u", direction);
    exit(1);
  }
}

const char*
DirectionOperations::toString(enum outlinerdirection direction) {
  switch (direction) {
  case dir_z:
    return("z");
  case dir_x:
    return("x");
  case dir_y:
    return("y");
  default:
    errf("Invalid internal variable: direction %u", direction);
    exit(1);
  }
}

float
DirectionOperations::outputx(enum outlinerdirection direction,
                             const aiVector3D& point) {
  switch (direction) {
  case dir_z:
    return(point.x);
  case dir_x:
    return(point.y);
  case dir_y:
    return(point.x);
  default:
    errf("Invalid internal variable: direction %u", direction);
    exit(1);
  }
}

float
DirectionOperations::outputx(enum outlinerdirection direction,
                             const OutlinerVector3D& point) {
  switch (direction) {
  case dir_z:
    return(point.x);
  case dir_x:
    return(point.y);
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
    return(point.z);
  case dir_y:
    return(point.z);
  default:
    errf("Invalid internal variable: direction %u", direction);
    exit(1);
  }
}

float
DirectionOperations::outputy(enum outlinerdirection direction,
                             const OutlinerVector3D& point) {
  switch (direction) {
  case dir_z:
    return(point.y);
  case dir_x:
    return(point.z);
  case dir_y:
    return(point.z);
  default:
    errf("Invalid internal variable: direction %u", direction);
    exit(1);
  }
}

float
DirectionOperations::outputz(enum outlinerdirection direction,
                             const aiVector3D& point) {
  switch (direction) {
  case dir_z:
    return(point.z);
  case dir_x:
    return(point.y);
  case dir_y:
    return(point.y);
  default:
    errf("Invalid internal variable: direction %u", direction);
    exit(1);
  }
}

float
DirectionOperations::outputz(enum outlinerdirection direction,
                             const OutlinerVector3D& point) {
  switch (direction) {
  case dir_z:
    return(point.z);
  case dir_x:
    return(point.y);
  case dir_y:
    return(point.y);
  default:
    errf("Invalid internal variable: direction %u", direction);
    exit(1);
  }
}

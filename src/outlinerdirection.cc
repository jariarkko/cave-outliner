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

#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"
#include "outlinerdebug.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Functions //////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

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

outlinerreal
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

outlinerreal
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

outlinerreal
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

outlinerreal
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

outlinerreal
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

outlinerreal
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

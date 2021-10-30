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

#ifndef OUTLINERDIRECTION_HH
#define OUTLINERDIRECTION_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "outlinerhighprecision.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Types //////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This type is a direction enumerated type, which is used to
/// represent the direction of view for the desired plan view.
///

enum outlinerdirection {
  dir_z,
  dir_x,
  dir_y
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This is part of the "direction of view" (enum outlinerdirection)
/// setup It is an associated class that helps perform mappings
/// between coordinates from a given direction.
///

class DirectionOperations {

public:

  /// Looking at the plan view, return the direction that corresponds
  /// to the x coordinate in that plan view, given the user-specified
  /// direction of view. For instance, if the user has specified that
  /// he or she wishes to look at the plan view from the model's 'z'
  /// direction (the default), then the x coordinate in the image is
  /// what was 'y' coordinate in the 3D model.
  static enum outlinerdirection screenx(enum outlinerdirection direction);

  /// Convert a direction to a string. Used for debugging.
  static const char* toString(enum outlinerdirection direction);

  /// Return the planview x coordinate in a vector, given a
  /// user-specified direction for the plan view.
  static float outputx(enum outlinerdirection direction,
                       const aiVector3D& point);
  /// Return the planview x coordinate in a vector, given a
  /// user-specified direction for the plan view.
  static float outputx(enum outlinerdirection direction,
                       const OutlinerVector3D& point);
  /// Return the planview y coordinate in a vector, given a
  /// user-specified direction for the plan view.
  static float outputy(enum outlinerdirection direction,
                       const aiVector3D& point);
  /// Return the planview y coordinate in a vector, given a
  /// user-specified direction for the plan view.
  static float outputy(enum outlinerdirection direction,
                       const OutlinerVector3D& point);
  /// Return the planview invisible z coordinate in a vector, 
  /// given a user-specified direction for the plan view.
  static float outputz(enum outlinerdirection direction,
                       const aiVector3D& point);
  /// Return the planview invisible z coordinate in a vector, 
  /// given a user-specified direction for the plan view.
  static float outputz(enum outlinerdirection direction,
                       const OutlinerVector3D& point);

};

#endif // OUTLINERDIRECTION_HH

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

#ifndef OUTLINERBOUNDINGBOXER_HH
#define OUTLINERBOUNDINGBOXER_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assimp/scene.h>
#include "outlinertypes.hh"
#include "outlinerhighprecision.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class definition ///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

///
/// This class enables the calculation to determine the size of a given
/// model, i.e., the bounding box the 3D model fits in.
///

class BoundingBoxer {

public:

  /// Construct an object can calculate the bounding box.
  BoundingBoxer(const aiScene* scene,
                outlinerreal xIncrease,
                outlinerreal yIncrease,
                outlinerreal zIncrease);

  /// Calculate the bounding box.
  void getOriginalBoundingBox(OutlinerBox3D& boundingBoxOut);
  
  /// Calculate the bounding box, with a given extra space around
  void getIncreasedBoundingBox(OutlinerBox3D& boundingBoxOut);

  /// Release all resources associated with the bounding box calculation.
  ~BoundingBoxer();

private:
  bool boundingBoxSet;
  OutlinerBox3D boundingBox;
  OutlinerBox3D increasedBoundingBox;
  
  void boundingScene(const aiScene* scene);
  void boundingNode(const aiScene* scene,
                    const aiNode* node);
  void boundingMesh(const aiMesh* mesh);
  void boundingFaces(const aiMesh* mesh);
  void boundingFace(const aiMesh* mesh,
                    const aiFace* face);

};

#endif // OUTLINERBOUNDINGBOXER_HH

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

#ifndef INDEXEDMESH_HH
#define INDEXEDMESH_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinerdirection.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Internal data types ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

struct IndexedMeshOneMeshOneTileFaces {
  unsigned int nFaces;
  unsigned int maxNFaces;
  const aiFace** faces;
};

struct IndexedMeshOneMesh {
  const aiMesh* mesh;
  unsigned int nOutsideModelBoundingBox;
  struct IndexedMeshOneMeshOneTileFaces** tileMatrix;
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class IndexedMesh {

public:

  IndexedMesh(unsigned int maxMeshesIn,
              unsigned int subdivisionsIn,
              const OutlinerVector3D& modelBoundingBoxStartIn,
              const OutlinerVector3D& modelBoundingBoxEndIn,
              const OutlinerVector2D& viewBoundingBoxStartIn,
              const OutlinerVector2D& viewBoundingBoxEndIn,
              enum outlinerdirection directionIn);
  void addScene(const aiScene* scene);
  void addNode(const aiScene* scene,
               const aiNode* node);
  void addMesh(const aiScene* scene,
               const aiMesh* mesh);
  void getFaces(const aiMesh* mesh,
                outlinerreal x,
                outlinerreal y,
                unsigned int* p_nFaces,
                const aiFace*** p_faces);
   ~IndexedMesh();
   
private:

  unsigned int nMeshes;
  unsigned int maxMeshes;
  unsigned int subdivisions;
  OutlinerVector3D modelBoundingBoxStart;
  OutlinerVector3D modelBoundingBoxEnd;
  OutlinerVector2D viewBoundingBoxStart;
  OutlinerVector2D viewBoundingBoxEnd;
  enum outlinerdirection direction;
  outlinerreal tileSizeX;
  outlinerreal tileSizeY;
  
  struct IndexedMeshOneMesh* meshes;
  
  void addFaces(struct IndexedMeshOneMesh& shadow,
                const aiScene* scene,
                const aiMesh* mesh);
  void addFace(struct IndexedMeshOneMesh& shadow,
               const aiScene* scene,
               const aiMesh* mesh,
               const aiFace* face);
  void addToTile(struct IndexedMeshOneMesh& shadow,
                 const aiScene* scene,
                 const aiMesh* mesh,
                 const aiFace* face,
                 unsigned int tileX,
                 unsigned int tileY);
  void getFacesTile(struct IndexedMeshOneMesh& shadow,
                    const aiMesh* mesh,
                    unsigned int tileX,
                    unsigned int tileY,
                    unsigned int* p_nFaces,
                    const aiFace*** p_faces);
  void coordsToTile(outlinerreal x,
                    outlinerreal y,
                    unsigned int& tileX,
                    unsigned int& tileY);
  void getShadow(const aiMesh* mesh,
                 struct IndexedMeshOneMesh** shadow);
  unsigned int minFacesPerTile(struct IndexedMeshOneMesh& shadow,
                               unsigned int& n);
  unsigned int maxFacesPerTile(struct IndexedMeshOneMesh& shadow,
                               unsigned int& n);
  float avgFacesPerTile(struct IndexedMeshOneMesh& shadow);
  unsigned int countTilesWithFaces(struct IndexedMeshOneMesh& shadow);
  void countFaces(struct IndexedMeshOneMesh& shadow,
                  unsigned int& nUniqueFaces,
                  unsigned int& nFacesInTiles);
};

#endif // INDEXEDMESH_HH

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

///
/// This object represents an optimized index to the mesh faces
/// contained in an imported 3D model. The imported model has a large
/// datastructure of 'faces' -- typically millions or even tens of
/// millions of faces. There is no efficient way to search for faces
/// at a given location in the 3D or 2D space, however. The indexed
/// mesh object sorts the faces into a 2D matrix of 'tiles'. For
/// instance, a large model could be split into 20x20 or 400 tiles, so
/// that when we are looking for a face within given (x,y)
/// coordinates, we only need to look at the tile where (x,y) falls
/// into. The indexing is performed only once, and all searches after
/// the indexing operation can use the more efficient search.
///

class IndexedMesh {

public:

  /// Create an IndexedMesh object.
  IndexedMesh(unsigned int maxMeshesIn,
              unsigned int subdivisionsIn,
              const OutlinerBox3D& modelBoundingBox,
              const OutlinerBox2D& viewBoundingBox,
              enum outlinerdirection directionIn);

  /// Add a 3D scene to the optimized index.
  void addScene(const aiScene* scene);
  
  /// Add a 3D node to the optimized index.
  void addNode(const aiScene* scene,
               const aiNode* node);
  
  /// Add a 3D mesh to the optimized index.
  void addMesh(const aiScene* scene,
               const aiMesh* mesh);
  
  /// Quickly get faces associated with a given (x,y) point in the plan view.
  void getFaces(const aiMesh* mesh,
                outlinerreal x,
                outlinerreal y,
                unsigned int* p_nFaces,
                const aiFace*** p_faces);
  
  /// Print information about the contents of the mesh
  void describe(std::ostream& stream);
  
  /// Release all resources associated with the index.
   ~IndexedMesh();
   
private:

  unsigned int nMeshes;
  unsigned int maxMeshes;
  unsigned int subdivisions;
  OutlinerBox3D modelBoundingBox;
  OutlinerBox2D viewBoundingBox;
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

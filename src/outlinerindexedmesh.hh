
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
              const HighPrecisionVector3D& modelBoundingBoxStartIn,
              const HighPrecisionVector3D& modelBoundingBoxEndIn,
              const HighPrecisionVector2D& viewBoundingBoxStartIn,
              const HighPrecisionVector2D& viewBoundingBoxEndIn,
              enum outlinerdirection directionIn);
  void addScene(const aiScene* scene);
  void addNode(const aiScene* scene,
               const aiNode* node);
  void addMesh(const aiScene* scene,
               const aiMesh* mesh);
  void getFaces(const aiMesh* mesh,
                outlinerhighprecisionreal x,
                outlinerhighprecisionreal y,
                unsigned int* p_nFaces,
                const aiFace*** p_faces);
   ~IndexedMesh();
   
private:

  unsigned int nMeshes;
  unsigned int maxMeshes;
  unsigned int subdivisions;
  HighPrecisionVector3D modelBoundingBoxStart;
  HighPrecisionVector3D modelBoundingBoxEnd;
  HighPrecisionVector2D viewBoundingBoxStart;
  HighPrecisionVector2D viewBoundingBoxEnd;
  enum outlinerdirection direction;
  outlinerhighprecisionreal tileSizeX;
  outlinerhighprecisionreal tileSizeY;
  
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
  void coordsToTile(outlinerhighprecisionreal x,
                    outlinerhighprecisionreal y,
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

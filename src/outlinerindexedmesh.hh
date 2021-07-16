
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
  struct IndexedMeshOneMeshOneTileFaces** tileMatrix;
};

///////////////////////////////////////////////////////////////////////////////////////////////
// Class interface ////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class IndexedMesh {

public:

  IndexedMesh(unsigned int maxMeshesIn,
              unsigned int subdivisionsIn,
              const aiVector2D& viewBoundingBoxStartIn,
              const aiVector2D& viewBoundingBoxEndIn);
  void addScene(const aiScene* scene);
  void addNode(const aiScene* scene,
               const aiNode* node);
  void addMesh(const aiScene* scene,
               const aiMesh* mesh);
  void getFaces(float x,
                float y,
                unsigned int* p_nFaces,
                const aiFace*** p_faces);
   ~IndexedMesh();
   
private:

  unsigned int nMeshes;
  unsigned int maxMeshes;
  unsigned int subdivisions;
  aiVector2D viewBoundingBoxStart;
  aiVector2D viewBoundingBoxEnd;
  float tileSizeX;
  float tileSizeY;
  
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
  void getFacesTile(unsigned int xTile,
                    unsigned int yTile,
                    unsigned int* p_nFaces,
                    const aiFace*** p_faces);
  void coordsToTile(float x,
                    float y,
                    unsigned int& tileX,
                    unsigned int& tileY);
};

#endif // INDEXEDMESH_HH

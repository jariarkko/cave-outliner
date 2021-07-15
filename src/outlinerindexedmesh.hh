
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
              unsigned int subdivisionsIn);
  void addScene(const aiScene* scene);
  void addNode(const aiScene* scene,
               const aiNode* node);
  void addMesh(const aiScene* scene,
               const aiMesh* mesh);
  void getFaces(unsigned int* p_nFaces,
                const aiFace** p_faces);
   ~IndexedMesh();
   
private:

  unsigned int nMeshes;
  unsigned int maxMeshes;
  unsigned int subdivisions;
  
  struct IndexedMeshOneMesh* meshes;
  
  void addFaces(struct IndexedMeshOneMesh& shadow,
                const aiScene* scene,
                const aiMesh* mesh);
  void addFace(struct IndexedMeshOneMesh& shadow,
               const aiScene* scene,
               const aiFace* face);
  void getFacesTile(unsigned int xTile,
                    unsigned int yTile,
                    unsigned int* p_nFaces,
                    const aiFace** p_faces);
};

#endif // INDEXEDMESH_HH


#ifndef INDEXEDMESH_HH
#define INDEXEDMESH_HH

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"

struct IndexedMeshOneMesh {
  const aiMesh* mesh;
};

class IndexedMesh {

public:

  IndexedMesh(unsigned int maxMeshesIn,
              unsigned int subdivisionsIn);
  ~IndexedMesh();
   
private:

  unsigned int nMeshes;
  unsigned int maxMeshesIn;
  unsigned int subdivisions;
  
  struct IndexedMeshOneMesh* meshes;
  
};

#endif // INDEXEDMESH_HH

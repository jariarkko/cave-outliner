
#ifndef PROCESSOR_HH
#define PROCESSOR_HH

///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinersvg.hh"
#include "outlinerindexedmesh.hh"
#include "outlinermaterialmatrix.hh"

class IndexedMesh;

class Processor {

public:
  Processor(aiVector3D boundingboxstartIn,
            aiVector3D boundingboxendIn,
            float stepxIn,
            float stepyIn,
            enum outlinerdirection directionIn,
            enum outlineralgorithm algorithmIn,
            IndexedMesh& indexedIn);
  ~Processor();
  
  bool processScene(const aiScene* scene,
                    SvgCreator& svg);

private:
  
  aiVector3D boundingboxstart;
  aiVector3D boundingboxend;
  float stepx;
  float stepy;
  enum outlinerdirection direction;
  enum outlineralgorithm algorithm;
  MaterialMatrix matrix;
  IndexedMesh& indexed;
  
  bool sceneHasMaterial(const aiScene* scene,
                        IndexedMesh& indexed,
                        float x,
                        float y);
  bool nodeHasMaterial(const aiScene* scene,
                       const aiNode* node,
                       IndexedMesh& indexed,
                       float x,
                       float y);
  bool meshHasMaterial(const aiScene* scene,
                       const aiMesh* mesh,
                       IndexedMesh& indexed,
                       float x,
                       float y);
  bool faceHasMaterial(const aiScene* scene,
                       const aiMesh* mesh,
                       const aiFace* face,
                       float x,
                       float y);
};

#endif // PROCESSOR_HH

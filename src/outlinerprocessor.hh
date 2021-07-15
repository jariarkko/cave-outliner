
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

class IndexedMesh;

bool processScene(const aiScene* scene,
                  aiVector3D boundingboxstart,
                  aiVector3D boundingboxend,
                  float step,
                  enum outlineralgorithm alg,
                  IndexedMesh& indexed,
                  SvgCreator& svg);

#endif // PROCESSOR_HH

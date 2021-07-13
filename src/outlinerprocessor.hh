
#ifndef PROCESSOR_HH
#define PROCESSOR_HH

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "outlinersvg.hh"

bool processScene(const aiScene* scene,
                  aiVector3D boundingboxstart,
                  aiVector3D boundingboxend,
                  float step,
                  enum outlineralgorithm alg,
                  SvgCreator& svg);

#endif // PROCESSOR_HH


#ifndef PROCESSOR_HH
#define PROCESSOR_HH

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"
#include "svg.hh"

bool processScene(const aiScene* scene,
                  SvgCreator& svg);

#endif // PROCESSOR_HH

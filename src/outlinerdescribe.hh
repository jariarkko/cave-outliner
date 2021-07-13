
#ifndef DESCRIBE_HH
#define DESCRIBE_HH

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinertypes.hh"
#include "outlinerconstants.hh"

void describeScene(const aiScene* scene,
                   bool transforms,
                   bool recurse,
                   bool vertexes,
                   bool faces);
void describeNode(const aiScene* scene,
                  const aiNode* node,
                  bool transforms,
                  bool recurse);
void describeFace(const aiScene* scene,
                  const aiMesh* mesh,
                  const aiFace* face);
void describeVertex(const aiScene* scene,
                    const aiMesh* mesh,
                    const aiVector3D* vertex);
void describeMesh(const aiScene* scene,
                  unsigned int no,
                  const aiMesh* mesh,
                  bool vertexes,
                  bool faces);
void describeTransformation(const aiMatrix4x4& x,
                            char* buf,
                            unsigned bufsiz);
void describeVector3D(const aiVector3D& x,
                      char* buf,
                      unsigned bufsiz);
 
#endif // DESCRIBE_HH


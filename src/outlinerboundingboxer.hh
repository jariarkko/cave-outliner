
#ifndef OUTLINERBOUNDINGBOXER_HH
#define OUTLINERBOUNDINGBOXER_HH

class BoundingBoxer {

public:
  BoundingBoxer(const aiScene* scene);
  void getBoundingBox(HighPrecisionVector3D& boundingBoxStartOut,
                      HighPrecisionVector3D& boundingBoxEndOut);
  ~BoundingBoxer();

private:
  bool boundingBoxSet;
  HighPrecisionVector3D boundingBoxStart;
  HighPrecisionVector3D boundingBoxEnd;

  void boundingScene(const aiScene* scene);
  void boundingNode(const aiScene* scene,
                    const aiNode* node);
  void boundingMesh(const aiMesh* mesh);
  void boundingFaces(const aiMesh* mesh);
  void boundingFace(const aiMesh* mesh,
                    const aiFace* face);

};

#endif // OUTLINERBOUNDINGBOXER_HH

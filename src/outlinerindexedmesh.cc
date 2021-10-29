
///////////////////////////////////////////////////////////////////////////////////////////////
// Includes ///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <stdlib.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "outlinerdebug.hh"
#include "outlinermath.hh"
#include "outlinerindexedmesh.hh"

///////////////////////////////////////////////////////////////////////////////////////////////
// Class methods //////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

IndexedMesh::IndexedMesh(unsigned int maxMeshesIn,
                         unsigned int subdivisionsIn,
                         const OutlinerVector3D& modelBoundingBoxStartIn,
                         const OutlinerVector3D& modelBoundingBoxEndIn,
                         const OutlinerVector2D& viewBoundingBoxStartIn,
                         const OutlinerVector2D& viewBoundingBoxEndIn,
                         enum outlinerdirection directionIn)
  : nMeshes(0),
    maxMeshes(maxMeshesIn),
    subdivisions(subdivisionsIn),
    modelBoundingBoxStart(modelBoundingBoxStartIn),
    modelBoundingBoxEnd(modelBoundingBoxEndIn),
    viewBoundingBoxStart(viewBoundingBoxStartIn),
    viewBoundingBoxEnd(viewBoundingBoxEndIn),
    direction (directionIn) {
  assert(maxMeshesIn >= 1);
  assert(subdivisionsIn >= 1);
  debugf("%u x %u tiles, or %u tiles", subdivisions, subdivisions, subdivisions * subdivisions);
  outlinerreal viewX = viewBoundingBoxEnd.x - viewBoundingBoxStart.x;
  outlinerreal viewY = viewBoundingBoxEnd.y - viewBoundingBoxStart.y;
  tileSizeX = viewX / subdivisions;
  tileSizeY = viewY / subdivisions;
  debugf("view bounding box %f x %f", viewX, viewY);
  debugf("tile size %f x %f", tileSizeX, tileSizeY);
  meshes = new IndexedMeshOneMesh [maxMeshes];
  if (meshes == 0) {
    errf("Cannot allocate %u indexed meshes", maxMeshes);
    exit(1);
  }
  for (unsigned int i = 0; i < maxMeshes; i++) {
    meshes[i].mesh = 0;
    meshes[i].nOutsideModelBoundingBox = 0;
    meshes[i].tileMatrix = new struct IndexedMeshOneMeshOneTileFaces* [subdivisions];
    if (meshes[i].tileMatrix == 0) {
      errf("Cannot allocate %u tile matrix", subdivisions);
      exit(1);
    }
    memset(meshes[i].tileMatrix,0,sizeof(struct IndexedMeshOneMeshOneTileFaces*) * subdivisions);
    for (unsigned int j = 0; j < subdivisions; j++) {
      meshes[i].tileMatrix[j] = new struct IndexedMeshOneMeshOneTileFaces [subdivisions];
      if (meshes[i].tileMatrix[j] == 0) {
        errf("Cannot allocate %u tile matrix second dimension", subdivisions);
        exit(1);
      }
      for (unsigned int k = 0; k < subdivisions;k++) {
        meshes[i].tileMatrix[j][k].nFaces = 0;
        meshes[i].tileMatrix[j][k].maxNFaces = 0;
        meshes[i].tileMatrix[j][k].faces = 0;
      }
    }
  }
}

unsigned int
IndexedMesh::minFacesPerTile(struct IndexedMeshOneMesh& shadow,
                             unsigned int& n) {
  n = 0;
  unsigned int min = 99999999;
  for (unsigned int tileX = 0; tileX < subdivisions; tileX++) {
    for (unsigned int tileY = 0; tileY < subdivisions; tileY++) {
      unsigned int nFaces;
      const aiFace** faces;
      getFacesTile(shadow,
                   shadow.mesh,
                   tileX,
                   tileY,
                   &nFaces,
                   &faces);
      if (nFaces < min) {
        min = nFaces;
        n = 1;
      } else if (nFaces == min) {
        n++;
      }
    }
  }
  return(min);
}

unsigned int
IndexedMesh::maxFacesPerTile(struct IndexedMeshOneMesh& shadow,
                             unsigned int& n) {
  n = 0;
  unsigned int max = 0;
  for (unsigned int tileX = 0; tileX < subdivisions; tileX++) {
    for (unsigned int tileY = 0; tileY < subdivisions; tileY++) {
      unsigned int nFaces;
      const aiFace** faces;
      getFacesTile(shadow,
                   shadow.mesh,
                   tileX,
                   tileY,
                   &nFaces,
                   &faces);
      if (nFaces > max || (n == 0 && max == 0)) {
        max = nFaces;
        n = 1;
      } else if (nFaces == max) {
        n++;
      }
    }
  }
  return(max);
}

unsigned int
IndexedMesh::countTilesWithFaces(struct IndexedMeshOneMesh& shadow) {
  unsigned int n = 0;
  for (unsigned int tileX = 0; tileX < subdivisions; tileX++) {
    for (unsigned int tileY = 0; tileY < subdivisions; tileY++) {
      unsigned int nFaces;
      const aiFace** faces;
      getFacesTile(shadow,
                   shadow.mesh,
                   tileX,
                   tileY,
                   &nFaces,
                   &faces);
      if (nFaces > 0) n++;
    }
  }
  return(n);
}

float
IndexedMesh::avgFacesPerTile(struct IndexedMeshOneMesh& shadow) {
  unsigned int n = 0;
  float sum = 0;
  for (unsigned int tileX = 0; tileX < subdivisions; tileX++) {
    for (unsigned int tileY = 0; tileY < subdivisions; tileY++) {
      unsigned int nFaces;
      const aiFace** faces;
      getFacesTile(shadow,
                   shadow.mesh,
                   tileX,
                   tileY,
                   &nFaces,
                   &faces);
      if (nFaces > 0) {
        n++;
        sum += nFaces;
      }
    }
  }
  return(sum/n);
}

void
IndexedMesh::countFaces(struct IndexedMeshOneMesh& shadow,
                        unsigned int& nUniqueFaces,
                        unsigned int& nFacesInTiles) {
  nUniqueFaces = shadow.mesh->mNumFaces;
  nFacesInTiles = 0;
  for (unsigned int tileX = 0; tileX < subdivisions; tileX++) {
    for (unsigned int tileY = 0; tileY < subdivisions; tileY++) {
      unsigned int nFaces;
      const aiFace** faces;
      getFacesTile(shadow,
                   shadow.mesh,
                   tileX,
                   tileY,
                   &nFaces,
                   &faces);
      nFacesInTiles += nFaces;
    }
  }
}

void
IndexedMesh::addScene(const aiScene* scene) {
  infof("computing tiling...");
  addNode(scene,scene->mRootNode);
  infof("  tiling statistics");
  for (unsigned int i = 0; i < nMeshes; i++) {
    struct IndexedMeshOneMesh& shadow = meshes[i];
    unsigned int nth = i+1;
    const char* nthstring = "th";
    switch (nth) {
    case 1: nthstring = "st"; break;
    case 2: nthstring = "nd"; break;
    case 3: nthstring = "rd"; break;
    }
    infof("    %u%s mesh",
          nth,
          nthstring);
    unsigned int nf;
    unsigned int mf;
    countFaces(shadow,nf,mf);
    infof("      unique faces %u (total %u, %u more, original face count %u)",
          nf, mf, mf - nf,
          shadow.mesh->mNumFaces);
    infof("      ignored %u (%.2f%%) faces as being outside 3D bounding box",
          shadow.nOutsideModelBoundingBox,
          (100.0 * (double)shadow.nOutsideModelBoundingBox) / (double)shadow.mesh->mNumFaces);
    unsigned int total = subdivisions * subdivisions;
    infof("      %u x %u = %u tiles", subdivisions, subdivisions, total);
    infof("      tile sizes %.2f x and %.2f y", tileSizeX, tileSizeY);
    unsigned int all = countTilesWithFaces(shadow);
    infof("      tiles with faces %u (%.2f%%)", all, (100.0*(float)all)/(float)total);
    unsigned int nmin;
    unsigned int min = minFacesPerTile(shadow,nmin);
    infof("      min number of faces per tile %u (%u instances, %.2f%%)", min, nmin, (100.0*(float)nmin)/(float)total);
    unsigned int nmax;
    unsigned int max = maxFacesPerTile(shadow,nmax);
    infof("      max number of faces per tile %u (that's %.2f%% of total faces, there were %u instances, %.2f%%)",
          max,
          (100.0*(float)max)/(float)nf,
          nmax,
          (100.0*(float)nmax)/(float)total);
    float avg = avgFacesPerTile(shadow);
    infof("      avg number of faces per tile %.1f", avg);
  }
}

void
IndexedMesh::addNode(const aiScene* scene,
                     const aiNode* node) {
  for (unsigned int j = 0; j < node->mNumMeshes; j++) {
    addMesh(scene,scene->mMeshes[node->mMeshes[j]]);
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    addNode(scene,node->mChildren[i]);
  }
}

void
IndexedMesh::addMesh(const aiScene* scene,
                     const aiMesh* mesh) {
  assert(scene != 0);
  assert(mesh != 0);
  if (nMeshes == maxMeshes) {
    errf("Cannot another mesh, already have %u", maxMeshes);
    exit(1);
  }
  struct IndexedMeshOneMesh& newMesh = meshes[nMeshes];
  newMesh.mesh = mesh;
  newMesh.nOutsideModelBoundingBox = 0;
  assert(newMesh.mesh == mesh);
  nMeshes++;
  addFaces(newMesh,scene,mesh);
}

void
IndexedMesh::addFaces(struct IndexedMeshOneMesh& shadow,
                      const aiScene* scene,
                      const aiMesh* mesh) {
  assert(scene != 0);
  assert(shadow.mesh == mesh);
  assert(mesh != 0);
  for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
    addFace(shadow,scene,mesh,&mesh->mFaces[f]);
  }
}

void
IndexedMesh::addFace(struct IndexedMeshOneMesh& shadow,
                     const aiScene* scene,
                     const aiMesh* mesh,
                     const aiFace* face) {

  // Sanity checks
  assert(shadow.mesh != 0);
  assert(shadow.mesh == mesh);
  assert(scene != 0);
  assert(mesh != 0);
  assert(face != 0);
  if (face->mNumIndices != 3) {
    errf("Cannot handle a face with %u indices", face->mNumIndices);
    exit(1);
  }
  if (face->mIndices[0] >= mesh->mNumVertices) {
    errf("Face points to a vertex %u that does not exist", face->mIndices[0]);
    exit(1);
  }
  if (face->mIndices[1] >= mesh->mNumVertices) {
    errf("Face points to a vertex %u that does not exist", face->mIndices[1]);
    exit(1);
  }
  if (face->mIndices[2] >= mesh->mNumVertices) {
    errf("Face points to a vertex %u that does not exist", face->mIndices[2]);
    exit(1);
  }

  // Calculate 3D bounding box
  aiVector3D* vertexA = &mesh->mVertices[face->mIndices[0]];
  aiVector3D* vertexB = &mesh->mVertices[face->mIndices[1]];
  aiVector3D* vertexC = &mesh->mVertices[face->mIndices[2]];
  OutlinerVector3D elementBoundingBoxStart;
  OutlinerVector3D elementBoundingBoxEnd;
  OutlinerTriangle3D t3(*vertexA,*vertexB,*vertexC);
  OutlinerMath::triangleBoundingBox3D(t3,
                                      elementBoundingBoxStart,elementBoundingBoxEnd);
  if (!OutlinerMath::boundingBoxesIntersect3D(modelBoundingBoxStart,
                                              modelBoundingBoxEnd,
                                              elementBoundingBoxStart,elementBoundingBoxEnd)) {
    deepdebugf("not including face due to not being inside model bounding box");
    shadow.nOutsideModelBoundingBox++;
    return;
  }
  
  // Calculate 2D bounding box
  OutlinerVector2D a(DirectionOperations::outputx(direction,*vertexA),DirectionOperations::outputy(direction,*vertexA));
  OutlinerVector2D b(DirectionOperations::outputx(direction,*vertexB),DirectionOperations::outputy(direction,*vertexB));
  OutlinerVector2D c(DirectionOperations::outputx(direction,*vertexC),DirectionOperations::outputy(direction,*vertexC));
  OutlinerVector2D elementFlatBoundingBoxStart;
  OutlinerVector2D elementFlatBoundingBoxEnd;
  OutlinerTriangle2D t2(a,b,c);
  OutlinerMath::triangleBoundingBox2D(t2,
                                      elementFlatBoundingBoxStart,
                                      elementFlatBoundingBoxEnd);

  // Calculate which tiles this belongs to (for big faces, may be more
  // than one)
  outlinerreal xStart = outlinermax(viewBoundingBoxStart.x,elementFlatBoundingBoxStart.x);
  outlinerreal xEnd = outlinermin(elementFlatBoundingBoxEnd.x,viewBoundingBoxEnd.x);
  outlinerreal yStart = outlinermax(viewBoundingBoxStart.y,elementFlatBoundingBoxStart.y);
  outlinerreal yEnd = outlinermin(elementFlatBoundingBoxEnd.y,viewBoundingBoxEnd.y);
  debugf("placing a face bounding box (%f..%f) x (%f..%f) to tiles",
         xStart, xEnd,
         yStart, yEnd);
  for (outlinerreal x = xStart; x <= xEnd; x += tileSizeX * 0.5) {
    for (outlinerreal y = yStart; y <= yEnd; y += tileSizeY * 0.5) {
      unsigned int tileX;
      unsigned int tileY;
      coordsToTile(x,y,tileX,tileY);
      addToTile(shadow,scene,mesh,face,tileX,tileY);
    }
  }
}

void
IndexedMesh::addToTile(struct IndexedMeshOneMesh& shadow,
                       const aiScene* scene,
                       const aiMesh* mesh,
                       const aiFace* face,
                       unsigned int tileX,
                       unsigned int tileY) {

  // Sanity checks
  assert(shadow.mesh == mesh);
  assert(scene != 0);
  assert(mesh != 0);
  assert(face != 0);
  assert(tileX < subdivisions);
  assert(tileY < subdivisions);
  assert(shadow.tileMatrix != 0);
  deepdebugf("adding face to tile (%u/%u,%u/%u)",
             tileX, subdivisions, tileY, subdivisions);

  // Find the right row (x) in a matrix of tiles
  struct IndexedMeshOneMeshOneTileFaces* row = shadow.tileMatrix[tileX];
  assert(row != 0);

  // Find the right cell (y) in that row
  struct IndexedMeshOneMeshOneTileFaces* tile = &row[tileY];
  assert(tile->maxNFaces > 0 ||  tile->faces == 0);

  // Did we allocate a face table for this cell yet? If not, do it
  if (tile->maxNFaces == 0) {
    tile->maxNFaces = 200;
    tile->nFaces = 0;
    tile->faces = new const aiFace* [tile->maxNFaces];
    if (tile->faces == 0) {
      errf("Cannot allocate face table in tile (%u,%u)", tileX, tileY);
      exit(1);
    }
    memset(tile->faces,0,sizeof(const aiFace*) * tile->maxNFaces);
  }

  // Check to see if the face is already on the table. If yes, exit.
  for (unsigned int i = 0; i < tile->nFaces; i++) {
    if (tile->faces[i] == face) {
      debugf("face is already in the table");
      return;
    }
  }
  
  // Check to see if face table is big enough. If not, expand.
  if (tile->nFaces == tile->maxNFaces) {
    unsigned int oldMaxNFaces = tile->maxNFaces;
    const aiFace** oldFaces = tile->faces;
    unsigned int newMaxNFaces = tile->maxNFaces * 2 + 3;
    const aiFace** newFaces = new const aiFace* [newMaxNFaces];
    if (newFaces == 0) {
      errf("Cannot expand face table in tile (%u,%u) from %u to %u",
           tileX, tileY, oldMaxNFaces, newMaxNFaces);
      exit(1);
    }
    debugf("Expanded face table in tile (%u,%u) from %u to %u",
           tileX, tileY, oldMaxNFaces, newMaxNFaces);
    memset(newFaces,0,sizeof(const aiFace*) * newMaxNFaces);
    memcpy(newFaces,oldFaces,sizeof(const aiFace*) * tile->nFaces);
    tile->faces = newFaces;
    tile->maxNFaces = newMaxNFaces;
    delete oldFaces;
  }
  
  // Add the face to the table
  assert(tile->nFaces < tile->maxNFaces);
  tile->faces[tile->nFaces++] = face;
}

void
IndexedMesh::getShadow(const aiMesh* mesh,
                       struct IndexedMeshOneMesh** shadow) {
  for (unsigned int i = 0; i < nMeshes; i++) {
    if (meshes[i].mesh == mesh) {
      *shadow = &meshes[i];
      return;
    }
  }
  *shadow = 0;
}

void
IndexedMesh::getFaces(const aiMesh* mesh,
                      outlinerreal x,
                      outlinerreal y,
                      unsigned int* p_nFaces,
                      const aiFace*** p_faces) {
  unsigned int tileX;
  unsigned int tileY;
  coordsToTile(x,y,tileX,tileY);
  struct IndexedMeshOneMesh* shadow = 0;
  getShadow(mesh,&shadow);
  assert(shadow != 0);
  getFacesTile(*shadow,
               mesh,
               tileX,
               tileY,
               p_nFaces,
               p_faces);
  if (x > -0.605 && x < -0.595) {
    debugf("getFaces(%.2f,%.2f) tile (%u,%u) has %u faces", x, y, tileX, tileY, *p_nFaces);
  }
}

void
IndexedMesh::getFacesTile(struct IndexedMeshOneMesh& shadow,
                          const aiMesh* mesh,
                          unsigned int tileX,
                          unsigned int tileY,
                          unsigned int* p_nFaces,
                          const aiFace*** p_faces) {
  // Sanity checks
  assert(shadow.mesh != 0);
  assert(shadow.mesh == mesh);
  assert(mesh != 0);
  if (tileX >= subdivisions || tileY >= subdivisions) {
    debugf("getFacesTile(%u,%u)", tileX, tileY);
  }
  assert(tileX < subdivisions);
  assert(tileY < subdivisions);
  assert(shadow.tileMatrix != 0);
  assert(p_nFaces != 0);
  assert(p_faces != 0);
  deepdebugf("      faces in tile (%u/%u,%u/%u)",
             tileX, subdivisions, tileY, subdivisions);
  
  // Find the right row (x) in a matrix of tiles
  struct IndexedMeshOneMeshOneTileFaces* row = shadow.tileMatrix[tileX];
  assert(row != 0);
  
  // Find the right cell (y) in that row
  struct IndexedMeshOneMeshOneTileFaces* tile = &row[tileY];
  assert(tile->maxNFaces > 0 ||  tile->faces == 0);

  // Return the result
  if (tile->nFaces == 0) {
    *p_nFaces = 0;
    *p_faces = 0;
  } else {
    *p_nFaces = tile->nFaces;
    *p_faces = tile->faces;
  }

  deepdebugf("      returning %u faces", *p_nFaces);
}

void
IndexedMesh::coordsToTile(outlinerreal x,
                          outlinerreal y,
                          unsigned int& tileX,
                          unsigned int& tileY) {
  outlinerreal xInView = x - viewBoundingBoxStart.x;
  tileX = xInView / tileSizeX;
  assert(tileX <= subdivisions);
  if (tileX == subdivisions) tileX = subdivisions - 1;
  outlinerreal yInView = y - viewBoundingBoxStart.y;
  tileY = yInView / tileSizeY;
  assert(tileY <= subdivisions);
  if (tileY == subdivisions) tileY = subdivisions - 1;
  debugf("      coordinate (%f,%f) tile is (%u,%u)",
         x, y,
         tileX, tileY);
}

IndexedMesh::~IndexedMesh() {
  if (meshes != 0) {
    for (unsigned int i = 0; i < maxMeshes; i++) {
      if (meshes[i].tileMatrix == 0) continue;
      for (unsigned int j = 0; j < subdivisions; j++) {
        if (meshes[i].tileMatrix[j] == 0) continue;
        for (unsigned int k = 0; k < subdivisions;k++) {
          if (meshes[i].tileMatrix[j][k].faces != 0) {
            delete meshes[i].tileMatrix[j][k].faces;
          }
        }
        delete meshes[i].tileMatrix[j];
      }
      delete meshes[i].tileMatrix;
    }
    
    delete meshes;
    meshes = 0;
  }
  
  maxMeshes = 0;
  subdivisions = 0;
}


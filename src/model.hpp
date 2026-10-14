#ifndef MODEL_H_
#define MODEL_H_

#include "mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
 public:
  Model(const char *path) {
   loadModel(path);
  }
  void draw(Shader *shader);
 private:
  std::vector<Mesh> meshes;
  std::string directory;
  std::vector<Texture> texturesLoaded;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif // MODEL_H_

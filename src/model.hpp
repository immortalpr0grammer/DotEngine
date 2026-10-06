#ifndef MODEL_H_
#define MODEL_H_

#include "mesh.hpp"

#include "../include/linux/assimp/Importer.hpp"
#include "../include/linux/assimp/scene.h"
#include "../include/linux/assimp/postprocess.h"

class Model {
 public:
  Model(char *path) {
   loadModel(path);
  }
  void draw(Shader *shader);
 private:
  std::vector<Mesh> meshes;
  std::string directory;

  void loadModel(std::string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif // MODEL_H_

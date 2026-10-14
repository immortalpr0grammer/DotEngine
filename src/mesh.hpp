#ifndef MESH_H_
#define MESH_H_

#include <string>
#include <vector>

#include "../include/glm/glm.hpp"

#include "shader.hpp"

struct Vertex {
 glm::vec3 position;
 glm::vec2 normal;
 glm::vec2 texCoords;
};

struct Texture {
 unsigned int id;
 std::string type;
 std::string path;
};

class Mesh {
 public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
  void draw(Shader *shader);
 private:
  unsigned int VAO, VBO, EBO;

  void setupMesh();
};


#endif // MESH_H_

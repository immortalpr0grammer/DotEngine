#ifndef SHADER_H_
#define SHADER_H_

#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"

#include <string>

class shader {
 public:
  unsigned int ID;

  shader(const char* vertexPath, const char* fragmentPath);
  void use();
  void reload();

  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  void setVec3(const std::string &name, glm::vec3 value) const;
  void setVec4(const std::string &name, float x, float y, float z, float w) const;
  void setVec4(const std::string &name, glm::vec4 value) const;

  bool getBool(const std::string &name);
  int getInt(const std::string &name);
  float getFloat(const std::string &name);

 private:
  const char* vertexPath;
  const char* fragmentPath;
};


#endif // SHADER_H_

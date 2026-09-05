#ifndef SHADER_H_
#define SHADER_H_

#include "../include/glad/glad.h"

#include <string>

class shader {
 public:
  uint ID;

  shader(const char* vertexPath, const char* fragmentPath);
  void use();

  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
};


#endif // SHADER_H_

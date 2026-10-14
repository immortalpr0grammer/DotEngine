#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
 this->vertexPath = vertexPath;
 this->fragmentPath = fragmentPath;

 std::string vertexCode;
 std::string fragmentCode;
 std::ifstream vertexShaderFile;
 std::ifstream fragmentShaderFile;

 // Making sure exceptions work
 vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
 fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
 try {
  vertexShaderFile.open(vertexPath);
  fragmentShaderFile.open(fragmentPath);
  // Creating string streams to put the shader code into
  std::stringstream vertexShaderStream, fragmentShaderStream;

  vertexShaderStream << vertexShaderFile.rdbuf();
  fragmentShaderStream << fragmentShaderFile.rdbuf();
  vertexShaderFile.close();
  fragmentShaderFile.close();

  vertexCode = vertexShaderStream.str();
  fragmentCode = fragmentShaderStream.str();
 }
 catch(std::ifstream::failure e) {
  std::cout << "Read shader files error encountered\n";
 }

 const char *vertexShaderSource = vertexCode.c_str();
 const char *fragmentShaderSource = fragmentCode.c_str();

 unsigned int vertexShader;
 vertexShader = glCreateShader(GL_VERTEX_SHADER);
 glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
 glCompileShader(vertexShader);

 int success;
 glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
 if (!success) {
  char infolog[512];
  glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
  std::cout << "Vertex shader compilation error:\n" << infolog;
 }

 unsigned int fragmentShader;
 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
 glCompileShader(fragmentShader);

 glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
 if (!success) {
  char infolog[512];
  glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
  std::cout << "Fragment shader compilation error:\n" << infolog;
 }

 ID = glCreateProgram();
 glAttachShader(ID, vertexShader);
 glAttachShader(ID, fragmentShader);
 glLinkProgram(ID);

 glGetProgramiv(ID, GL_LINK_STATUS, &success);
 if (!success) {
  char infolog[512];
  glGetProgramInfoLog(ID, 512, NULL, infolog);
  std::cout << "Shader program linking error:\n" << infolog;
 }

 glDeleteShader(vertexShader);
 glDeleteShader(fragmentShader);
}

void Shader::use() {
 glUseProgram(ID);
}

void Shader::reload() {
 std::string vertexCode;
 std::string fragmentCode;
 std::ifstream vertexShaderFile;
 std::ifstream fragmentShaderFile;

 // Making sure exceptions work
 vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
 fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
 try {
  vertexShaderFile.open(vertexPath);
  fragmentShaderFile.open(fragmentPath);
  // Creating string streams to put the shader code into
  std::stringstream vertexShaderStream, fragmentShaderStream;

  vertexShaderStream << vertexShaderFile.rdbuf();
  fragmentShaderStream << fragmentShaderFile.rdbuf();
  vertexShaderFile.close();
  fragmentShaderFile.close();

  vertexCode = vertexShaderStream.str();
  fragmentCode = fragmentShaderStream.str();
 }
 catch(std::ifstream::failure e) {
  std::cout << "Read shader files error encountered\n";
 }

 const char *vertexShaderSource = vertexCode.c_str();
 const char *fragmentShaderSource = fragmentCode.c_str();

 unsigned int vertexShader;
 vertexShader = glCreateShader(GL_VERTEX_SHADER);
 glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
 glCompileShader(vertexShader);

 int success;
 glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
 if (!success) {
  char infolog[512];
  glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
  std::cout << "Vertex shader compilation error:\n" << infolog;
 }

 unsigned int fragmentShader;
 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
 glCompileShader(fragmentShader);

 glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
 if (!success) {
  char infolog[512];
  glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
  std::cout << "Fragment shader compilation error:\n" << infolog;
 }

 ID = glCreateProgram();
 glAttachShader(ID, vertexShader);
 glAttachShader(ID, fragmentShader);
 glLinkProgram(ID);

 glGetProgramiv(ID, GL_LINK_STATUS, &success);
 if (!success) {
  char infolog[512];
  glGetProgramInfoLog(ID, 512, NULL, infolog);
  std::cout << "Shader program linking error:\n" << infolog;
 }

 glDeleteShader(vertexShader);
 glDeleteShader(fragmentShader);
}

void Shader::setBool(const std::string &name, bool value) const {

 glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
 glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
 glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
 glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string &name, glm::vec3 value) const {
 glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
 glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setVec4(const std::string &name, glm::vec4 value) const {
 glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string &name, glm::mat4 value) const {
 glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

bool Shader::getBool(const std::string &name) {
 int returnValue;
 glGetUniformiv(ID, glGetUniformLocation(ID, name.c_str()), &returnValue);
 return (bool)returnValue;
}

int Shader::getInt(const std::string &name) {
 int returnValue;
 glGetUniformiv(ID, glGetUniformLocation(ID, name.c_str()), &returnValue);
 return returnValue;
}

float Shader::getFloat(const std::string &name) {
 float returnValue;
 glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &returnValue);
 return returnValue;
}

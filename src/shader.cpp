#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

shader::shader(const char* vertexPath, const char* fragmentPath) {
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

 uint vertexShader;
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

 uint fragmentShader;
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

void shader::use() {
 glUseProgram(ID);
}

void shader::setBool(const std::string &name, bool value) const {
 glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void shader::setInt(const std::string &name, int value) const {
 glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::setFloat(const std::string &name, float value) const {
 glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

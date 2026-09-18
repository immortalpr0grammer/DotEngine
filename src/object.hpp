#ifndef OBJECT_H_
#define OBJECT_H_

#include "../include/glad/glad.h"
#include <iostream>

class VBO {
public:
 unsigned int ID;

 VBO(const float *vertices, size_t verticesSize, GLenum glDrawType);
 void bind();
 void unbind();
};

class VAO {
public:
 unsigned int ID;

 VAO();
 void bind();
 void unbind();
};

class EBO {
public:
 unsigned int ID;

 EBO(unsigned int *indices, size_t indicesSize, GLenum glDrawType);
 void bind();
 void unbind();
};


#endif // OBJECT_H_

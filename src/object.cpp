#include "object.hpp"

VBO::VBO(const float *vertices, size_t verticesSize, GLenum glDrawType) {
 glGenBuffers(1, &this->ID);
 glBindBuffer(GL_ARRAY_BUFFER, this->ID);
 glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, glDrawType);
 glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::bind() {
 glBindBuffer(GL_ARRAY_BUFFER, this->ID);
}

void VBO::unbind() {
 glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VAO::VAO() {
 glGenVertexArrays(1, &this->ID);
}

void VAO::bind() {
 glBindVertexArray(this->ID);
}

void VAO::unbind() {
 glBindVertexArray(0);
}

EBO::EBO(unsigned int *indices, size_t indicesSize, GLenum glDrawType) {
 glGenBuffers(1, &this->ID);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::bind() {
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ID);
}

void EBO::unbind() {
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

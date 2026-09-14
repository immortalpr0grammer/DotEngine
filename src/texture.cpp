#include "../include/glad/glad.h"
#include "../include/stb_image.h"

#include "texture.hpp"

texture::texture(const char *path, unsigned int targetID) {
 ID = targetID;
 glGenTextures(1, &textureID);
 glActiveTexture(GL_TEXTURE0 + ID);
 glBindTexture(GL_TEXTURE_2D, textureID);

 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

 int textureWidth, textureHeight, nrChannels;
 unsigned char* textureData = stbi_load(path, &textureWidth, &textureHeight, &nrChannels, 0);
 if (textureData) {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
  glGenerateMipmap(GL_TEXTURE_2D);
 }
 else {
  std::cout << "Failed to load texture\n";
 }
 stbi_image_free(textureData);
 glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::bind2D() {
 glBindTexture(GL_TEXTURE_2D, textureID);
}

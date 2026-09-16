#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <iostream>

class texture {
 public:
  unsigned int textureID;
  unsigned int ID;
  texture(const char *path, unsigned int ID);
  void bind2D();
};

#endif // TEXTURE_H_

#ifndef CAMERA_H_
#define CAMERA_H_

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

class camera {
 public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  float yaw = 0.0f, pitch = 0.0f;

  camera(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraUp);
  glm::mat4 getViewMatrix();
  void turn(float yaw, float pitch);
  void forward(float value);
  void backward(float value);
  void right(float value);
  void left(float value);
};


#endif // CAMERA_H_

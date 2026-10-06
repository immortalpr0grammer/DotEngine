#include "camera.hpp"

Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraFront, glm::vec3 cameraUp) {
 this->position = cameraPosition;
 this->front = cameraFront;
 this->up = cameraUp;
}

glm::mat4 Camera::getViewMatrix() {
 glm::mat4 view;
 view = glm::lookAt(this->position, this->position + this->front, this->up);
 return view;
}

void Camera::turn(float yaw, float pitch) {
 this->yaw += yaw;
 this->pitch += pitch;
 if (this->pitch > 89.0f) {
  this->pitch = 89.0f;
 }
 if (this->pitch < -89.0f) {
  this->pitch = -89.0f;
 }

 glm::vec3 direction;
 direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
 direction.y = sin(glm::radians(this->pitch));
 direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
 this->front = glm::normalize(direction);
}

void Camera::forward(float value) {
 this->position += value * this->front;
}

void Camera::backward(float value) {
 this->position -= value * this->front;
}

void Camera::right(float value) {
 this->position += glm::normalize(glm::cross(this->front, this->up)) * value;
}

void Camera::left(float value) {
 this->position -= glm::normalize(glm::cross(this->front, this->up)) * value;
}

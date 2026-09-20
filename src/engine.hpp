#ifndef ENGINE_H_
#define ENGINE_H_


#include "../include/glad/glad.h"
#ifdef _WIN32
 #include "../include/GLFW/windows/glfw3.h"
#else
 #include "../include/GLFW/linux/glfw3.h"
#endif

#include "shader.hpp"
#include "camera.hpp"

namespace engine {
 extern float changeColorLastTime;
 extern float changeCursorModeLastTime;

 extern camera cam;

 extern float FOV;

 extern float deltaTime;
 extern float lastFrame;

 extern float lastX, lastY;
 extern float sensitivity;
 extern bool firstMouse;
 extern bool mouseActive;

 // To make the window resizable
 void framebuffer_size_callback(GLFWwindow* window, int width, int height);

 void mouse_callback(GLFWwindow* window, double xpos, double ypos);

 void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

 void processInput(GLFWwindow* window, shader shaderProgram);

 GLFWwindow* initEngine(int windowWidth, int windowHeight, char *windowName);

 int random(int min, int max);
}

#endif // ENGINE_H_

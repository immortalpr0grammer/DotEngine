#include "engine.hpp"
#include <iostream>

/* Global variables */
float engine::changeColorLastTime = 0.0f;
float engine::changeCursorModeLastTime = 0.0f;

camera engine::cam = camera(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

float engine::FOV = 45.0f;

float engine::deltaTime = 0.0f;
float engine::lastFrame = 0.0f;

float engine::lastX = 400, engine::lastY = 300;
float engine::sensitivity = 0.1f;
bool engine::firstMouse = true;
bool engine::mouseActive = true;


// To make the window resizable
void engine::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void engine::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
 if (!mouseActive) {return;}
 if (firstMouse) {
  lastX = xpos;
  lastY = ypos;
  firstMouse = false;
 }

 float xOffset = xpos - lastX;
 float yOffset = lastY - ypos; // Reversed, because they range from bottom to top
 lastX = xpos;
 lastY = ypos;

 xOffset *= sensitivity;
 yOffset *= sensitivity;

 float yaw = xOffset;
 float pitch = yOffset;

 cam.turn(yaw, pitch);
}


void engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
 FOV -= (float)yoffset;
 if (FOV < 1.0f)
     FOV = 1.0f;
 if (FOV > 45.0f)
     FOV = 45.0f;
}

void engine::processInput(GLFWwindow* window, shader shaderProgram) {
 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
  glfwSetWindowShouldClose(window, true);
 }
 if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && changeColorLastTime < glfwGetTime() - 0.5) {
  shaderProgram.setBool("useInColor", shaderProgram.getBool("useInColor") ^ 1); // Toggles it
  changeColorLastTime = glfwGetTime();
 }

 if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && changeCursorModeLastTime < glfwGetTime() - 0.5) {
  mouseActive ^= 1;
  changeCursorModeLastTime = glfwGetTime();
  if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
  else {
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
 }

 float cameraSpeed = 2.5f * deltaTime;
 if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
  cameraSpeed *= 2;
 }
 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
  cam.forward(cameraSpeed);
 }
 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
  cam.backward(cameraSpeed);
 }
 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
  cam.left(cameraSpeed);
 }
 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
  cam.right(cameraSpeed);
 }
}

GLFWwindow* engine::initEngine(int windowWidth, int windowHeight, char *windowName) {
 glfwInit();
 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
 glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

 GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "engine", NULL, NULL);
 if (window == NULL) {
   std::cout << "Failed to create GLFW window\n";
   glfwTerminate();
   return NULL;
 }

 glfwMakeContextCurrent(window);
 glfwSwapInterval(0); // Removing the 60 fps limit
 glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Making the cursor invisible in center when application has focus

 if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
  std::cout << "failed to initialize GLAD\n";
  return NULL;
 }

 glViewport(0, 0, windowWidth, windowHeight);
 // Making it do that function whenever the window gets resized
 glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 glfwSetCursorPosCallback(window, mouse_callback);
 glfwSetScrollCallback(window, scroll_callback);

 return window;
}

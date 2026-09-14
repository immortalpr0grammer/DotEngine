#include <iostream>
#include <cmath>
#include <unistd.h>

#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"
#include "../include/stb_image.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include "shader.hpp"
#include "texture.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

/* Todos from most important to least important */
// TODO: continue with learnopengl
// TODO: improve cooldown system

// Global variables
float changeColorLastTime = 0.0f;

const float vertices[] = {
// This doesnt use indices cause its just for testing
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
glm::vec3( 0.0f, 0.0f, 0.0f),
glm::vec3( 2.0f, 5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3( 2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f, 3.0f, -7.5f),
glm::vec3( 1.3f, -2.0f, -2.5f),
glm::vec3( 1.5f, 2.0f, -2.5f),
glm::vec3( 1.5f, 0.2f, -1.5f),
glm::vec3(-1.3f, 1.0f, -1.5f)
};

const unsigned int indices[] = {
};

glm::vec3 cameraPosition(0.0f, 0.0f, -1.0f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f); // Normalized
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

float yaw = 0.0f, pitch = 0.0f;
float FOV = 45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;
float sensitivity = 0.1f;
bool firstMouse = true;


// To make the window resizable
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
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

 yaw += xOffset;
 pitch += yOffset;

 if (pitch > 89.0f) {
  pitch = 89.0f;
 }
 else if (pitch < -89.0f) {
  pitch = -89.0f;
 }

 // Some math stuff for turning the camera that i copy pasted cause i dont understand it for shits sake
 glm::vec3 direction;
 direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
 direction.y = sin(glm::radians(pitch));
 direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
 cameraFront = glm::normalize(direction);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
 FOV -= (float)yoffset;
 if (FOV < 1.0f)
     FOV = 1.0f;
 if (FOV > 45.0f)
     FOV = 45.0f;
}

void processInput(GLFWwindow* window, shader shaderProgram) {
 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
  glfwSetWindowShouldClose(window, true);
 }
 if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && changeColorLastTime < glfwGetTime() - 0.5) {
  shaderProgram.setBool("useInColor", shaderProgram.getBool("useInColor") ^ 1); // Toggles it
  changeColorLastTime = glfwGetTime();
 }

 float cameraSpeed = 2.5f * deltaTime;
 if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
  cameraSpeed *= 2;
 }
 if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
  cameraPosition += cameraSpeed * cameraFront;
 }
 if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
  cameraPosition -= cameraSpeed * cameraFront;
 }
 if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
  cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
 }
 if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
  cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
 }
}

GLFWwindow* initEngine(int windowWidth, int windowHeight, char *windowName) {
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


int main() {
 std::cout << "\n";
 GLFWwindow* window = initEngine(WINDOW_WIDTH, WINDOW_HEIGHT, (char*)"engine");

 texture crate("textures/crate.png", 0);

 // VAO stores stuff like vertexattribpointers
 unsigned int VAO;
 glGenVertexArrays(1, &VAO);
 glBindVertexArray(VAO);

 // EBO stores indices to not have multiple of the same vertices
 unsigned int EBO;
 glGenBuffers(1, &EBO);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

 unsigned int VBO;
 glGenBuffers(1, &VBO);
 glBindBuffer(GL_ARRAY_BUFFER, VBO);
 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

 // Position attribute
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
 glEnableVertexAttribArray(0);
 // Texture coordinates attribute
 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
 glEnableVertexAttribArray(1);

 shader shaderProgram("shaders/vertex.sha", "shaders/fragment.sha");

 shaderProgram.use();

 shaderProgram.setInt("inTexture", crate.ID);

 shaderProgram.setBool("useInColor", false);

 glEnable(GL_DEPTH_TEST);

 glBindVertexArray(VAO);
 crate.bind2D();

 double timeSinceLastSecond = 0.0;


 while (!glfwWindowShouldClose(window)) {
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;

  processInput(window, shaderProgram);

  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram.use();

  float time = glfwGetTime();
  float greenValue = (sin(time) / 2.0f) + 0.5f;
  glUniform4f(glGetUniformLocation(shaderProgram.ID, "inColor"), 0.0f, greenValue, 0.0f, 1.0f);

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(FOV), 800.0f / 600.0f, 0.1f, 100.0f);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


  glm::mat4 view;
  view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

  for (unsigned int i = 0; i < 10; i++) {
   glm::mat4 model(1.0f);
   model = glm::translate(model, cubePositions[i]);
   model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 0.5f, 0.0f));
   glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

   glDrawArrays(GL_TRIANGLES, 0 , 36);
  }

  // Commented, cause for testing purposes indices arent being used
  // glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(float), GL_UNSIGNED_INT, 0);

  glfwSwapBuffers(window);
  glfwPollEvents();

  timeSinceLastSecond += deltaTime;
  if (timeSinceLastSecond > 1.0) {
   std::cout << "FPS: " << 1 / deltaTime << "\n";
   std::cout << "frameTime: " << deltaTime << "\n";
   timeSinceLastSecond -= 1.0;
  }
  lastFrame = currentFrame;
 }

 glfwTerminate();
 return 0;
}

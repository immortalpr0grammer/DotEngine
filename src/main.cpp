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

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

/* Todos from most important to least important */
// TODO: continue with learnopengl
// TODO: improve cooldown system

// Global variables
float changeColorLastTime = 0.0f;

// To make the window resizable
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, shader shaderProgram) {
 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
  glfwSetWindowShouldClose(window, true);
 }
 if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && changeColorLastTime < glfwGetTime() - 0.5) {
  shaderProgram.setBool("useInColor", shaderProgram.getBool("useInColor") ^ 1); // Toggles it
  changeColorLastTime = glfwGetTime();
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

 if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
  std::cout << "failed to initialize GLAD\n";
  return NULL;
 }

 glViewport(0, 0, windowWidth, windowHeight);
 // Making it do that function whenever the window gets resized
 glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

 return window;
}

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

const uint indices[] = {
};

int main() {
 std::cout << "\n";
 GLFWwindow* window = initEngine(WINDOW_WIDTH, WINDOW_HEIGHT, (char*)"engine");

 uint texture;
 glGenTextures(1, &texture);
 glActiveTexture(GL_TEXTURE0);
 glBindTexture(GL_TEXTURE_2D, texture);

 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

 int textureWidth, textureHeight, nrChannels;
 unsigned char* textureData = stbi_load("textures/crate.png", &textureWidth, &textureHeight, &nrChannels, 0);
 if (textureData) {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
  glGenerateMipmap(GL_TEXTURE_2D);
 }
 else {
  std::cout << "Failed to load texture\n";
 }
 stbi_image_free(textureData);

 // VAO stores stuff like vertexattribpointers
 uint VAO;
 glGenVertexArrays(1, &VAO);
 glBindVertexArray(VAO);

 // EBO stores indices to not have multiple of the same vertices
 uint EBO;
 glGenBuffers(1, &EBO);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

 uint VBO;
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

 shaderProgram.setInt("inTexture", 0);

 shaderProgram.setBool("useInColor", false);

 glEnable(GL_DEPTH_TEST);

 glBindVertexArray(VAO);
 glBindTexture(GL_TEXTURE_2D, texture);
 while (!glfwWindowShouldClose(window)) {
  processInput(window, shaderProgram);

  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram.use();

  float time = glfwGetTime();
  float greenValue = (sin(time) / 2.0f) + 0.5f;
  glUniform4f(glGetUniformLocation(shaderProgram.ID, "inColor"), 0.0f, greenValue, 0.0f, 1.0f);

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  glm::mat4 view(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

  for (uint i = 0; i < 10; i++) {
   glm::mat4 model(1.0f);
   model = glm::translate(model, cubePositions[i] + glm::vec3(sin(glfwGetTime()) * 3, 0.0f, 0.0f));
   model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.5f, 0.0f));
   glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

   glDrawArrays(GL_TRIANGLES, 0 , 36);
  }


  // Commented, cause for testing purposes indices arent being used
  // glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(float), GL_UNSIGNED_INT, 0);

  glfwSwapBuffers(window);
  glfwPollEvents();
 }

 glfwTerminate();
 return 0;
}

#include <iostream>
#include <cmath>
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"
#include "../include/stb_image.h"
#include "shader.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// To make the window resizable
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, shader shaderProgram) {
 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
  glfwSetWindowShouldClose(window, true);
 }
 if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
  shaderProgram.setBool("useInColor", shaderProgram.getBool("useInColor") ^ 1); // Toggles it
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
-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // Bottom left
-0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, // Top left
0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // Bottom right
0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f // Top right
};

const uint indices[] = {
0, 1, 2,
1, 2, 3
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
 unsigned char* textureData = stbi_load("textures/wood.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
 if (textureData) {
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
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
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
 glEnableVertexAttribArray(0);
 // Color attribute
 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
 glEnableVertexAttribArray(1);
 // Texture coordinates attribute
 glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
 glEnableVertexAttribArray(2);

 shader shaderProgram("shaders/vertex.sha", "shaders/fragment.sha");

 shaderProgram.use();

 shaderProgram.setInt("inTexture", 0);
 shaderProgram.setBool("useInTexture", true);

 shaderProgram.setBool("useInColor", false);

 while (!glfwWindowShouldClose(window)) {
  processInput(window, shaderProgram);

  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shaderProgram.use();

  float time = glfwGetTime();
  float greenValue = (sin(time) / 2.0f) + 0.5f;
  glUniform4f(glGetUniformLocation(shaderProgram.ID, "inColor"), 0.0f, greenValue, 0.0f, 1.0f);

  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(float), GL_UNSIGNED_INT, 0);

  glfwSwapBuffers(window);
  glfwPollEvents();
 }

 glfwTerminate();
 return 0;
}

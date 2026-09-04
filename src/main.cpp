#include <iostream>
#include <cmath>
#include "../include/glad/glad.h"
#include "../include/GLFW/glfw3.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"

"out vec3 color;\n"

"void main() {\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
" color = aColor;\n"
"}";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 color;\n"

"uniform vec4 inColor;\n"

"void main() {\n"
" if (inColor == vec4(0.0f, 0.0f, 0.0f, 0.0f)) {\n"
"  FragColor = vec4(color, 1.0);\n"
" }\n"
" else {\n"
"  FragColor = inColor;\n"
" }\n"
"}";

// To make the window resizable
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
 if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
  glfwSetWindowShouldClose(window, true);
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
-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // Bottom left
-0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // Top left
0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // Bottom right
0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 1.0f // Top right
};

const uint indices[] = {
0, 1, 2,
1, 2, 3
};

int main() {
 GLFWwindow* window = initEngine(WINDOW_WIDTH, WINDOW_HEIGHT, (char*)"engine");

 // VAO stores stuff like vertexattribpointers
 uint VAO;
 glGenVertexArrays(1, &VAO);
 glBindVertexArray(VAO);

 // EBO stores indices to not have multiple of the same vertices
 uint EBO;
 glGenBuffers(1, &EBO);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
 glBindBuffer(1, 0); // Unbinding the buffer

 uint VBO;
 glGenBuffers(1, &VBO);
 glBindBuffer(GL_ARRAY_BUFFER, VBO);
 glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

 // Position attribute
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
 glEnableVertexAttribArray(0);
 // Color attribute
 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
 glEnableVertexAttribArray(1);

 uint vertexShader;
 vertexShader = glCreateShader(GL_VERTEX_SHADER);
 glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
 glCompileShader(vertexShader);

 int success;
 glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
 if (!success) {
  char infolog[512];
  glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
  std::cout << "Vertex shader compilation error:\n" << infolog;
  return -1;
 }

 uint fragmentShader;
 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
 glCompileShader(fragmentShader);

 glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
 if (!success) {
  char infolog[512];
  glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
  std::cout << "Fragment shader compilation error:\n" << infolog;
  return -1;
 }

 uint shaderProgram;
 shaderProgram = glCreateProgram();
 glAttachShader(shaderProgram, vertexShader);
 glAttachShader(shaderProgram, fragmentShader);
 glLinkProgram(shaderProgram);

 glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
 if (!success) {
  char infolog[512];
  glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
  std::cout << "Shader program linking error:\n" << infolog;
  return -1;
 }



 glUseProgram(shaderProgram);
 glBindVertexArray(VAO);
 glBindBuffer(1, EBO);
 glDeleteShader(vertexShader);
 glDeleteShader(fragmentShader);



 while (!glfwWindowShouldClose(window)) {
  processInput(window);

  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(shaderProgram);

  float time = glfwGetTime();
  float greenValue = (sin(time) / 2.0f) + 0.5f;
  //glUniform4f(glGetUniformLocation(shaderProgram, "inColor"), 0.0f, greenValue, 0.0f, 1.0f);

  glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(float), GL_UNSIGNED_INT, 0);

  glfwSwapBuffers(window);
  glfwPollEvents();
 }

 glfwTerminate();
 return 0;
}

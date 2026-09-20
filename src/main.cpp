#include <iostream>
#include <cmath>
#include <random>

#include "../include/glad/glad.h"
#ifdef _WIN32
 #include "../include/GLFW/windows/glfw3.h"
#else
 #include "../include/GLFW/linux/glfw3.h"
#endif

#include "../include/stb_image.h"

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "engine.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

/* Todos from most important to least important */
// TODO: continue with learnopengl
// TODO: improve cooldown system

// Global variables

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

glm::vec3 cubeMovementGoals[10];

int cubesADHD = 2;

/*const unsigned int indices[] = {
};*/

int main() {
 srand((unsigned int)time(0));

 std::cout << "\n";
 GLFWwindow* window = engine::initEngine(WINDOW_WIDTH, WINDOW_HEIGHT, (char*)"engine");

 texture crate("textures/crate.png", 0);

 // VAO stores stuff like vertexattribpointers
 VAO crateVAO;
 crateVAO.bind();

 VBO crateVBO(vertices, sizeof(vertices), GL_STATIC_DRAW);
 crateVBO.bind();

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

 crateVAO.bind();
 crate.bind2D();

 double timeSinceLastSecond = 0.0;

 for (unsigned int i = 0; i < 10; i++) {
  cubeMovementGoals[i] = glm::vec3(engine::random(-(cubesADHD / 2), cubesADHD / 2), engine::random(-(cubesADHD / 2), cubesADHD / 2), engine::random(-(cubesADHD / 2), cubesADHD / 2));
 }

 while (!glfwWindowShouldClose(window)) {
  float currentFrame = glfwGetTime();
  engine::deltaTime = currentFrame - engine::lastFrame;

  engine::processInput(window, shaderProgram);

  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram.use();

  float time = glfwGetTime();
  float greenValue = (sin(time) / 2.0f) + 0.5f;
  glUniform4f(glGetUniformLocation(shaderProgram.ID, "inColor"), 0.0f, greenValue, 0.0f, 1.0f);

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(engine::FOV), 800.0f / 600.0f, 0.1f, 100.0f);
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  glm::mat4 view;
  view = engine::cam.getViewMatrix();
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

  for (unsigned int i = 0; i < 10; i++) {
   cubePositions[i] += cubeMovementGoals[i] * engine::deltaTime;

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

  timeSinceLastSecond += engine::deltaTime;
  if (timeSinceLastSecond > 1.0) {
   std::cout << "FPS: " << 1 / engine::deltaTime << "\n";
   std::cout << "frameTime: " << engine::deltaTime << "\n";
   timeSinceLastSecond -= 1.0;

   for (unsigned int i = 0; i < 10; i++) {
    cubeMovementGoals[i] = glm::vec3(engine::random(-(cubesADHD / 2), cubesADHD / 2), engine::random(-(cubesADHD / 2), cubesADHD / 2), engine::random(-(cubesADHD / 2), cubesADHD / 2));
   }
  }
  engine::lastFrame = currentFrame;
 }

 glfwTerminate();
 return 0;
}

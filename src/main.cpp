#include <iostream>
#include <cmath>

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

// Global variables

const float vertices[] = {
// Back face
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

// Front face
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

// Left face
-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,

// Right face
0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

// Bottom face
-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
-0.5f, -0.5f,  0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
-0.5f, -0.5f, -0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f,

// Top face
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f
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

glm::vec3 lightPos(0.0f, 1.0f, -5.0f);

glm::vec3 cubeMovementGoals[10];

int cubesADHD = 0;

/*const unsigned int indices[] = {
};*/

int main() {
 srand((unsigned int)time(0));

 std::cout << "\n";
 GLFWwindow* window = engine::initEngine(WINDOW_WIDTH, WINDOW_HEIGHT, (char*)"engine");

 texture crate("textures/crate2.png", 0);
 texture crateSpecular("textures/crate2Specular.png", 1);

 VAO crateVAO;
 crateVAO.bind();

 VBO crateVBO(vertices, sizeof(vertices), GL_STATIC_DRAW);
 crateVBO.bind();

 // Position attribute
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
 glEnableVertexAttribArray(0);
 // Normal attribute
 glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
 glEnableVertexAttribArray(1);
 // Texture coordinates attribute
 glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
 glEnableVertexAttribArray(2);

 VAO lightVAO;
 lightVAO.bind();

 // Position attribute
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
 glEnableVertexAttribArray(0);

 shader lightingShader("shaders/lighting.vert", "shaders/lighting.frag");
 shader lightSourceShader("shaders/lightSource.vert", "shaders/lightSource.frag");

 glEnable(GL_DEPTH_TEST);

 crate.bind2D();
 crateSpecular.bind2D();

 double timeSinceLastSecond = 0.0;

 for (unsigned int i = 0; i < 10; i++) {
  cubeMovementGoals[i] = glm::vec3(engine::random(-(cubesADHD / 2), cubesADHD / 2), engine::random(-(cubesADHD / 2), cubesADHD / 2), engine::random(-(cubesADHD / 2), cubesADHD / 2));
 }

 while (!glfwWindowShouldClose(window)) {
  float currentFrame = glfwGetTime();
  engine::deltaTime = currentFrame - engine::lastFrame;

  engine::processInput(window);

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::vec3 movingLightPos = lightPos + glm::vec3(sin(glfwGetTime()) * 3, 0.0f, 0.0f);
  glm::vec3 lightColor(
   sin(glfwGetTime() * 0.43f) * 0.5f + 0.5f,
   sin(glfwGetTime() * 0.1f) * 0.5f + 0.5f,
   sin(glfwGetTime() * 0.63f) * 0.5f + 0.5f
  );

  lightingShader.use();
  lightingShader.setVec3("viewPos", engine::cam.position);
  lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
  lightingShader.setInt("material.diffuse", 0);
  lightingShader.setInt("material.specular", 1);
  lightingShader.setFloat("material.shininess", 64.0f);
  lightingShader.setVec3("light.position", movingLightPos);
  lightingShader.setVec3("light.ambient",  lightColor * glm::vec3(0.2f));
  lightingShader.setVec3("light.diffuse",  lightColor * glm::vec3(0.5f));
  lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
  lightingShader.setFloat("light.maxRange", 20.0f);

  crateVAO.bind();

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(engine::FOV), 800.0f / 600.0f, 0.1f, 100.0f);
  glUniformMatrix4fv(glGetUniformLocation(lightingShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  glm::mat4 view;
  view = engine::cam.getViewMatrix();
  glUniformMatrix4fv(glGetUniformLocation(lightingShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

  for (unsigned int i = 0; i < 10; i++) {
   cubePositions[i] += cubeMovementGoals[i] * engine::deltaTime;

   glm::mat4 model(1.0f);
   model = glm::translate(model, cubePositions[i]);
   glUniformMatrix4fv(glGetUniformLocation(lightingShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

   glDrawArrays(GL_TRIANGLES, 0 , 36);
  }

  lightSourceShader.use();
  lightSourceShader.setVec3("color", lightColor);
  lightVAO.bind();

  glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
  glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
  glm::mat4 model(1.0f);
  model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
  model = glm::translate(model, movingLightPos);
  glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

  glDrawArrays(GL_TRIANGLES, 0, 36);

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

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
glm::vec3(0.0f, 0.0f, 0.0f),
glm::vec3(2.0f, 5.0f, -15.0f),
glm::vec3(-1.5f, -2.2f, -2.5f),
glm::vec3(-3.8f, -2.0f, -12.3f),
glm::vec3(2.4f, -0.4f, -3.5f),
glm::vec3(-1.7f, 3.0f, -7.5f),
glm::vec3(1.3f, -2.0f, -2.5f),
glm::vec3(1.5f, 2.0f, -2.5f),
glm::vec3(1.5f, 0.2f, -1.5f),
glm::vec3(-1.3f, 1.0f, -1.5f),
glm::vec3(0.0f, -6.0f, 0.0f)
};


glm::vec3 pointLightPositions[] = {
glm::vec3(0.7f, -0.2f, 2.0f),
glm::vec3(2.3f, 3.3f, -4.0f),
glm::vec3(-4.0f, 4.0f, -8.0f),
glm::vec3(0.0f, 2.0f, -3.0f)
};

glm::vec3 cubeMovementGoals[11];

int cubesADHD = 0; // NOTE: a value of 1 doesn't do stuff cause it's programmed badly by me

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

 for (unsigned int i = 0; i < 11; i++) {
  cubeMovementGoals[i] = glm::vec3(engine::random(-(cubesADHD / 2), cubesADHD / 2), engine::random(-(cubesADHD / 2), cubesADHD / 2), engine::random(-(cubesADHD / 2), cubesADHD / 2));
 }

 lightingShader.use();

 lightingShader.setBool("ignoreSpotLight", true);

 while (!glfwWindowShouldClose(window)) {
  float currentFrame = glfwGetTime();
  engine::deltaTime = currentFrame - engine::lastFrame;

  lightingShader.use();

  engine::processInput(window, &lightingShader);

  glClearColor(
   0.529f,
   0.808f,
   0.922f,
   1.0f
  );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::vec3 lightColor(
   sin(glfwGetTime() * 0.43f) * 0.5f + 0.5f,
   sin(glfwGetTime() * 0.1f) * 0.5f + 0.5f,
   sin(glfwGetTime() * 0.63f) * 0.5f + 0.5f
  );


  lightingShader.setVec3("directionalLight.direction", 0.2f, -1.0f, 0.3f);
  lightingShader.setVec3("directionalLight.ambient", 0.1f, 0.1f, 0.1f);
  lightingShader.setVec3("directionalLight.diffuse", 0.7f, 0.7f, 0.7f);
  lightingShader.setVec3("directionalLight.specular", 0.5f, 0.5f, 0.5f);

  lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
  lightingShader.setVec3("pointLights[0].ambient", 0.15f, 0.15f, 0.15f);
  lightingShader.setVec3("pointLights[0].diffuse", lightColor * 0.8f);
  lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
  lightingShader.setFloat("pointLights[0].constant", 1.0f);
  lightingShader.setFloat("pointLights[0].linear", 0.09f);
  lightingShader.setFloat("pointLights[0].quadratic", 0.032f);

  lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
  lightingShader.setVec3("pointLights[1].ambient", 0.15f, 0.15f, 0.15f);
  lightingShader.setVec3("pointLights[1].diffuse", lightColor * 0.8f);
  lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
  lightingShader.setFloat("pointLights[1].constant", 1.0f);
  lightingShader.setFloat("pointLights[1].linear", 0.09f);
  lightingShader.setFloat("pointLights[1].quadratic", 0.032f);

  lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
  lightingShader.setVec3("pointLights[2].ambient", 0.15f, 0.15f, 0.15f);
  lightingShader.setVec3("pointLights[2].diffuse", lightColor * 0.8f);
  lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
  lightingShader.setFloat("pointLights[2].constant", 1.0f);
  lightingShader.setFloat("pointLights[2].linear", 0.09f);
  lightingShader.setFloat("pointLights[2].quadratic", 0.032f);

  lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
  lightingShader.setVec3("pointLights[3].ambient", 0.15f, 0.15f, 0.15f);
  lightingShader.setVec3("pointLights[3].diffuse", lightColor * 0.8f);
  lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
  lightingShader.setFloat("pointLights[3].constant", 1.0f);
  lightingShader.setFloat("pointLights[3].linear", 0.09f);
  lightingShader.setFloat("pointLights[3].quadratic", 0.032f);

  lightingShader.setVec3("spotLight.position", engine::cam.position);
  lightingShader.setVec3("spotLight.direction", engine::cam.front);
  lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
  lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
  lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
  lightingShader.setFloat("spotLight.constant", 1.0f);
  lightingShader.setFloat("spotLight.linear", 0.09f);
  lightingShader.setFloat("spotLight.quadratic", 0.032f);
  lightingShader.setFloat("spotLight.innerCutOff", glm::cos(glm::radians(12.5f)));
  lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

  lightingShader.setVec3("viewPos", engine::cam.position);
  lightingShader.setInt("material.diffuse", 0);
  lightingShader.setInt("material.specular", 1);
  lightingShader.setFloat("material.shininess", 32.0f);

  crateVAO.bind();

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(engine::FOV), 800.0f / 600.0f, 0.1f, 100.0f);
  glUniformMatrix4fv(glGetUniformLocation(lightingShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  glm::mat4 view;
  view = engine::cam.getViewMatrix();
  glUniformMatrix4fv(glGetUniformLocation(lightingShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

  for (unsigned int i = 0; i < 11; i++) {
   cubePositions[i] += cubeMovementGoals[i] * engine::deltaTime;

   glm::mat4 model(1.0f);
   model = glm::translate(model, cubePositions[i]);
   if (i == 10) {
    model = glm::scale(model, glm::vec3(30.0f, 5.0f, 30.0f));
   }
   glUniformMatrix4fv(glGetUniformLocation(lightingShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

   glDrawArrays(GL_TRIANGLES, 0 , 36);
  }

  lightSourceShader.use();
  lightSourceShader.setVec3("color", lightColor);
  lightVAO.bind();

  glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
  glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

  for (unsigned int i = 0; i < sizeof(pointLightPositions) / sizeof(glm::vec3); i++) {
   glm::mat4 model(1.0f);
   model = glm::translate(model, glm::vec3(pointLightPositions[i]));
   model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
   glUniformMatrix4fv(glGetUniformLocation(lightSourceShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

   glDrawArrays(GL_TRIANGLES, 0, 36);
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

   for (unsigned int i = 0; i < 11; i++) {
    cubeMovementGoals[i] = glm::vec3(engine::random(-(cubesADHD / 2), cubesADHD / 2), engine::random(-(cubesADHD / 2), cubesADHD / 2), engine::random(-(cubesADHD / 2), cubesADHD / 2));
   }
  }
  engine::lastFrame = currentFrame;
 }

 glfwTerminate();
 return 0;
}

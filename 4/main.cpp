#include "../Utils.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ, pyrLocX, pyrLocY, pyrLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
// allocate variables used in display() function, so that they won’t need to be
// allocated during rendering
GLuint mvLoc, projLoc, widthLoc, heightLoc, tfLoc;
int width, height;
float aspect, timeFactor;
glm::mat4 pMat, rMat, tMat, vMat, mMat, mvMat;
void setupVertices(
    void) { // 36 vertices, 12 triangles, makes 2x2x2 cube placed at origin
  float cubePositions[108] = {

      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f,
      -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
      1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f};

  // pyramid with 18 vertices, comprising 6 triangles (four sides, and two on
  // the bottom)
  float pyramidPositions[54] = {
      -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
      1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
      -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,
      1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f};
  /* Vertex attributes are values that are interpolated across a triangle from
   * the 3 points that make a triangle*/
  glGenVertexArrays(numVAOs, vao); // generate vertex array attribute
  glBindVertexArray(vao[0]);       // bind a vao to the current opengl context
  glGenBuffers(numVBOs, vbo); // generate vertex buffer object (vbo will hold
                              // IDs (GLuint) to the buffer)
  glBindBuffer(GL_ARRAY_BUFFER,
               vbo[0]); // bind buffer object to the current OpenGL context.
                        // Once bound, any operations performed on the target
                        // buffer will affect the currently bound object
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions,
               GL_STATIC_DRAW); // actually fill buffer data

  glBindBuffer(GL_ARRAY_BUFFER,
               vbo[1]); // bind buffer object to the current OpenGL context.
                        // Once bound, any operations performed on the target
                        // buffer will affect the currently bound object
  glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions,
               GL_STATIC_DRAW); // actually fill buffer data
}

void init(GLFWwindow *window) {
  renderingProgram =
      Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
  cameraX = 0.0f;
  cameraY = 0.0f;
  cameraZ = 8.0f;
  cubeLocX = 0.0f;
  // shift down Y to reveal perspective
  cubeLocY = -2.0f;
  cubeLocZ = 0.0f;
  pyrLocX = 1.0f;
  pyrLocY = 2.5f;
  pyrLocZ = 0.0f;
  setupVertices();
}

void display(GLFWwindow *window, double currentTime) {
  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);
  // enables openGL calls to determine shader's VAs and uniform locations
  glUseProgram(renderingProgram); // installs glsl code on the GPU

  // get the uniform variables for the MV and projection matrices
  mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
  projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
  // widthLoc = glGetUniformLocation(renderingProgram, "windowWidth");
  // heightLoc = glGetUniformLocation(renderingProgram, "windowHeight");
  // build perspective matrix
  glfwGetFramebufferSize(window, &width, &height);
  aspect = (float)width / (float)height;
  pMat = glm::perspective(1.0472f, aspect, 0.1f,
                          1000.0f); // 1.0472 radians = 60 degrees
  // build view matrix, model matrix, and model-view matrix
  vMat =
      glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

  mMat =
      glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocX, cubeLocY, cubeLocZ));
  mvMat = vMat * mMat;
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  // draw the pyramid (use buffer #1)
  mMat = glm::translate(glm::mat4(1.0f), glm::vec3(pyrLocX, pyrLocY, pyrLocZ));
  mvMat = vMat * mMat;
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDrawArrays(GL_TRIANGLES, 0, 18);
}

int main(void) {
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  GLFWwindow *window =
      glfwCreateWindow(600, 600, "Chapter 4 - program 1", NULL, NULL);
  glfwMakeContextCurrent(window);
  if (glewInit() != GLEW_OK) {
    exit(EXIT_FAILURE);
  }
  glfwSwapInterval(1);
  init(window);
  while (!glfwWindowShouldClose(window)) {
    display(window, glfwGetTime());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
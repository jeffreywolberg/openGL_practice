#include "../Utils.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <fstream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <soil2/SOIL2.h>
#include <stack>
#include <string>

using namespace std;
using namespace glm;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float pyrLocX, pyrLocY, pyrLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
// allocate variables used in display() function, so that they won’t need to be
// allocated during rendering
GLuint mvLoc, projLoc, widthLoc, heightLoc, tfLoc, brickTexture;
int width, height;
float aspect, timeFactor;
mat4 pMat, rMat, tMat, vMat, mMat, mvMat;
stack<mat4> mvStack;

void setupVertices(
    void) {
  float pyramidPositions[54] = {
      -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
      1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
      -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,
      1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f};

  float pyrTexCoords[36] = {
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
      0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
      0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f};
  /* Vertex attributes are values that are interpolated across a triangle from
   * the 3 points that make a triangle*/
  glGenVertexArrays(numVAOs, vao); // generate vertex array attribute
  glBindVertexArray(vao[0]);       // bind a vao to the current opengl context
  glGenBuffers(numVBOs, vbo); // generate vertex buffer object (vbo will hold
                              // IDs (GLuint) to the buffer)

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions,
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pyrTexCoords), pyrTexCoords,
               GL_STATIC_DRAW);
}

void init(GLFWwindow *window) {
  renderingProgram =
      Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");
  cameraX = 0.0f;
  cameraY = 2.0f;
  cameraZ = 2.0f;
  pyrLocX = 0.0f;
  pyrLocY = 0.0f;
  pyrLocZ = 0.0f;

  brickTexture = Utils::loadTexture("brick1_resized.jpg");
  setupVertices();

  glfwGetFramebufferSize(window, &width, &height);
  aspect = (float)width / (float)height;
  pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

void display(GLFWwindow *window, double currentTime) {
  glClear(GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT);
  // enables openGL calls to determine shader's VAs and uniform locations
  glUseProgram(renderingProgram); // installs glsl code on the GPU

  // get the uniform variables for the MV and projection matrices
  mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
  projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(pMat));
  // build view matrix, model matrix, and model-view matrix
  mMat = translate(mat4(1.0f), vec3(pyrLocX, pyrLocY, pyrLocZ));
  vMat = rotate(mat4(1.0f), (1.0f/4.0f) * 3.14f, vec3(1.0f,0.0f,0.0f));
  vMat *= translate(mat4(1.0f), vec3(-cameraX, -cameraY, -cameraZ));
  // vMat = translate(mat4(1.0f), vec3(-cameraX, -cameraY, -cameraZ));

  mvMat = mMat * vMat;
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, value_ptr(mvMat));

  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, brickTexture);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDrawArrays(GL_TRIANGLES, 0, 18);
}

void window_reshape_callback(GLFWwindow *window, int newWidth, int newHeight) {
  cout << "Callback, w: " << newWidth << ", h: " << newHeight << endl;
  aspect = (float)newWidth /
           (float)newHeight; // new width&height provided by the callback
  glViewport(0, 0, newWidth, newHeight); // sets screen
  // region associated with framebuffer pMat =
  glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void) {
  if (!glfwInit()) {
    exit(EXIT_FAILURE);
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  GLFWwindow *window = glfwCreateWindow(600, 600, "Chapter 4", NULL, NULL);
  glfwMakeContextCurrent(window);
  if (glewInit() != GLEW_OK) {
    exit(EXIT_FAILURE);
  }
  glfwSwapInterval(1);
  glfwSetWindowSizeCallback(window, window_reshape_callback);
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
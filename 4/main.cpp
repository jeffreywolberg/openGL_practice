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
#include <stack>
#include <string>

using namespace std;
using namespace glm;

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
mat4 pMat, rMat, tMat, vMat, mMat, mvMat;
stack<mat4> mvStack;

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
  cameraZ = 15.0f;
  cubeLocX = 0.0f;
  // shift down Y to reveal perspective
  cubeLocY = -2.0f;
  cubeLocZ = 0.0f;
  pyrLocX = 1.0f;
  pyrLocY = 2.5f;
  pyrLocZ = 0.0f;
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
  glEnable(GL_CULL_FACE);
  // glCullFace(GL_FRONT); // culling the front facing triangles (don't do this, just for testing purposes)

  // get the uniform variables for the MV and projection matrices
  mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
  projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
  // widthLoc = glGetUniformLocation(renderingProgram, "windowWidth");
  // heightLoc = glGetUniformLocation(renderingProgram, "windowHeight");

  glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(pMat));
  // build view matrix, model matrix, and model-view matrix
  vMat = translate(mat4(1.0f), vec3(-cameraX, -cameraY, -cameraZ));

  mvStack.push(vMat);

  // pyramid = sun
  mvStack.push(mvStack.top());
  mvStack.top() *= translate(mat4(1.0f), vec3(0.0f));
  mvStack.push(mvStack.top());
  mvStack.top() *=
      rotate(mat4(1.0f), float(currentTime), vec3(1.0f, 0.0f, 0.0f));

  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, value_ptr(mvStack.top()));
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LEQUAL);
  glFrontFace(GL_CCW);
  glDrawArrays(GL_TRIANGLES, 0, 18); // draw the sun
  mvStack.pop(); // remove the sun’s axial rotation from the stack
  //----------------------- cube == planet
  //---------------------------------------------
  mvStack.push(mvStack.top());
  mvStack.top() *= glm::translate(glm::mat4(1.0f),
                                  glm::vec3(sin((float)currentTime) * 4.0, 0.0f,
                                            cos((float)currentTime) * 4.0));
  mvStack.push(mvStack.top());
  mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime,
                               glm::vec3(0.0, 1.0, 0.0));
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  // planet rotation
  glFrontFace(GL_CW);
  glDrawArrays(GL_TRIANGLES, 0, 36); // draw the planet
  mvStack.pop(); // remove the planet’s axial rotation from the stack
  //----------------------- smaller cube == moon
  //-----------------------------------
  mvStack.push(mvStack.top());
  mvStack.top() *= glm::translate(glm::mat4(1.0f),
                                  glm::vec3(0.0f, sin((float)currentTime) * 2.0,
                                            cos((float)currentTime) * 2.0));
  mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime,
                               glm::vec3(0.0, 0.0, 1.0));
  // moon rotation
  mvStack.top() *= glm::scale(
      glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f)); // make the moon smaller
  glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  glFrontFace(GL_CW);
  glDrawArrays(GL_TRIANGLES, 0, 36); // draw the moon
  // remove moon scale/rotation/position, planet position, sun position, and
  // view matrices from stack
  mvStack.pop();
  mvStack.pop();
  mvStack.pop();
  mvStack.pop();
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
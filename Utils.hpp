#ifndef UTILS_H
#define UTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Utils {
public: 
    static void printShaderLog(GLuint shader);
    static void printProgramLog(int prog);
    static bool checkOpenGLError();
    static string readShaderSource(const char *filePath);
    static GLuint createShaderProgram(const char *vsource, const char *fsource);
    static GLuint loadTexture(const char *texImagePath);
};

#endif
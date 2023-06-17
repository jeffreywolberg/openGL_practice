#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

void init(GLFWwindow *window) {}
void display(GLFWwindow *window, double currentTime)
{
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}
int main(void)
{
    cout << "1" << endl;
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    cout << "2" << endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    cout << "3" << endl;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    cout << "4" << endl;
    GLFWwindow *window = glfwCreateWindow(600, 600, "Chapter2 - program1", NULL, NULL);
    cout << "5" << endl;
    glfwMakeContextCurrent(window);
    cout << "6" << endl;
#ifndef __APPLE__
    if (glewInit() != GLEW_OK)
    {
        cout << "6.5" << endl;
        exit(EXIT_FAILURE);
    }
#endif
#ifdef __APPLE__
cout << "Trying some apple config!" << endl;
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    cout << "7" << endl;
    glfwSwapInterval(1);
    cout << "8" << endl;
    init(window);
    cout << "9" << endl;
    while (!glfwWindowShouldClose(window))
    {
    cout << "9.25" << endl;
        display(window, glfwGetTime());
    cout << "9.5" << endl;
        glfwSwapBuffers(window);
    cout << "9.75" << endl;
        glfwPollEvents();
    }
    cout << "10" << endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
    cout << "11" << endl;
}
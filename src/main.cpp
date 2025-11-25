#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "init/init_gl.h"

void processInput(GLFWwindow *window);

int main() {
    
    GLFWwindow *window { initOpenGL() };

    if (window == nullptr)
        return EXIT_FAILURE;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

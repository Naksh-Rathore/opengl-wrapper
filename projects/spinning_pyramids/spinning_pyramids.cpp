#include <iostream>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

const char *vertShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view; 

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
}
)";

const char *fragShaderSource = R"(
#version 330 core

out vec4 FragColor;

in vec3 ourColor;

void main() {
    FragColor = vec4(ourColor, 1.0f);
}
)";


void processInput(GLFWwindow*window);
void framebufferSizeCallback(GLFWwindow *window, int width, int height);

int main() {
    

    if (!glfwInit()) {
        std::cerr << "GLFW failed to initialize!\n";
        return EXIT_FAILURE;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window { glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", nullptr, nullptr) };

    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);


    GLfloat vertices[] = {
        // ----- Side 1 (Red) -----
        0.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, -1.0f,   1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, -1.0f,   1.0f, 0.0f, 0.0f,

        // ----- Side 2 (Green) -----
        0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,
        1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,

        // ----- Side 3 (Blue) -----
        0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
        1.0f, 0.0f,  1.0f,   0.0f, 0.0f, 1.0f,
        -1.0f, 0.0f,  1.0f,   0.0f, 0.0f, 1.0f,

        // ----- Side 4 (Yellow) -----
        0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f,
        -1.0f, 0.0f,  1.0f,   1.0f, 1.0f, 0.0f,
        -1.0f, 0.0f, -1.0f,   1.0f, 1.0f, 0.0f,

        // ----- Base Triangle 1 (Magenta) -----
        -1.0f, 0.0f, -1.0f,   1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, -1.0f,   1.0f, 0.0f, 1.0f,
        1.0f, 0.0f,  1.0f,   1.0f, 0.0f, 1.0f,

        // ----- Base Triangle 2 (Cyan) -----
        -1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 1.0f,
        1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 1.0f,
        -1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 1.0f,
    };

    std::array<glm::vec3, 2> cubePositions {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.1f, 0.0f)
    };

    GLuint VBO, VAO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    GLuint vert { glCreateShader(GL_VERTEX_SHADER) };
    GLuint frag { glCreateShader(GL_FRAGMENT_SHADER) };

    glShaderSource(vert, 1, &vertShaderSource, nullptr);
    glCompileShader(vert);

    glShaderSource(frag, 1, &fragShaderSource, nullptr);
    glCompileShader(frag);

    GLuint shaderProg { glCreateProgram() };

    glAttachShader(shaderProg, vert);
    glAttachShader(shaderProg, frag);

    glLinkProgram(shaderProg);

    glDeleteShader(vert);
    glDeleteShader(frag);

    glUseProgram(shaderProg);

    glEnable(GL_DEPTH_TEST);  

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);

        glUseProgram(shaderProg);

        glm::mat4 view { glm::mat4(1.0f) };
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

        glm::mat4 projection { glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f) };

        glUniformMatrix4fv(glGetUniformLocation(shaderProg, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProg, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        for (int i = 0; i < cubePositions.size(); i++) {
            glm::mat4 model { glm::mat4(1.0f) };

            if (i == 1) {
                model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
                model = glm::rotate(model, -((float) glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
            }

            else {
                model = glm::rotate(model, (float) glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            }

            model = glm::translate(model, cubePositions.at(i));

            glUniformMatrix4fv(glGetUniformLocation(shaderProg, "model"), 1, GL_FALSE, glm::value_ptr(model));

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 18);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProg);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

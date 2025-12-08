#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

const char *vertShaderSource {
"#version 330 core\n"
"\n"
"layout (location = 0) in vec3 aPos;\n"
"\n"
"uniform mat4 trans;\n"
"\n"
"void main() {\n"
"    gl_Position = trans * vec4(aPos, 1.0f);\n"
"}\n"
};

const char *fragShaderSource {
"#version 330 core\n"
"\n"
"out vec4 FragColor;\n"
"\n"
"void main() {\n"
"    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n"
};

constexpr float GRAVITY { -3.0f };
constexpr float FRICTION { 0.4f };
constexpr float RESTITUTION { -0.75f };

struct Circle {
    std::vector<GLfloat> vertices{};

    float radius{};
    int segments{};

    glm::mat4 trans{};

    glm::vec2 position{};
    glm::vec2 velocity{};

    void genVertices() {
        if (!vertices.empty())
            return;

        for (int i = 0; i < 3; i++)
            vertices.push_back(0.0f);

        for (int i = 0; i <= segments; i++) {
            float theta = 2.0f * M_PI * i / segments;

            vertices.push_back(radius * cos(theta));
            vertices.push_back(radius * sin(theta));
            vertices.push_back(0.0f);
        }
    }

    void update(float deltaTime) {

        velocity.y += GRAVITY * deltaTime;
        position += velocity * deltaTime;

        if (position.y <= -0.75f) {
            position.y = -0.75f;
            velocity.y *= RESTITUTION;

            velocity.x -= velocity.x * FRICTION * deltaTime;
        }

        if (position.y >= 0.75f) {
            position.y = 0.75f;
            velocity.y *= RESTITUTION;
        }

        if (position.x <= -0.75f) {
            position.x = -0.75f;
            velocity.x *= RESTITUTION;
        }

        if (position.x >= 0.75f) {
            position.x = 0.75f;
            velocity.x *= RESTITUTION;
        }

        if (fabs(velocity.y) < 0.01f)
            velocity.y = 0;

        if (fabs(velocity.x) < 0.01f)
            velocity.x = 0;

        resetTrans();
        translate(position.x, position.y, 0.0f);
    }

    void scale(float x, float y, float z) { trans = glm::scale(trans, glm::vec3(x, y, z)); }
    void translate(float x, float y, float z) { trans = glm::translate(trans, glm::vec3(x, y, z)); }
    void rotate(float angle, float x, float y, float z) { trans = glm::rotate(trans, angle, glm::vec3(x, y, z)); }

    void resetTrans() { trans = glm::mat4(1.0f); }
};

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

    std::mt19937 mt { static_cast<std::mt19937::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()) };
    std::uniform_real_distribution<float> vel{ -5.0f, 5.0f };

    Circle circle { {}, 0.25f, 72, glm::mat4(1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(vel(mt), vel(mt)) };

    circle.genVertices();

    GLuint VBO, VAO;

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, circle.vertices.size() * sizeof(GLfloat), circle.vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);

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

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        glUseProgram(shaderProg);

        circle.update(0.016f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProg, "trans"), 1, GL_FALSE, glm::value_ptr(circle.trans));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 2 + circle.segments);

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


void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}


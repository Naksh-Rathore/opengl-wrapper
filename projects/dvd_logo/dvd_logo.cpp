#include <iostream>
#include <chrono>
#include <thread>
#include <array>
#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

const char *fragShaderSource { 
"#version 330 core\n"
"out vec4 FragColor;\n"
"\n"
"in vec2 TexCoord;\n"
"\n"
"uniform sampler2D ourTexture;\n"
"uniform vec3 dvdColor;\n"
"\n"
"void main()\n"
"{\n"
"    FragColor = texture(ourTexture, TexCoord) * vec4(dvdColor, 1.0f);\n"
"}\n"
};

const char *vertShaderSource { 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"\n"
"out vec2 TexCoord;\n"
"\n"
"uniform mat4 trans;\n"
"\n"
"void main()\n"
"{\n"
"    gl_Position = trans * vec4(aPos, 1.0);\n"
"    TexCoord = aTexCoord;\n"
"}\n"
};

struct DVD {
    glm::mat4 trans{};
    glm::vec3 color{};
    glm::vec2 velocity{};

    void bounce() {
        float halfWidth = 0.5f * 0.5f;  
        float halfHeight = 0.5f * 0.5f;

        if (x() + halfWidth >= 1.0f || x() - halfWidth <= -1.0f) {
            velocity.x *= -1;
            cycleColors();
        }

        if (y() + halfHeight >= 1.0f || y() - halfHeight <= -1.0f) {
            velocity.y *= -1;
            cycleColors();
        }
    }

    void cycleColors() {
        static glm::vec3 oldColor { color };

        static std::array<glm::vec3, 5> colors {
            glm::vec3(1.0f, 1.0f, 1.0f), // White
            glm::vec3(1.0f, 0.0f, 0.0f), // Red
            glm::vec3(0.0f, 0.0f, 1.0f), // Blue
            glm::vec3(0.0f, 1.0f, 0.0f), // Green
            glm::vec3(1.0f, 1.0f, 0.0f)  // Yellow
        };

        static int currentIndex = 0;
        currentIndex = (currentIndex + 1) % colors.size();
        color = colors[currentIndex];
    }

    void scale(float x, float y, float z) { trans = glm::scale(trans, glm::vec3(x, y, z)); }
    void translate(float x, float y, float z) { trans = glm::translate(trans, glm::vec3(x, y, z)); }
    void rotate(float angle, float x, float y, float z) { trans = glm::rotate(trans, angle, glm::vec3(x, y, z)); }

    GLfloat x() const { return trans[3].x; }
    GLfloat y() const { return trans[3].y; }
    GLfloat z() const { return trans[3].z; }
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

    GLfloat vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    GLuint indices[] = {  
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  

    GLuint VBO, VAO, EBO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);  

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);  

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

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    unsigned char *data { stbi_load("assets/textures/dvd-xxl.png", &width, &height, &nrChannels, 4) };

    if (data == nullptr) {
        std::cerr << "Could not load texture" << "\n";
        return EXIT_FAILURE;
    }

    GLuint texture{};

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glUseProgram(shaderProg);
    glUniform1i(glGetUniformLocation(shaderProg, "ourTexture"), 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::mt19937 mt { static_cast<std::mt19937::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()) };
    std::uniform_real_distribution<float> vel{ -0.01f, 0.01f };

    DVD dvd { glm::mat4(1.0f), glm::vec3(1.0f), glm::vec2(vel(mt), vel(mt)) }; // Velocity between 0.000f - 0.01f
    
    dvd.scale(0.5f, 0.5f, 1.0f);

    auto startTime { std::chrono::high_resolution_clock::now() };

    while (!glfwWindowShouldClose(window)) {
        auto currentTime { std::chrono::high_resolution_clock::now() };
        auto elapsedTime { std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() };

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);

        if (elapsedTime >= 5) {
            dvd.translate(dvd.velocity.x, dvd.velocity.y, 0.0f);
            dvd.bounce();

            startTime = std::chrono::high_resolution_clock::now();
        }

        glUseProgram(shaderProg);

        glUniformMatrix4fv(glGetUniformLocation(shaderProg, "trans"), 1, GL_FALSE, glm::value_ptr(dvd.trans));
        glUniform3fv(glGetUniformLocation(shaderProg, "dvdColor"), 1, glm::value_ptr(dvd.color));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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


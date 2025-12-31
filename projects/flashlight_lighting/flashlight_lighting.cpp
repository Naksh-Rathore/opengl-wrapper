#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h> 

#include "init/init_gl.h"

#include "textures/texture.h"

#include "cameras/fly_cam.h"

#include "shaders/shader.h"
#include "shaders/shader_program.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::FlyCamera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.25f);

void processInput(GLFWwindow*window);

void mouseCallBack(GLFWwindow* window, double xposIn, double yposIn);

float lastX;
float lastY;

bool firstMouse { true };

float deltaTime;
float lastFrame;

int main() {
    
    GLFWwindow *window { Init::initOpenGL() };

    if (window == nullptr)
        return EXIT_FAILURE;

    glfwSetCursorPosCallback(window, mouseCallBack);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    GLuint VBO, cubeVAO;

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // GLuint lightVAO;

    // glGenVertexArrays(1, &lightVAO);

    // glBindVertexArray(lightVAO);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void *) 0);
    // glEnableVertexAttribArray(0);

    Shader::Shader cubeVert("assets/shaders/vertex.vs", GL_VERTEX_SHADER);
    Shader::Shader cubeFrag("assets/shaders/fragment.fs", GL_FRAGMENT_SHADER);

    cubeVert.compileShader();
    cubeFrag.compileShader();

    Shader::ShaderProgram cubeShader;

    cubeShader.link(cubeVert, cubeFrag);
    
    // Shader::Shader lightVert("assets/shaders/vertex2.vs", GL_VERTEX_SHADER);
    // Shader::Shader lightFrag("assets/shaders/fragment2.fs", GL_FRAGMENT_SHADER);

    // lightVert.compileShader();
    // lightFrag.compileShader();

    // Shader::ShaderProgram lightShader;

    // lightShader.link(lightVert, lightFrag);

    // glm::vec3 lightPos { glm::vec3(1.2f, 1.0f, 2.0f) };

    glEnable(GL_DEPTH_TEST);

    Texture::Texture2D diffuseMap;
    Texture::Texture2D specularMap;

    diffuseMap.link("projects/box_map_lighting/assets/box.png" , GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
    specularMap.link("projects/box_map_lighting/assets/box_specular.png" , GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);

        cubeShader.use();

        glm::mat4 view { camera.viewMatrix() };
        glm::mat4 projection { glm::perspective(camera.zoom(), 800.0f/800.0f, 0.1f, 100.0f) };
        // glm::mat4 model { glm::mat4(1.0f) };

        glUniformMatrix4fv(glGetUniformLocation(cubeShader.shaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(cubeShader.shaderProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // glUniformMatrix4fv(glGetUniformLocation(cubeShader.shaderProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));

        glUniform3fv(glGetUniformLocation(cubeShader.shaderProgram(), "viewPos"), 1, glm::value_ptr(camera.pos()));

        glUniform3fv(glGetUniformLocation(cubeShader.shaderProgram(), "light.pos"), 1, glm::value_ptr(camera.pos()));
        glUniform3fv(glGetUniformLocation(cubeShader.shaderProgram(), "light.diffuse"), 1, glm::value_ptr(glm::vec3(0.5f)));
        glUniform3fv(glGetUniformLocation(cubeShader.shaderProgram(), "light.ambient"), 1, glm::value_ptr(glm::vec3(0.2f)));
        glUniform3fv(glGetUniformLocation(cubeShader.shaderProgram(), "light.specular"), 1, glm::value_ptr(glm::vec3(1.0f)));

        glUniform1f(glGetUniformLocation(cubeShader.shaderProgram(), "light.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(cubeShader.shaderProgram(), "light.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(cubeShader.shaderProgram(), "light.quadratic"), 0.032f);

        glUniform3fv(glGetUniformLocation(cubeShader.shaderProgram(), "light.dir"), 1, glm::value_ptr(camera.front() - 0.1f));
        glUniform1f(glGetUniformLocation(cubeShader.shaderProgram(), "light.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(cubeShader.shaderProgram(), "light.outerCutOff"), glm::cos(glm::radians(17.5f)));


        glUniform1i(glGetUniformLocation(cubeShader.shaderProgram(), "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(cubeShader.shaderProgram(), "material.specular"), 1);
        glUniform1f(glGetUniformLocation(cubeShader.shaderProgram(), "material.shininess"), 32.0f);

        glActiveTexture(GL_TEXTURE0);
        diffuseMap.use();

        glActiveTexture(GL_TEXTURE1);
        specularMap.use();

        // glBindVertexArray(cubeVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        for (int i = 0; i < 10; i++) {
            glm::mat4 model { glm::mat4(1.0f) };

            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));

            glUniformMatrix4fv(glGetUniformLocation(cubeShader.shaderProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
                
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // lightShader.use();

        // view = camera.viewMatrix();
        // projection = glm::perspective(camera.zoom(), 800.0f/800.0f, 0.1f, 100.0f);

        // glm::mat4 model = glm::mat4(1.0f);

        // model = glm::translate(model, lightPos);
        // model = glm::scale(model, glm::vec3(0.5f));

        // glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        // glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));

        // glBindVertexArray(lightVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboardInput(Camera::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboardInput(Camera::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboardInput(Camera::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboardInput(Camera::RIGHT, deltaTime);
}

void mouseCallBack(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.processMouseInput(xoffset, yoffset);
}

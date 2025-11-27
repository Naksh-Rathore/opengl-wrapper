#include "shaders/shader_program.h"
#include "shaders/shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

ShaderProgram::ShaderProgram()
    : m_shaderProgram { glCreateProgram() }
{
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_shaderProgram);
}

void ShaderProgram::link(Shader& vertexShader, Shader& fragmentShader) {
    glAttachShader(m_shaderProgram, vertexShader.shader());
    glAttachShader(m_shaderProgram, fragmentShader.shader());
    glLinkProgram(m_shaderProgram);

    glDetachShader(m_shaderProgram, vertexShader.shader());
    glDetachShader(m_shaderProgram, fragmentShader.shader());

    glDeleteShader(vertexShader.shader());
    glDeleteShader(fragmentShader.shader());
}

void ShaderProgram::use() {
    glUseProgram(m_shaderProgram);
}

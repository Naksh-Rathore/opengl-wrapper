#include <string>
#include <fstream>
#include <iostream>
#include <string_view>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders/shader.h"

Shader::Shader(const std::string& shaderSourceFile, GLenum shaderType)
    : m_shaderSourceContent { readShader(shaderSourceFile) }
    , m_shaderType { shaderType }
    , m_shader { glCreateShader(m_shaderType) }
{
}

std::string Shader::readShader(const std::string& shaderSourceFile) {
    std::string shaderContents{};
    std::string shaderContentsBuffer{};
    
    std::ifstream shaderFileStream(shaderSourceFile);

    if (!shaderFileStream.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + shaderSourceFile);
    }

    while (std::getline(shaderFileStream, shaderContentsBuffer))
        shaderContents += shaderContentsBuffer + "\n";
    std::cout << shaderContents;

    shaderFileStream.close();
    return shaderContents;
}

void Shader::compileShader() {
    const char *shaderContent { m_shaderSourceContent.c_str() };

    glShaderSource(m_shader, 1, &shaderContent, nullptr);
    glCompileShader(m_shader);

    int success{};
    char infoLog[1024]{};

    glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(m_shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type " << m_shaderType 
                << "\n" << infoLog 
                << "\n-- --------------------------------------------------- -- " << std::endl;    
    }
}

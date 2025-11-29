#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shapes/generic.h"

#include <vector>

namespace Shape {
        GenericShape2D::GenericShape2D(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices, GLenum drawMode)
        : m_vertices { vertices }
        , m_indices { indices }
        , m_drawMode { drawMode }
    {
    }

    GenericShape2D::~GenericShape2D() {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }

    void GenericShape2D::link() {
        glGenVertexArrays(1, &m_VAO);  
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);

        linkAttrib(0, 3, 3 * sizeof(GLfloat), (void *) 0);

        if (!m_indices.empty()) {
            glGenBuffers(1, &m_EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);
        }
    }

    void GenericShape2D::draw(GLuint first, GLuint count, GLenum drawMode) {
        if (drawMode == 0)
            drawMode = m_drawMode;

        glBindVertexArray(m_VAO);

        if (m_indices.empty())
            glDrawArrays(drawMode, first, count);

        else
            glDrawElements(drawMode, count, GL_UNSIGNED_INT, 0);
    }


    void GenericShape2D::addVertex(GLfloat x, GLfloat y) {
        m_vertices.push_back(x);
        m_vertices.push_back(y);
        m_vertices.push_back(0.0f);
    }

    void GenericShape2D::removeLastVertex() {
        if (m_vertices.size() >= 3) {
            m_vertices.pop_back();
            m_vertices.pop_back();
            m_vertices.pop_back();
        }
    }


    void GenericShape2D::linkAttrib(int index, int size, int stride, void *offset) {
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(index);
    }

}

#ifndef GENERIC_H
#define GENERIC_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace Shape {
    class GenericShape2D {
        protected:
            std::vector<GLfloat> m_vertices{};
            std::vector<GLuint> m_indices{};

        private:
            GLenum m_drawMode{};

            GLuint m_VBO{};
            GLuint m_VAO{};
            GLuint m_EBO{};

        public:
            GenericShape2D(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices = {}, GLenum drawMode = GL_TRIANGLE_FAN);
            virtual ~GenericShape2D();

            void link();
            void linkAttrib(int index, int size, int stride, void *offset);

            void draw(GLuint first, GLuint count, GLenum drawMode = 0);

            void addVertex(GLfloat x, GLfloat y);
            void removeLastVertex();

            inline std::vector<GLfloat> vertices() const { return m_vertices; }
            inline void setVertices(const std::vector<GLfloat>& v) { m_vertices = v; }

            inline std::vector<GLuint> indices() const { return m_indices; }
            inline void setIndices(const std::vector<GLuint>& i) { m_indices = i; }

            inline GLenum drawMode() const { return m_drawMode; }
            inline void setdrawMode(GLenum d) { m_drawMode = d; }

            inline GLuint VBO() const { return m_VBO; }
            inline GLuint VAO() const { return m_VAO; }
            inline GLuint EBO() const { return m_EBO; }
    };
}

#endif

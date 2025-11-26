#include "shapes/rectangle.h"

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

Rectangle2D::Rectangle2D(std::vector<GLfloat>& vertices, std::vector<GLuint> indices)
    : GenericShape2D(vertices, indices)
{
}

void Rectangle2D::draw() {
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

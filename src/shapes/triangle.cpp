#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "shapes/triangle.h"
#include "shapes/generic.h"

Triangle2D::Triangle2D(std::vector<GLfloat> vertices)
    : GenericShape2D(vertices, {}, GL_TRIANGLES)
{
}

void Triangle2D::draw() {
    glDrawArrays(drawMode(), 0, 3);
}

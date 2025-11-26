#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "shapes/generic.h"


class Rectangle2D : public GenericShape2D {
    public:
        Rectangle2D(std::vector<GLfloat>& vertices, std::vector<GLuint> indices);

        void draw();
};

#endif

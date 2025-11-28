#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "shapes/generic.h"

namespace Shape {
    class Triangle2D : public GenericShape2D {
        public:
            explicit Triangle2D(std::vector<GLfloat> vertices);

            void draw();
    };
}

#endif

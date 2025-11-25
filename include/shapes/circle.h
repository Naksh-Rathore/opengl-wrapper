#ifndef CIRCLE_H
#define CIRCLE_H

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "generic.h"

class Circle2D : public GenericShape2D {
    private:
        float m_radius{};
        int m_segments{};

    public:
        Circle2D(float radius = 0.0f, int segments = 0);

        void generateVertices();
        void draw();

        inline float radius() const { return m_radius; }
        inline void setRadius(float r) { m_radius = r; }

        inline int segments() const { return m_segments; }
        inline void setSegments(int s) { m_segments = s; }

};

#endif

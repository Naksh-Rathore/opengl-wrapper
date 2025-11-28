#define _USE_MATH_DEFINES
#include <cmath>

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shapes/circle.h"
#include "shapes/generic.h" 


namespace Shape {
    Circle2D::Circle2D(float radius, int segments)
        : GenericShape2D({})
        , m_radius { radius }
        , m_segments { segments }
    {
    }

    void Circle2D::generateVertices() {
        if (!m_vertices.empty())
            return;

        for (int i = 0; i < 3; i++)
            m_vertices.push_back(0.0f);

        for (int i = 0; i <= m_segments; i++) {
            float theta = 2.0f * M_PI * i / m_segments;

            m_vertices.push_back(m_radius * cos(theta));
            m_vertices.push_back(m_radius * sin(theta));
            m_vertices.push_back(0.0f);
        }
    }

    void Circle2D::draw() {
        glDrawArrays(drawMode(), 0, m_segments + 2);
    }
}

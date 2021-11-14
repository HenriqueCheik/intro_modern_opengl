#ifndef QUAD_HPP
#define QUAD_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Quad
{
    public:
        glm::vec2 position;
        glm::vec2 dimensions;
        glm::vec4 color;

        double dx;
        double dy;

        Quad(glm::vec2 position = glm::vec2(0.0f, 0.0f), glm::vec2 dimensions = glm::vec2(50.0f, 50.0f), glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        void update(double deltaTime);

        glm::mat4 getModelMatrix();

    private:
};

#endif
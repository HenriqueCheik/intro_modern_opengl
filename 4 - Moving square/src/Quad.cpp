#include "Quad.hpp"

Quad::Quad (glm::vec2 position, glm::vec2 dimensions, glm::vec4 color)
{
    this->position = position;
    this->dimensions = dimensions;
    this->color = color;
    this->dx = 0.0;
    this->dy = 0.0;
}

glm::mat4 Quad::getModelMatrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(this->position, 0.0f));
    model = glm::scale(model, glm::vec3(this->dimensions, 1.0f));
    return model;
}

void Quad::update(double deltaTime)
{
    this->position.x += this->dx * deltaTime;
    this->position.y += this->dy * deltaTime;
}
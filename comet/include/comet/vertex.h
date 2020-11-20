#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
    Vertex(glm::vec3 pos) : position(pos), normal(0.0f), tc(0.0f) {}
    Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 texCoord) : position(pos), normal(normal), tc(texCoord) {}

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tc;
};

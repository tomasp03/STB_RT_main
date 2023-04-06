#pragma once
#include <glm/glm.hpp>

struct Material
{
    glm::vec4 albido;
    float roughness;
    glm::vec4 emissionColor;
    float emissionStrength;
    float smoothness;
};
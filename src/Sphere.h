#pragma once
#include "Material.h"

struct Sphere
{
	glm::vec3 center;
	float radius;
	Material mat;	
};

struct Plane
{
	glm::vec3 point;
	glm::vec3 Normal;
	Material mat;
};


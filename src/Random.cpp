#include "Random.h"

float RandFloat()
{
	return 2 * ((float)std::rand() / float(RAND_MAX)) - 1.0f;
}

float RandSmall()
{
	return (float)std::rand() / (float)RAND_MAX;
}

glm::vec3 RandoVec3()
{
	return glm::vec3(RandFloat(), RandFloat(), RandFloat());
}

glm::vec3 RandomDirecion()
{
	int safetyLimit = 100;
	for (int i = 0; i < safetyLimit; i++)
	{
		glm::vec3 pointInCube = RandoVec3();
		float sqrtDstFromCenter = glm::dot(pointInCube, pointInCube);

		if (sqrtDstFromCenter <= 1.0f)
		{
			return pointInCube / sqrt(sqrtDstFromCenter);
		}
	}
	return glm::vec3(0);
}

glm::vec3 RandomHemisphereDirection(glm::vec3 normal)
{
	glm::vec3 dir = RandomDirecion();
	return dir * glm::sign(glm::dot(normal, dir));
}
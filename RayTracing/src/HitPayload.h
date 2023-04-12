#pragma once
#include <glm/glm.hpp>

struct HitPayload
{
	float hitDistance;
	glm::vec3 worldPos;
	glm::vec3 worldNorm;

	int objectIndex;
};
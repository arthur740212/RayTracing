#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Shape.h"

struct Material
{
	glm::vec3 albedo{ 1.0f };
	float roughness = 1.0f;
	float metallic = 0.0f;
};

struct Scene
{
	std::vector<Shape*> shapes;
	std::vector<Sphere> spheres;
	std::vector<Material> materials;
};
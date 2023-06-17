#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Material
{
	glm::vec3 albedo{ 1.0f };
	float roughness = 1.0f;
	float metallic = 0.0f;
	glm::vec3 emissionColor{ 0.0f };
	float emissionPower = 0.0f;

	glm::vec3 GetEmission() const { return emissionColor * emissionPower; }
};
struct Sphere
{
	glm::vec3 center{ 0.0f };
	float radius = 0.5f;

	int materialIndex = 0;
};
struct Scene
{
	std::vector<Sphere> spheres;
	std::vector<Material> materials;
};
#pragma once
#include <glm/glm.hpp>
#include "Ray.h"
#include "HitPayload.h"
#include "Walnut/Application.h"
#include "glm/gtc/type_ptr.hpp"


struct Shape
{
	int materialIndex = 0;

	virtual void OnGUI() = 0;
	virtual HitPayload CreatePayload(const Ray& ray, float hitDistance, int objectIndex) const = 0;
	virtual float RayIntersection(Ray ray) const = 0;
};

struct Sphere : Shape
{
	glm::vec3 center{ 0.0f };
	float radius = 0.5f;

	void OnGUI() override;
	HitPayload CreatePayload(const Ray& ray, float hitDistance, int objectIndex) const override;
	float RayIntersection(Ray ray) const override;
};
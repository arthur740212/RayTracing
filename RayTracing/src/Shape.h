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

struct Triangle : Shape
{
	glm::vec3 vertex0{ 3.0,2.0f,0.0f };
	glm::vec3 vertex1{ -3.0f,2.0f,0.0f };
	glm::vec3 vertex2{ 0.0f,-1.0f,0.0f };

	void OnGUI() override;
	HitPayload CreatePayload(const Ray& ray, float hitDistance, int objectIndex) const override;
	float RayIntersection(Ray ray) const override;
};
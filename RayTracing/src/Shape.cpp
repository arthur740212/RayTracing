#include "Shape.h"

void Sphere::OnGUI()
{
	ImGui::DragFloat3("Center", glm::value_ptr(center), 0.1f);
	ImGui::DragFloat("Radius", &radius, 0.1f);
}

HitPayload Sphere::CreatePayload(const Ray& ray, float hitDistance, int objectIndex) const
{
	HitPayload payload;
	payload.hitDistance = hitDistance;
	payload.objectIndex = objectIndex;

	glm::vec3 relativeOrigin = ray.origin - center;
	payload.worldPos = relativeOrigin + ray.direction * hitDistance;
	payload.worldNorm = glm::normalize(payload.worldPos);

	payload.worldPos += center;

	return payload;
}

float Sphere::RayIntersection(Ray ray) const
{
	glm::vec3 relativeOrigin = ray.origin - center;

	float a = glm::dot(ray.direction, ray.direction);
	float b = 2.0f * glm::dot(relativeOrigin, ray.direction);
	float c = glm::dot(relativeOrigin, relativeOrigin) - radius * radius;

	float discriminant = b * b - 4.0f * a * c;
	if (discriminant < 0.0f)
	{
		return std::numeric_limits<float>::max();
	}

	//float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a); // Second hit distance (currently unused)
	float smallestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
	return smallestT;
}
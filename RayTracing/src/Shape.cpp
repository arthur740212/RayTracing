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

void Triangle::OnGUI()
{
	ImGui::DragFloat3("Vertex 0", glm::value_ptr(vertex0), 0.1f);
	ImGui::DragFloat3("Vertex 1", glm::value_ptr(vertex1), 0.1f);
	ImGui::DragFloat3("Vertex 2", glm::value_ptr(vertex2), 0.1f);
}

HitPayload Triangle::CreatePayload(const Ray& ray, float hitDistance, int objectIndex) const
{
	HitPayload payload;
	payload.hitDistance = hitDistance;
	payload.objectIndex = objectIndex;

	payload.worldPos = ray.origin + ray.direction * hitDistance;
	glm::vec3 edge1 = vertex1 - vertex0;
	glm::vec3 edge2 = vertex2 - vertex0;
	payload.worldNorm= glm::normalize(glm::cross(edge1, edge2));

	return payload;
}

float Triangle::RayIntersection(Ray ray) const
{
	glm::vec3 edge1 = vertex1 - vertex0;
	glm::vec3 edge2 = vertex2 - vertex0;
	glm::vec3 pvec = glm::cross(ray.direction, edge2);
	float det = glm::dot(edge1, pvec);

	if (det < std::numeric_limits<float>::epsilon() && det>-std::numeric_limits<float>::epsilon())
	{
		return std::numeric_limits<float>::max();
	}

	float invDet = 1.0f / det;
	glm::vec3 tvec = ray.origin - vertex0;
	float u = glm::dot(tvec, pvec) * invDet;

	if (u < 0.0f || u > 1.0f)
	{
		return std::numeric_limits<float>::max();
	}

	glm::vec3 qvec = glm::cross(tvec, edge1);
	float v = glm::dot(ray.direction, qvec) * invDet;

	if (v < 0.0f || u + v > 1.0f)
	{
		return std::numeric_limits<float>::max();
	}

	float t = glm::dot(edge2, qvec) * invDet;
	return t;
}
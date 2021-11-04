#pragma once
#include "ColorDbl.h"
#include "Ray.h"
#include "Scene.h"
#include <glm/glm.hpp>

class Sphere {

	public:

	ColorDbl rgb;
	double radius;
	glm::vec3 position;
	const int material = 1; // default perfect reflector/mirror (currently not implemented)

	Sphere() = default;

	~Sphere() {};

	bool getIntersectionPoint(Ray& ray, Scene scene, float& t_nearest) {
		// Find intersection point between ray and implicit sphere
		double b = glm::dot(2.0f * glm::normalize(ray.direction.direction), (ray.startPoint - this->position));
		double c = glm::dot((ray.startPoint - this->position), (ray.startPoint - this->position)) - this->radius * this->radius;
		double delta = (b * b / 4) - c;
		
		float t;
		// Ray intersects with sphere
		if (delta > 0) {

			t = -b / 2 - sqrt(delta);

			if (t_nearest > t) {

				t_nearest = t;

				ray.endPoint = ray.startPoint + ray.direction.direction * t;
				glm::vec3 sphereNorm = glm::normalize(ray.endPoint - this->position);

				glm::vec3 lightDirection = glm::normalize(scene.lightSource - ray.endPoint);
				double shadowFact = std::max(0.0f, glm::dot(lightDirection, sphereNorm));

				ray.rgb.R = 1.0 * shadowFact;
				ray.rgb.G = 0.0 * shadowFact;
				ray.rgb.B = 0.0 * shadowFact;

				return true;
			}
		}
		// Ray touches sphere
		else if (delta == 0) {

			t = -b / 2;

			if (t_nearest > t) {
				t_nearest = t;

				ray.endPoint = ray.startPoint + ray.direction.direction * t;

				glm::vec3 lightDirection = glm::normalize(scene.lightSource - ray.endPoint);
				glm::vec3 sphereNorm = glm::normalize(ray.endPoint - this->position);
				double shadowFact = std::max(0.0f, glm::dot(lightDirection, sphereNorm));

				ray.rgb.R = 1.0 * shadowFact;
				ray.rgb.G = 0.0 * shadowFact;
				ray.rgb.B = 0.0 * shadowFact;

				return true;
			}
		}
		return false;
	}
};


#pragma once
#include "ColorDbl.h"
#include "Ray.h"
#include <glm/glm.hpp>

class Sphere {

	public:

	ColorDbl rgb = ColorDbl(1.0, 0.0, 0.0);
	double radius;
	glm::vec3 position;
	const int material = 1; // default perfect reflector/mirror (currently not implemented)

	Sphere() = default;

	~Sphere() {};

	bool getIntersectionPoint(Ray& ray, float& t_nearest) {
		// Find intersection point between ray and implicit sphere
		double b = glm::dot(2.0f * glm::normalize(ray.direction.direction), (ray.startPoint - this->position));
		double c = glm::dot((ray.startPoint - this->position), (ray.startPoint - this->position)) - this->radius * this->radius;
		double delta = (b * b / 4) - c;
		
		float t;
		// Ray intersects with sphere
		if (delta > 0) {

			t = -b / 2 - sqrt(delta);

			if (t_nearest > t && t > 0) {
				t_nearest = t;
				ray.endPoint = ray.startPoint + ray.direction.direction * t;

				return true;
			}
		}
		// Ray touches sphere
		else if (delta == 0) {

			t = -b / 2;

			if (t_nearest > t && t > 0) {
				t_nearest = t;
				ray.endPoint = ray.startPoint + ray.direction.direction * t;

				return true;
			}
		}
		return false;
	}
};


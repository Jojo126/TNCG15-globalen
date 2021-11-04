/*
The triangle is defined by three objects of the class Vertex.
The Triangle has a color, which we represent by an instance of ColorDbl.
The triangle’s normal direction is stored in an instance of Direction.
It has a method rayIntersection(Ray arg) that computes the intersection
between a Ray and the Triangle with the Möller-Trumbore algorithm.
*/

#pragma once
#include <glm/glm.hpp>
#include "Vertex.h"
#include "ColorDbl.h"
#include "Direction.h"
#include "Ray.h"
//#include "Scene.h" // creates error

class Triangle
{		
	public:
		ColorDbl rgb;
		Direction normal;
		Vertex v1, v2, v3;
		const int material = 0; // default Lambertian (currently not implemented)

		Triangle() = default;
		
		Triangle(Vertex v1, Vertex v2, Vertex v3, ColorDbl rgb, Direction normal) 
			: v1{ v1 }, v2{ v2 }, v3{ v3 }, rgb{ rgb }, normal{ normal } {}
		
		~Triangle() {};

		/*bool getIntersectionPoint(Ray& ray, Scene scene, float& t_nearest) {
			// Rename triangle vertices
			glm::vec3 p_s = ray.startPoint;
			//glm::vec3 p_e = pixelCoord;
			glm::vec3 v0 = this->v1.coords;
			glm::vec3 v1 = this->v2.coords;
			glm::vec3 v2 = this->v3.coords;

			glm::vec3 T = p_s - v0; // T = P_s - v_0
			glm::vec3 E1 = v1 - v0;
			glm::vec3 E2 = v2 - v0;
			glm::vec3 D = ray.direction.direction; // !! D = P_e - P_s
			glm::vec3 P = glm::cross(D, E2); // P = D x E_2
			glm::vec3 Q = glm::cross(T, E1); // Q = T x E_1 

			double denom = glm::dot(P, E1);
			float t;
			if (denom != 0) {
				t = glm::dot(Q, E2) / denom;
				double u = glm::dot(P, T) / denom;
				double v = glm::dot(Q, D) / denom;

				// Check if point inside triangle
				// t > 0: triangle in front of camera 
				if (t > 0 && u >= 0 && v >= 0 && u + v <= 1) {

					// Need only color from triangle with nearest intersection point to p_s
					if (t_nearest > t) {
						t_nearest = t;

						// Get calculated intersectionpoint and color for pixel from traced ray
						ray.endPoint = ray.startPoint + ray.direction.direction * t;

						// Get triangles normal and compare with light source normal to get local shadow
						glm::vec3 lightDirection = glm::normalize(scene.lightSource - ray.endPoint);
						double shadowFact = std::max(0.0f, glm::dot(lightDirection, this->normal.direction));

						ray.rgb.R = this->rgb.R * shadowFact;
						ray.rgb.G = this->rgb.G * shadowFact;
						ray.rgb.B = this->rgb.B * shadowFact;

						return true;
					}
				}
			}
			return false;
		}*/
};


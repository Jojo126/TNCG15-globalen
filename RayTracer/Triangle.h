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

class Ray;

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

		Triangle operator=(Triangle tri) {
			return tri;
		}

		// Defined in Triangle.cpp
		ColorDbl getLightColor(Ray ray, glm::vec3 light, Triangle surfaceObject, float lightIntensity = 1);
		bool getIntersectionPoint(Ray& ray, float& t_nearest);
};


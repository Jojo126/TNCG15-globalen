/*
The triangle is defined by three objects of the class Vertex.
The Triangle has a color, which we represent by an instance of ColorDbl.
The triangle’s normal direction is stored in an instance of Direction.
It has a method rayIntersection(Ray arg) that computes the intersection
between a Ray and the Triangle with the Möller-Trumbore algorithm.
*/

#pragma once
#include "Vertex.h"
#include "ColorDbl.h"
#include "Direction.h"
#include "Ray.h"

class Ray;

class Triangle
{		
	public:
		ColorDbl rgb;
		Direction normal;
		Vertex v1, v2, v3;

		Triangle() = default;
		
		Triangle(Vertex v1, Vertex v2, Vertex v3, ColorDbl rgb, Direction normal) 
			: v1{ v1 }, v2{ v2 }, v3{ v3 }, rgb{ rgb }, normal{ normal } {}
		
		~Triangle() {};

		// beräkna skärningspunkt mellan triangeln och strålen
        bool rayIntersection(Ray myRay);
	
};


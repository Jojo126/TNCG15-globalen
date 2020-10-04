/*
It has the three components of a direction vector x, y, z.
*/

#pragma once
#include "Vertex.h"

class Direction
{		
	public:
		double x, y, z;
		
		Direction() = default;
		
		Direction(double x, double y, double z) 
		: x{ x }, y{ y }, z{ z } {};
		
		Direction(Vertex v1, Vertex v2, Vertex v3) {
			x = (v2.y - v1.y) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.y - v1.y);
			y = (v2.z - v1.z) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.z - v1.z);
			z = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
		}

		~Direction() {};


		
		// Kryssprodukt
		// Direction crossProduct(Direction inV);

		/// Skalärprodukt
		// double dotProduct(Direction inV);


};


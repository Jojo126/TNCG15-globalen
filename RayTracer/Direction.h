/*
It has the three components of a direction vector x, y, z.
*/

#pragma once
#include <iostream>
#include "Vertex.h"
#include "glm/glm.hpp"

class Direction
{		
	public:
		glm::vec3 direction;
		
		Direction() = default;
		
		Direction(double x, double y, double z) 
		: direction( x ,  y ,  z ) {};

		Direction(glm::vec3 dir)
			: direction{ glm::normalize(dir) } {};
		
		Direction(Vertex v1, Vertex v2, Vertex v3) {
			direction.x = (v2.coords.y - v1.coords.y) * (v3.coords.z - v1.coords.z) - (v2.coords.z - v1.coords.z) * (v3.coords.y - v1.coords.y);
			direction.y = (v2.coords.z - v1.coords.z) * (v3.coords.x - v1.coords.x) - (v2.coords.x - v1.coords.x) * (v3.coords.z - v1.coords.z);
			direction.z = (v2.coords.x - v1.coords.x) * (v3.coords.y - v1.coords.y) - (v2.coords.y - v1.coords.y) * (v3.coords.x - v1.coords.x);
			direction = glm::normalize(direction);
			//std::cout << "Direction.x osv. = " << direction.x << "  " << direction.y << "  " << direction.z << std::endl;
		}

		~Direction() {};

		// Multiplikation mellan vector + skalär
		Direction operator*(float inD);

		friend std::ostream& operator<<(std::ostream& os, const Direction& cd) {
			os << '(' << cd.direction.x << ',' << cd.direction.y << ',' << cd.direction.z << ')';
			return os;
		}

		// Kryssprodukt
		/*Direction crossProduct(Direction inV);

		/// Skalärprodukt
		double dotProduct(Direction inV);

		Direction operator+(Direction inD);

		Direction operator-(Direction inD);*/

};


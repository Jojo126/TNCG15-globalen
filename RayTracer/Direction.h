/*
It has the three components of a direction vector x, y, z.
*/

#pragma once
#include <vector>
#include "Vertex.h"

class Direction
{
	private:
		std::vector<double> direction; // (x,y,z)
	public:
		Direction() = default;
		// Use cross product to compute the normal from the triangles normal from its vertices
		Direction(Vertex v1, Vertex v2, Vertex v3) {
			direction[0] = (v2.y - v1.y) * (v3.z - v1.z) - (v2.z - v1.z) * (v3.y - v1.y);
			direction[1] = (v2.z - v1.z) * (v3.x - v1.x) - (v2.x - v1.x) * (v3.z - v1.z);
			direction[2] = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x);
		}
};


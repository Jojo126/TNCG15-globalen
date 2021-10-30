/*
It has three spatial coordinates x, y, z and w (for homogeneous
coordinates).
*/

#pragma once
#include "glm/glm.hpp"

class Vertex
{
	public:
		glm::vec4 coords;

		Vertex() = default;
		
		/*
		Vertex(int x, int y, int z, int w) 
		: x{ x }, y{ y }, z{ z }, w{ w } {};
		*/

		Vertex(double x, double y, double z, double w)
			: coords(x,y,z,w) {};
		
		~Vertex() {};

		// Overload '-' operator
		//Vertex operator-(Vertex inV);
		//Vertex operator+(Vertex inV);
		//Vertex operator*(double db);
};


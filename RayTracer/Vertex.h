/*
It has three spatial coordinates x, y, z and w (for homogeneous
coordinates).
*/

#pragma once

class Vertex
{
	public:
		int x, y, z, w;

		Vertex() = default;
		
		Vertex(int x, int y, int z, int w) 
		: x{ x }, y{ y }, z{ z }, w{ w } {};
		
		~Vertex() {};
};


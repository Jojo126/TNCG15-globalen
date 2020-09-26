/*
It has two instances of Vertex, which are the ray’s starting point and end
point. You can put the vertices into a vertex list and use references to
these points in Ray. Ray contains a reference to the triangle on which
the end point is located. The ray color is a ColorDbl.
*/

#pragma once
#include "Triangle.h"
#include "ColorDbl.h"
#include "Vertex.h"
#include <list>

class Triangle;

class Ray
{
	public:
		std::list<Vertex>& vertList; //ska innehålla start- & endpoints
		Triangle& triangle;
		ColorDbl rgb;
		
		/*
		Ray(Vertex startP)
		//: rgb{ 0.0, 0.0, 0.0 } 
		{
			//vertList.insert(startP);
		};*/
		
		~Ray() {};
};


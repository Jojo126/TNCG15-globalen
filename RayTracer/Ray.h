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
#include "Scene.h"
#include <cmath>


class Triangle;

class Ray
{
	public:
		//std::list<Vertex>& vertList; //ska innehålla start- & endpoints
		Vertex startPoint, endPoint;
		Triangle& triangle;
		ColorDbl rgb;
		
		
		Ray(Vertex startP, int i, int j) {
			startPoint = startP;

			double deltaY = rand() / RAND_MAX, deltaZ = rand() / RAND_MAX;
			endPoint = Vertex(0.0, (i - 401 + deltaY) * 0.0025, (j - 401 + deltaZ) * 0.0025, 1.0);
	
			triangle = Scene::intersectedTriangle(this);
		};
		
		~Ray() {};
};


/*
It has two instances of Vertex, which are the ray�s starting point and end
point. You can put the vertices into a vertex list and use references to
these points in Ray. Ray contains a reference to the triangle on which
the end point is located. The ray color is a ColorDbl.
*/
//#ifndef TRIANGLE_H
//#define TRIANGLE_H


#pragma once
#include "Triangle.h"
#include "ColorDbl.h"
#include "Vertex.h"
class Ray
{
	public:
		//std::list<Vertex>& vertList; //ska inneh�lla start- & endpoints
		glm::vec3 startPoint, endPoint;
		ColorDbl rgb;
		Direction dir;

		//Ray(Vertex startP, int i, int j);
		Ray() = default;

		
		//triangle = Scene::intersectedTriangle(this);
		
		~Ray() {};
};

//#endif
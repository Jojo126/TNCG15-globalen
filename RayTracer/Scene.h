/*
Scene contains instances of Triangle. We use one Scene object that
consists of 24 instances of Triangle. Scene objects are closed.
The triangles of the floor and the ceiling should be white. Each of the 6
walls has a different color.
It should have a method that determines which triangle is intersected by
the Ray arg by calling successively the rayIntersection(Ray arg) method
of each Triangle. It then passes references to the triangle and the
intersection point to the Ray arg
*/

#pragma once
#include "Triangle.h"
#include "Ray.h"

class Scene
{
	private:
		Triangle floor1, floor2, floor3, floor4, floor5, floor6, // ska vara vita
			ceiling1, ceiling2, ceiling3, ceiling4, ceiling5, ceiling6, // ska vara vita
			wall1p1, wall1p2, // väggarna ska ha olika färger
			wall2p1, wall2p2, 
			wall3p1, wall3p2, 
			wall4p1, wall4p2, 
			wall5p1, wall5p2, 
			wall6p1, wall6p2; 
	public:
		Triangle intersectedTriangle(Ray arg) {
			//loopa igenom varje triangel och kalla på  Triangle::rayIntersection-metod för att se om den blir intersected
			//It then passes references to the triangle and the intersection point to the Ray arg. Lite osäker på vad de innebär
		}
};


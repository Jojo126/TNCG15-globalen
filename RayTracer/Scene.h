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
		Vertex  v1 = Vertex( 0,  6, 5, 1),  v2 = Vertex( 0,  6, -5, 1),
			    v3 = Vertex(10,  6, 5, 1),  v4 = Vertex(10,  6, -5, 1),
	  		    v5 = Vertex(13,  0, 5, 1),  v6 = Vertex(13,  0, -5, 1),
			    v7 = Vertex(10, -6, 5, 1),  v8 = Vertex(10, -6, -5, 1),
			    v9 = Vertex( 0, -6, 5, 1), v10 = Vertex( 0, -6, -5, 1),
			   v11 = Vertex(-3,  0, 5, 1), v12 = Vertex(-3,  0, -5, 1),
			   v13 = Vertex( 5,  0, 5, 1), v14 = Vertex( 5,  0, -5, 1);

		// tak och golv ska vara vita
		Triangle ceiling1 = Triangle(v13, v11,  v1, ColorDbl(255, 255, 255), Direction(v13, v11,  v1)),
				 ceiling2 = Triangle(v13,  v1,  v3, ColorDbl(255, 255, 255), Direction(v13,  v1,  v3)),
				 ceiling3 = Triangle(v13,  v3,  v5, ColorDbl(255, 255, 255), Direction(v13,  v3,  v5)),
				 ceiling4 = Triangle(v13,  v5,  v7, ColorDbl(255, 255, 255), Direction(v13,  v5,  v7)),
				 ceiling5 = Triangle(v13,  v7,  v9, ColorDbl(255, 255, 255), Direction(v13,  v7,  v9)),
				 ceiling6 = Triangle(v13,  v9, v11, ColorDbl(255, 255, 255), Direction(v13,  v9, v11)),
		
				 floor1 = Triangle(v14,  v2, v12, ColorDbl(255, 255, 255), Direction(v14,  v2, v12)),
				 floor2 = Triangle(v14,  v4,  v2, ColorDbl(255, 255, 255), Direction(v14,  v4,  v2)),
				 floor3 = Triangle(v14,  v6,  v4, ColorDbl(255, 255, 255), Direction(v14,  v6,  v4)),
				 floor4 = Triangle(v14,  v8,  v6, ColorDbl(255, 255, 255), Direction(v14,  v8,  v6)),
				 floor5 = Triangle(v14, v10,  v8, ColorDbl(255, 255, 255), Direction(v14, v10,  v8)),
				 floor6 = Triangle(v14, v12, v10, ColorDbl(255, 255, 255), Direction(v14, v12, v10)),

		 		 // väggarna ska ha olika färger
				 wall1p1 = Triangle(v13, v11, v1, ColorDbl(1.0, 0.0, 0.0), Direction(v13, v11, v1)),
				 wall1p2 = Triangle(v13, v11, v1, ColorDbl(1.0, 0.0, 0.0), Direction(v13, v11, v1)),
				 wall2p1 = Triangle(v13, v11, v1, ColorDbl(0.0, 1.0, 0.0), Direction(v13, v11, v1)),
				 wall2p2 = Triangle(v13, v11, v1, ColorDbl(0.0, 1.0, 0.0), Direction(v13, v11, v1)),
				 wall3p1 = Triangle(v13, v11, v1, ColorDbl(0.0, 0.0, 1.0), Direction(v13, v11, v1)),
				 wall3p2 = Triangle(v13, v11, v1, ColorDbl(0.0, 0.0, 1.0), Direction(v13, v11, v1)),
				 wall4p1 = Triangle(v13, v11, v1, ColorDbl(1.0, 1.0, 0.0), Direction(v13, v11, v1)),
				 wall4p2 = Triangle(v13, v11, v1, ColorDbl(1.0, 1.0, 0.0), Direction(v13, v11, v1)),
				 wall5p1 = Triangle(v13, v11, v1, ColorDbl(1.0, 0.0, 1.0), Direction(v13, v11, v1)),
				 wall5p2 = Triangle(v13, v11, v1, ColorDbl(1.0, 0.0, 1.0), Direction(v13, v11, v1)),
				 wall6p1 = Triangle(v13, v11, v1, ColorDbl(0.0, 1.0, 1.0), Direction(v13, v11, v1)),
				 wall6p2 = Triangle(v13, v11, v1, ColorDbl(0.0, 1.0, 1.0), Direction(v13, v11, v1));

	public:
		Triangle intersectedTriangle(Ray arg) {
			//loopa igenom varje triangel och kalla på  Triangle::rayIntersection-metod för att se om den blir intersected
			//It then passes references to the triangle and the intersection point to the Ray arg. Lite osäker på vad de innebär
		}
};


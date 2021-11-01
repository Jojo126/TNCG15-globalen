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
#include <iostream>
#include "Triangle.h"
#include <vector>

class Scene
{
	public:
		std::vector<Vertex> mVertices;
		std::vector<Triangle> mTriangles;
		
		Scene() {
			std::cout << "Creating room..." << std::endl;

			// Vertices used for room mesh
			mVertices.push_back(Vertex(0.0, 6.0, 5.0));
			mVertices.push_back(Vertex(0.0, 6.0, -5.0));
			mVertices.push_back(Vertex(10.0, 6.0, 5.0));
			mVertices.push_back(Vertex(10.0, 6.0, -5.0));
			mVertices.push_back(Vertex(13.0, 0.0, 5.0));
			mVertices.push_back(Vertex(13.0, 0.0, -5.0));
			mVertices.push_back(Vertex(10.0, -6.0, 5.0));
			mVertices.push_back(Vertex(10.0, -6.0, -5.0));
			mVertices.push_back(Vertex(0.0, -6.0, 5.0));
			mVertices.push_back(Vertex(0.0, -6.0, -5.0));
			mVertices.push_back(Vertex(-3.0, 0.0, 5.0));
			mVertices.push_back(Vertex(-3.0, 0.0, -5.0));
			mVertices.push_back(Vertex(5.0, 0.0, 5.0));
			mVertices.push_back(Vertex(5.0, 0.0, -5.0));

			// Tetrahedron
			mVertices.push_back(Vertex(7.0, -1.0, -1.0));
			mVertices.push_back(Vertex(6.5, -1.0, -3.0));
			mVertices.push_back(Vertex(9.0, -2.0, -3.0));
			mVertices.push_back(Vertex(9.0, 0.0, -3.0));
			
			// Square (debugging)
			mVertices.push_back(Vertex(6.0, -2.0, -5.0));
			mVertices.push_back(Vertex(6.0, -2.0, -3.0));
			mVertices.push_back(Vertex(6.0, 2.0, -5.0));
			mVertices.push_back(Vertex(6.0, 2.0, -3.0));
			mVertices.push_back(Vertex(10.0, -2.0, -5.0));
			mVertices.push_back(Vertex(10.0, -2.0, -3.0));
			mVertices.push_back(Vertex(10.0, 2.0, -5.0));
			mVertices.push_back(Vertex(10.0, 2.0, -3.0));
			// Square (debugging)
			mTriangles.push_back(Triangle(mVertices.at(19), mVertices.at(25), mVertices.at(23), ColorDbl(0.0, 255.0, 0.0), Direction(mVertices.at(19), mVertices.at(25), mVertices.at(23))));
			mTriangles.push_back(Triangle(mVertices.at(19), mVertices.at(21), mVertices.at(25), ColorDbl(0.0, 255.0, 255.0), Direction(mVertices.at(19), mVertices.at(21), mVertices.at(25))));
			mTriangles.push_back(Triangle(mVertices.at(19), mVertices.at(18), mVertices.at(21), ColorDbl(0.0, 0.0, 0.0), Direction(mVertices.at(19), mVertices.at(18), mVertices.at(21))));
			mTriangles.push_back(Triangle(mVertices.at(18), mVertices.at(20), mVertices.at(21), ColorDbl(255.0, 0.0, 0.0), Direction(mVertices.at(18), mVertices.at(20), mVertices.at(21))));
			mTriangles.push_back(Triangle(mVertices.at(21), mVertices.at(20), mVertices.at(25), ColorDbl(255.0, 0.0, 0.0), Direction(mVertices.at(21), mVertices.at(20), mVertices.at(25))));
			mTriangles.push_back(Triangle(mVertices.at(20), mVertices.at(24), mVertices.at(25), ColorDbl(255.0, 0.0, 0.0), Direction(mVertices.at(20), mVertices.at(24), mVertices.at(25))));
			mTriangles.push_back(Triangle(mVertices.at(25), mVertices.at(24), mVertices.at(23), ColorDbl(0.0, 255.0, 0.0), Direction(mVertices.at(25), mVertices.at(24), mVertices.at(23))));
			mTriangles.push_back(Triangle(mVertices.at(24), mVertices.at(22), mVertices.at(23), ColorDbl(0.0, 255.0, 255.0), Direction(mVertices.at(24), mVertices.at(22), mVertices.at(23))));
			mTriangles.push_back(Triangle(mVertices.at(22), mVertices.at(19), mVertices.at(23), ColorDbl(0.0, 0.0, 0.0), Direction(mVertices.at(22), mVertices.at(19), mVertices.at(23))));
			mTriangles.push_back(Triangle(mVertices.at(22), mVertices.at(18), mVertices.at(19), ColorDbl(255.0, 0.0, 0.0), Direction(mVertices.at(22), mVertices.at(18), mVertices.at(19))));
			mTriangles.push_back(Triangle(mVertices.at(22), mVertices.at(24), mVertices.at(18), ColorDbl(255.0, 0.0, 0.0), Direction(mVertices.at(22), mVertices.at(24), mVertices.at(18))));
			mTriangles.push_back(Triangle(mVertices.at(24), mVertices.at(20), mVertices.at(18), ColorDbl(255.0, 0.0, 0.0), Direction(mVertices.at(24), mVertices.at(20), mVertices.at(18))));
			
			// Ceiling
			mTriangles.push_back(Triangle(mVertices.at(12), mVertices.at(10), mVertices.at(0), ColorDbl(255.0, 255.0, 255.0), Direction(mVertices.at(12), mVertices.at(10), mVertices.at(0))));
			mTriangles.push_back(Triangle(mVertices.at(12), mVertices.at(0), mVertices.at(2), ColorDbl(255.0, 255.0, 255.0), Direction(mVertices.at(12), mVertices.at(0), mVertices.at(2))));
			mTriangles.push_back(Triangle(mVertices.at(12), mVertices.at(2), mVertices.at(4), ColorDbl(255.0, 255.0, 255.0), Direction(mVertices.at(12), mVertices.at(2), mVertices.at(4))));
			mTriangles.push_back(Triangle(mVertices.at(12), mVertices.at(4), mVertices.at(6), ColorDbl(255.0, 255.0, 255.0), Direction(mVertices.at(12), mVertices.at(4), mVertices.at(6))));
			mTriangles.push_back(Triangle(mVertices.at(12), mVertices.at(6), mVertices.at(8), ColorDbl(255.0, 255.0, 255.0), Direction(mVertices.at(12), mVertices.at(6), mVertices.at(8))));
			mTriangles.push_back(Triangle(mVertices.at(12), mVertices.at(8), mVertices.at(10), ColorDbl(255.0, 255.0, 255.0), Direction(mVertices.at(12), mVertices.at(8), mVertices.at(10))));
			
			// Floor
			mTriangles.push_back(Triangle(mVertices.at(13), mVertices.at(1), mVertices.at(11), ColorDbl(255.0, 255.0, 255.0), Direction(mVertices.at(13), mVertices.at(1), mVertices.at(11))));
			mTriangles.push_back(Triangle(mVertices.at(13), mVertices.at(3), mVertices.at(1), ColorDbl(255.0, 255.0, 255.0), Direction(mVertices.at(13), mVertices.at(3), mVertices.at(1))));
			mTriangles.push_back(Triangle(mVertices.at(13), mVertices.at(5), mVertices.at(3), ColorDbl(255.0, 255.0, 255.0), Direction(mVertices.at(13), mVertices.at(5), mVertices.at(3))));
			mTriangles.push_back(Triangle(mVertices.at(13), mVertices.at(7), mVertices.at(5), ColorDbl(255.0, 255.0, 255.0), Direction(mVertices.at(13), mVertices.at(7), mVertices.at(5))));
			mTriangles.push_back(Triangle(mVertices.at(13), mVertices.at(9), mVertices.at(7), ColorDbl(255.0, 255.0, 255.0), Direction(mVertices.at(13), mVertices.at(9), mVertices.at(7))));
			mTriangles.push_back(Triangle(mVertices.at(13), mVertices.at(11), mVertices.at(9), ColorDbl(255.0, 255.0, 255.0), Direction(mVertices.at(13), mVertices.at(11), mVertices.at(9))));

			// Walls
			mTriangles.push_back(Triangle(mVertices.at(10), mVertices.at(11), mVertices.at(1), ColorDbl(255.0, 0.0, 0.0), Direction(mVertices.at(10), mVertices.at(11), mVertices.at(1))));
			mTriangles.push_back(Triangle(mVertices.at(10), mVertices.at(1), mVertices.at(0), ColorDbl(255.0, 0.0, 0.0), Direction(mVertices.at(10), mVertices.at(1), mVertices.at(0))));
			mTriangles.push_back(Triangle(mVertices.at(0), mVertices.at(1), mVertices.at(3), ColorDbl(0.0, 255.0, 0.0), Direction(mVertices.at(0), mVertices.at(1), mVertices.at(3))));
			mTriangles.push_back(Triangle(mVertices.at(0), mVertices.at(3), mVertices.at(2), ColorDbl(0.0, 255.0, 0.0), Direction(mVertices.at(0), mVertices.at(3), mVertices.at(2))));
			mTriangles.push_back(Triangle(mVertices.at(2), mVertices.at(3), mVertices.at(5), ColorDbl(0.0, 0.0, 255.0), Direction(mVertices.at(2), mVertices.at(3), mVertices.at(5))));
			mTriangles.push_back(Triangle(mVertices.at(2), mVertices.at(5), mVertices.at(4), ColorDbl(0.0, 0.0, 255.0), Direction(mVertices.at(2), mVertices.at(5), mVertices.at(4))));
			mTriangles.push_back(Triangle(mVertices.at(4), mVertices.at(5), mVertices.at(7), ColorDbl(255.0, 255.0, 0.0), Direction(mVertices.at(4), mVertices.at(5), mVertices.at(7))));
			mTriangles.push_back(Triangle(mVertices.at(4), mVertices.at(7), mVertices.at(6), ColorDbl(255.0, 255.0, 0.0), Direction(mVertices.at(4), mVertices.at(7), mVertices.at(6))));
			mTriangles.push_back(Triangle(mVertices.at(6), mVertices.at(7), mVertices.at(9), ColorDbl(255.0, 0.0, 255.0), Direction(mVertices.at(6), mVertices.at(7), mVertices.at(9))));
			mTriangles.push_back(Triangle(mVertices.at(6), mVertices.at(9), mVertices.at(8), ColorDbl(255.0, 0.0, 255.0), Direction(mVertices.at(6), mVertices.at(9), mVertices.at(8))));
			mTriangles.push_back(Triangle(mVertices.at(8), mVertices.at(9), mVertices.at(11), ColorDbl(0.0, 255.0, 255.0), Direction(mVertices.at(8), mVertices.at(9), mVertices.at(11))));
			mTriangles.push_back(Triangle(mVertices.at(8), mVertices.at(11), mVertices.at(10), ColorDbl(0.0, 255.0, 255.0), Direction(mVertices.at(8), mVertices.at(11), mVertices.at(10))));

			// Tetrahedron
			mTriangles.push_back(Triangle(mVertices.at(14), mVertices.at(15), mVertices.at(16), ColorDbl(255.0, 0.0, 0.0), Direction(mVertices.at(14), mVertices.at(15), mVertices.at(16))));
			mTriangles.push_back(Triangle(mVertices.at(14), mVertices.at(17), mVertices.at(15), ColorDbl(255.0, 0.0, 255.0), Direction(mVertices.at(14), mVertices.at(17), mVertices.at(15))));
			mTriangles.push_back(Triangle(mVertices.at(14), mVertices.at(16), mVertices.at(17), ColorDbl(0.0, 255.0, 255.0), Direction(mVertices.at(14), mVertices.at(16), mVertices.at(17))));
			mTriangles.push_back(Triangle(mVertices.at(15), mVertices.at(17), mVertices.at(16), ColorDbl(0.0, 0.0, 0.0), Direction(mVertices.at(15), mVertices.at(17), mVertices.at(16))));
			
			std::cout << "Room created." << std::endl;
		}
			

		/*Triangle intersectedTriangle(Ray arg) {
			//loopa igenom varje triangel och kalla på  Triangle::rayIntersection-metod för att se om den blir intersected
			//It then passes references to the triangle and the intersection point to the Ray arg. Lite osäker på vad de innebär

			for (int z = 0; z < 24; z++) {
				Triangle::rayIntersection(arg);
			}

		}*/
};


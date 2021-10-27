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
//#include "Ray.h"
#include <vector>

// TODO: replace vertex with glm vectors?
class Scene
{
	public:
		std::vector<Vertex> mVertices;
		std::vector<Triangle> mTriangles;
		
		Scene() {
			std::cout << "creating room..." << std::endl;

			// Vertices used for room mesh
			mVertices.push_back(Vertex(0.0, 6.0, 5.0, 1.0));
			mVertices.push_back(Vertex(0.0, 6.0, -5.0, 1.0));
			mVertices.push_back(Vertex(10.0, 6.0, 5.0, 1.0));
			mVertices.push_back(Vertex(10.0, 6.0, -5.0, 1.0));
			mVertices.push_back(Vertex(13.0, 0.0, 5.0, 1.0));
			mVertices.push_back(Vertex(13.0, 0.0, -5.0, 1.0));
			mVertices.push_back(Vertex(10.0, -6.0, 5.0, 1.0));
			mVertices.push_back(Vertex(10.0, -6.0, -5.0, 1.0));
			mVertices.push_back(Vertex(0.0, -6.0, 5.0, 1.0));
			mVertices.push_back(Vertex(0.0, -6.0, -5.0, 1.0));
			mVertices.push_back(Vertex(-3.0, 0.0, 5.0, 1.0));
			mVertices.push_back(Vertex(-3.0, 0.0, -5.0, 1.0));
			mVertices.push_back(Vertex(5.0, 0.0, 5.0, 1.0));
			mVertices.push_back(Vertex(5.0, 0.0, -5.0, 1.0));

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

			std::cout << "created room" << std::endl;
		}
			

		/*Triangle intersectedTriangle(Ray arg) {
			//loopa igenom varje triangel och kalla på  Triangle::rayIntersection-metod för att se om den blir intersected
			//It then passes references to the triangle and the intersection point to the Ray arg. Lite osäker på vad de innebär

			for (int z = 0; z < 24; z++) {
				Triangle::rayIntersection(arg);
			}

		}*/
};


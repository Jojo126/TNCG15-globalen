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
#include "Sphere.h"
#include <vector>

class Scene
{
	public:
		std::vector<Vertex> mVertices;
		std::vector<Triangle> mTriangles;
		glm::vec3 lightSource = glm::vec3(5.0, 0.0, 4.0);
		Sphere sphere;
		
		Scene() {
			std::cout << "Creating room..." << std::endl;

			// Colors
			ColorDbl red = ColorDbl(1.0, 0.0, 0.0);
			ColorDbl green = ColorDbl(0.0, 1.0, 0.0);
			ColorDbl blue = ColorDbl(0.0, 0.0, 1.0);
			ColorDbl cyan = ColorDbl(0.0, 1.0, 1.0);
			ColorDbl magenta = ColorDbl(1.0, 0.0, 1.0);
			ColorDbl yellow = ColorDbl(1.0, 1.0, 0.0);
			ColorDbl black = ColorDbl(0.0, 0.0, 0.0);
			ColorDbl white = ColorDbl(1.0, 1.0, 1.0);

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

			// Area light
			mVertices.push_back(Vertex(7.0, 1.0, 4.5)); //26
			mVertices.push_back(Vertex(5.0, 1.0, 4.5));
			mVertices.push_back(Vertex(5.0, -1.0, 4.5));
			mVertices.push_back(Vertex(7.0, -1.0, 4.5));

			// Square (debugging)
			mTriangles.push_back(Triangle(mVertices.at(19), mVertices.at(25), mVertices.at(23), green, Direction(mVertices.at(19), mVertices.at(23), mVertices.at(25))));
			mTriangles.push_back(Triangle(mVertices.at(19), mVertices.at(21), mVertices.at(25), cyan, Direction(mVertices.at(19), mVertices.at(25), mVertices.at(21))));
			mTriangles.push_back(Triangle(mVertices.at(19), mVertices.at(18), mVertices.at(21), magenta, Direction(mVertices.at(19), mVertices.at(21), mVertices.at(18))));
			mTriangles.push_back(Triangle(mVertices.at(18), mVertices.at(20), mVertices.at(21), red, Direction(mVertices.at(18), mVertices.at(21), mVertices.at(20))));
			mTriangles.push_back(Triangle(mVertices.at(21), mVertices.at(20), mVertices.at(25), red, Direction(mVertices.at(21), mVertices.at(25), mVertices.at(20))));
			mTriangles.push_back(Triangle(mVertices.at(20), mVertices.at(24), mVertices.at(25), red, Direction(mVertices.at(20), mVertices.at(25), mVertices.at(24))));
			mTriangles.push_back(Triangle(mVertices.at(25), mVertices.at(24), mVertices.at(23), green, Direction(mVertices.at(25), mVertices.at(23), mVertices.at(24))));
			mTriangles.push_back(Triangle(mVertices.at(24), mVertices.at(22), mVertices.at(23), cyan, Direction(mVertices.at(24), mVertices.at(23), mVertices.at(22))));
			mTriangles.push_back(Triangle(mVertices.at(22), mVertices.at(19), mVertices.at(23), red, Direction(mVertices.at(22), mVertices.at(23), mVertices.at(19))));
			mTriangles.push_back(Triangle(mVertices.at(22), mVertices.at(18), mVertices.at(19), red, Direction(mVertices.at(22), mVertices.at(19), mVertices.at(18))));
			mTriangles.push_back(Triangle(mVertices.at(22), mVertices.at(24), mVertices.at(18), red, Direction(mVertices.at(22), mVertices.at(18), mVertices.at(24))));
			mTriangles.push_back(Triangle(mVertices.at(24), mVertices.at(20), mVertices.at(18), red, Direction(mVertices.at(24), mVertices.at(18), mVertices.at(20))));
			
			// Ceiling
			mTriangles.push_back(Triangle(mVertices.at(12), mVertices.at(10), mVertices.at(0), white, Direction(mVertices.at(12), mVertices.at(10), mVertices.at(0))));
			mTriangles.push_back(Triangle(mVertices.at(12), mVertices.at(0), mVertices.at(2), white, Direction(mVertices.at(12), mVertices.at(0), mVertices.at(2))));
			mTriangles.push_back(Triangle(mVertices.at(12), mVertices.at(2), mVertices.at(4), white, Direction(mVertices.at(12), mVertices.at(2), mVertices.at(4))));
			mTriangles.push_back(Triangle(mVertices.at(12), mVertices.at(4), mVertices.at(6), white, Direction(mVertices.at(12), mVertices.at(4), mVertices.at(6))));
			mTriangles.push_back(Triangle(mVertices.at(12), mVertices.at(6), mVertices.at(8), white, Direction(mVertices.at(12), mVertices.at(6), mVertices.at(8))));
			mTriangles.push_back(Triangle(mVertices.at(12), mVertices.at(8), mVertices.at(10), white, Direction(mVertices.at(12), mVertices.at(8), mVertices.at(10))));
			
			// Area light
			mTriangles.push_back(Triangle(mVertices.at(26), mVertices.at(28), mVertices.at(27), red, Direction(mVertices.at(26), mVertices.at(28), mVertices.at(27)), 1));
			mTriangles.push_back(Triangle(mVertices.at(26), mVertices.at(29), mVertices.at(28), red, Direction(mVertices.at(26), mVertices.at(29), mVertices.at(28)), 1));


			// Floor
			mTriangles.push_back(Triangle(mVertices.at(13), mVertices.at(1), mVertices.at(11), white, Direction(mVertices.at(13), mVertices.at(1), mVertices.at(11))));
			mTriangles.push_back(Triangle(mVertices.at(13), mVertices.at(3), mVertices.at(1), white, Direction(mVertices.at(13), mVertices.at(3), mVertices.at(1))));
			mTriangles.push_back(Triangle(mVertices.at(13), mVertices.at(5), mVertices.at(3), white, Direction(mVertices.at(13), mVertices.at(5), mVertices.at(3))));
			mTriangles.push_back(Triangle(mVertices.at(13), mVertices.at(7), mVertices.at(5), white, Direction(mVertices.at(13), mVertices.at(7), mVertices.at(5))));
			mTriangles.push_back(Triangle(mVertices.at(13), mVertices.at(9), mVertices.at(7), white, Direction(mVertices.at(13), mVertices.at(9), mVertices.at(7))));
			mTriangles.push_back(Triangle(mVertices.at(13), mVertices.at(11), mVertices.at(9), white, Direction(mVertices.at(13), mVertices.at(11), mVertices.at(9))));

			// Walls
			mTriangles.push_back(Triangle(mVertices.at(10), mVertices.at(11), mVertices.at(1), red, Direction(mVertices.at(10), mVertices.at(11), mVertices.at(1))));
			mTriangles.push_back(Triangle(mVertices.at(10), mVertices.at(1), mVertices.at(0), red, Direction(mVertices.at(10), mVertices.at(1), mVertices.at(0))));
			mTriangles.push_back(Triangle(mVertices.at(0), mVertices.at(1), mVertices.at(3), green, Direction(mVertices.at(0), mVertices.at(1), mVertices.at(3))));
			mTriangles.push_back(Triangle(mVertices.at(0), mVertices.at(3), mVertices.at(2), green, Direction(mVertices.at(0), mVertices.at(3), mVertices.at(2))));
			mTriangles.push_back(Triangle(mVertices.at(2), mVertices.at(3), mVertices.at(5), blue, Direction(mVertices.at(2), mVertices.at(3), mVertices.at(5))));
			mTriangles.push_back(Triangle(mVertices.at(2), mVertices.at(5), mVertices.at(4), blue, Direction(mVertices.at(2), mVertices.at(5), mVertices.at(4))));
			mTriangles.push_back(Triangle(mVertices.at(4), mVertices.at(5), mVertices.at(7), yellow, Direction(mVertices.at(4), mVertices.at(5), mVertices.at(7))));
			mTriangles.push_back(Triangle(mVertices.at(4), mVertices.at(7), mVertices.at(6), yellow, Direction(mVertices.at(4), mVertices.at(7), mVertices.at(6))));
			mTriangles.push_back(Triangle(mVertices.at(6), mVertices.at(7), mVertices.at(9), magenta, Direction(mVertices.at(6), mVertices.at(7), mVertices.at(9))));
			mTriangles.push_back(Triangle(mVertices.at(6), mVertices.at(9), mVertices.at(8), magenta, Direction(mVertices.at(6), mVertices.at(9), mVertices.at(8))));
			mTriangles.push_back(Triangle(mVertices.at(8), mVertices.at(9), mVertices.at(11), cyan, Direction(mVertices.at(8), mVertices.at(9), mVertices.at(11))));
			mTriangles.push_back(Triangle(mVertices.at(8), mVertices.at(11), mVertices.at(10), cyan, Direction(mVertices.at(8), mVertices.at(11), mVertices.at(10))));

			// Tetrahedron
			mTriangles.push_back(Triangle(mVertices.at(14), mVertices.at(15), mVertices.at(16), red, Direction(mVertices.at(14), mVertices.at(15), mVertices.at(16))));
			mTriangles.push_back(Triangle(mVertices.at(14), mVertices.at(17), mVertices.at(15), magenta, Direction(mVertices.at(14), mVertices.at(17), mVertices.at(15))));
			mTriangles.push_back(Triangle(mVertices.at(14), mVertices.at(16), mVertices.at(17), cyan, Direction(mVertices.at(14), mVertices.at(16), mVertices.at(17))));
			mTriangles.push_back(Triangle(mVertices.at(15), mVertices.at(17), mVertices.at(16), black, Direction(mVertices.at(15), mVertices.at(17), mVertices.at(16))));
			
			// Add sphere into scene
			sphere.position = glm::vec3(3.0, 1.0, -1.0);
			sphere.radius = 1.0;

			std::cout << "Room created." << std::endl;
		}
};


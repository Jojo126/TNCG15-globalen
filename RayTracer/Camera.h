/*
It contains two instances of Vertex (the eye points) and a variable that
allows you to switch between both eye points.
It contains a 2D array of size 800 × 800. Each element is a Pixel.
Its method render() launches a ray through each pixel one at a time.
The ray is followed through the scene and the radiance we give to the
pixel is computed according to what we learnt in lectures 4 and 5.
Initially and to test your code you follow the ray until it hits the first
triangle and assign the triangle color to the ray.
*/

#pragma once
#include "Vertex.h"
#include "Pixel.h"
#include "Ray.h"

class Camera
{
	public:

		Vertex eye1, eye2;
		bool useFirstEye;
		Pixel image[800][800]; //borde hämta mått från RayTracer.cpp

		void render() {
			
			for (int i = 0; i < 800; i++) {
				for (int j = 0; j < 800; j++) {
					/*
					if (useFirstEye)
						Ray(eye1);
					else
						Ray(eye2);
					*/
				}
			}
		}
};


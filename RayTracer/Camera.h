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

class Camera
{
	private:
		Vertex eye1, eye2;
		bool useEye1;
		Pixel image[800][800]; //borde hämta mått från RayTracer.cpp
	public:
		void render(); 
		// render() ska loopa genom alla pixlar i image
		// för varje pixel, skapa en ny ray som skjuter genom den pixeln (behöver inte vara genom pixelns mitt)
		// följ rayn och beräkna pixelns färg, följ den till första skärningspunkt och tilldela rayn den korsade triangelns färg
		
		void createimage(); //ska basically göra samma som det i main() som just nu genererar en godtycklig bild

};


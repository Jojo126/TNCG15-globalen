/*
It contains two instances of Vertex (the eye points) and a variable that
allows you to switch between both eye points.
It contains a 2D array of size 800 � 800. Each element is a Pixel.
Its method render() launches a ray through each pixel one at a time.
The ray is followed through the scene and the radiance we give to the
pixel is computed according to what we learnt in lectures 4 and 5.
Initially and to test your code you follow the ray until it hits the first
triangle and assign the triangle color to the ray.
*/

#pragma once
#include "Vertex.h"
#include "Scene.h"
#include <iostream>
//#include "Pixel.h"
//#include "Ray.h"

class Camera
{
	public:

		glm::vec3 eye1 = glm::vec3(-2.0, 0.0, 0.0);
		glm::vec3 eye2 = glm::vec3(-1.0, 0.0, 0.0);
		//glm::vec3 eye2 = glm::vec3(0.5, 0.0, 0.0);
		bool useFirstEye;
		//Pixel image[800][800]; //borde h�mta m�tt fr�n RayTracer.cpp

		// Constructor/Destructors
		Camera() = default;
		~Camera() {};

		//void render(Scene scene);

		//void createImage();
		// i createimage: loopa igenom alla pixlar, i,j, och de tre kanalerna R,G,B, f�r att hitta i_max (h�gsta intensitetsv�rdet i antingen R,G, eller B) 
		// (blir fem nestade loopar)
};


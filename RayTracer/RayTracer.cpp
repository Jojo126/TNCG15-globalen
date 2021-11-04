#include <iostream>
#include <cstdlib>
#include <time.h>
#include <thread>
#include <array>
#include <algorithm>
#include <glm/glm.hpp>

#include "BmpSave.h"
#include "Scene.h"
#include "Camera.h"
#include "Ray.h"
#include "Sphere.h"


// 1. Fixa perfect reflector
//		?. add material properties?? ist�llet f�r att h�rdkoda att sf�ren �r en perfect reflector
// 2. h�lla p� med avst�nd till ljuset o grejs
// 3. shadow rays
// 4. lambertian
// 5. monte carlo
// 6. triangel-ljusk�lla i taket
// 7. KLARA - RAPPORT :DDDDDD

// Specs for bitmap
const int HEIGHT = 800;
const int WIDTH = 800;
double intensityImage[800][800][3];

// Creates an empty room
Scene scene;

// Create the camera
Camera camera;
const glm::vec3 cameraPos = camera.eye2;

// Init image file
static unsigned char image[HEIGHT][WIDTH][BYTES_PER_PIXEL];
char* imageFileName = (char*)"bitmapImage.bmp";

ColorDbl getColor(ColorDbl color, glm::vec3 surfaceNormal, glm::vec3 lightDir) {
	double shadowFact = std::max(0.0f, glm::dot(lightDir, surfaceNormal));

	return ColorDbl(color.R * shadowFact, color.G * shadowFact, color.B * shadowFact);
}

void rendersegment(int s, int e) {
	int i, j;
	double i_max = 0;
	for (i = s; i < e; i++) {
		for (j = 0; j < HEIGHT; j++) {

			// Get pixelcoords from pixel index in image
			glm::vec3 pixelCoord = glm::vec3(0.0, (j - 401.0 + ((double)rand() / (RAND_MAX))) * 0.0025, (i - 401.0 + ((double)rand() / (RAND_MAX))) * 0.0025);

			// Get rays direction from camera and pixel coordinates
			Ray firstRay;
			firstRay.startPoint = cameraPos;
			firstRay.direction = glm::normalize(pixelCoord - firstRay.startPoint);

			float t;
			float t_nearest = INFINITY;

			// Loop through all triangles in scene
			for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
				Triangle currentTriangle = *it;

				if (currentTriangle.getIntersectionPoint(firstRay, t_nearest)) {
					// Get triangles normal and compare with light source normal to get local shadow
					glm::vec3 lightDirection = glm::normalize(scene.lightSource - firstRay.endPoint);
					firstRay.rgb = getColor(currentTriangle.rgb, currentTriangle.normal.direction, lightDirection);
				}
			}

			// Add sphere into scene
			Sphere sphere;
			sphere.position = glm::vec3(4.0, -2.0, 1.0);
			sphere.radius = 1.0;

			if (sphere.getIntersectionPoint(firstRay, t_nearest)) {
				glm::vec3 sphereNorm = glm::normalize(firstRay.endPoint - sphere.position);
				glm::vec3 lightDirection = glm::normalize(scene.lightSource - firstRay.endPoint);

				// TODO: find direction of reflecting ray and find next intersection point for it
				// Get that objects color and save it in this ray
				Ray reflectionRay;
				reflectionRay.startPoint = firstRay.endPoint;

				reflectionRay.direction.direction = glm::vec3(firstRay.direction.direction - sphereNorm * (2.0f * glm::dot(firstRay.direction.direction, sphereNorm)));				

				for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
					Triangle currentTriangle = *it;

					t_nearest = INFINITY;
					if (currentTriangle.getIntersectionPoint(reflectionRay, t_nearest)) {
						// Get triangles normal and compare with light source normal to get local shadow
						glm::vec3 lightDirection = glm::normalize(scene.lightSource - reflectionRay.endPoint);
						reflectionRay.rgb = getColor(currentTriangle.rgb, currentTriangle.normal.direction, lightDirection);
					}
				}

				firstRay.rgb = reflectionRay.rgb;

				//firstRay.rgb = getColor(sphere.rgb, sphereNorm, lightDirection);
			}


			/******************* Shadow Rays *********************/
			// Save sphere or triangle index when intersection done on them?
			// Then check if shadow ray intersects with itself?

			// Shoot shadow ray
			/*
			Ray shadowRay;
			shadowRay.startPoint = firstRay.endPoint;
			shadowRay.endPoint = scene.lightSource;
			shadowRay.direction = Direction(glm::normalize(scene.lightSource - firstRay.endPoint));
			*/

			//glm::vec3 sphereC = glm::vec3(10.0, 2.0, 1.0);
			//double sphereR = 1.5;
			//double b = glm::dot(2.0f * glm::normalize(firstRay.direction.direction), (firstRay.startPoint - sphereC));
			//double c = glm::dot((firstRay.startPoint - sphereC), (firstRay.startPoint - sphereC)) - sphereR * sphereR;
			/*
			 b = glm::dot(2.0f * glm::normalize(shadowRay.direction.direction), (shadowRay.startPoint - sphereC));
			 c = glm::dot((shadowRay.startPoint - sphereC), (shadowRay.startPoint - sphereC)) - sphereR * sphereR;
			 delta = (b * b / 4) - c;
			// Ray intersects with sphere
			if (delta > 0) {
				t = -b / 2 - sqrt(delta);

				if (t > 0.0001 && t_nearest > t) {
					t_nearest = t;
					firstRay.rgb = ColorDbl(0.0, 0.0, 0.0);
				}
			}
			// Ray touches sphere
			else if (delta == 0) {
				t = -b / 2;

				if (t > 0.0001 && t_nearest > t) {
					t_nearest = t;
					firstRay.rgb = ColorDbl(0.0, 0.0, 0.0);
				}
			}
			*/
			// Check if shadow ray intersects with scene objects between intersection point and light source
			/*
			for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
				Triangle currentTriangle = *it;

				std::cout << "new triangle" << std::endl;

				//if (currentTriangle == alreadyintercetedTriangle) skip


				// Rename traingles vertices
				glm::vec3 p_s = shadowRay.startPoint;
				glm::vec3 p_e = shadowRay.endPoint;
				glm::vec3 v0 = currentTriangle.v1.coords;
				glm::vec3 v1 = currentTriangle.v2.coords;
				glm::vec3 v2 = currentTriangle.v3.coords;

				glm::vec3 T = p_s - v0; // T = P_s - v_0
				glm::vec3 E1 = v1 - v0;
				glm::vec3 E2 = v2 - v0;
				glm::vec3 D = p_e - p_s; // D = P_e - P_s
				glm::vec3 P = glm::cross(D, E2); // P = D x E_2
				glm::vec3 Q = glm::cross(T, E1); // Q = T x E_1

				double denom = glm::dot(P, E1);

				if (denom != 0) {
					t = glm::dot(Q, E2) / denom;
					double u = glm::dot(P, T) / denom;
					double v = glm::dot(Q, D) / denom;

					// Check if point inside triangle
					// t > 0: triangle in front of camera
					if (t > 0.0001 && u >= 0 && v >= 0 && u + v <= 1) {
						std::cout << "found intersection" << std::endl;
						if (shadowRay.startPoint + (shadowRay.direction*t).direction == shadowRay.startPoint)
							continue;

						// intersection happened
						firstRay.rgb = ColorDbl(0.0, 0.0, 0.0);
						//break;
					}
				}
			}*/

			// Summerar f�rg fr�n flera rays (BRDF?)
			//firstRay.rgb = shadowRay.rgb;

			/******************************************************/
			
			// Store found color in rendered image
			intensityImage[i][j][2] = firstRay.rgb.R*255;
			intensityImage[i][j][1] = firstRay.rgb.G*255;
			intensityImage[i][j][0] = firstRay.rgb.B*255;
		}
	}
}

int main()
{
	srand(static_cast <unsigned> (time(0))); // Init rand

	// TODO: Render the scene from camera
	//camera.render(scene);

	// Draw/store image
	std::cout << "Rendering image..." << std::endl;

	const int n_threads = 8;
	std::array<std::thread, n_threads> threads;
	for (int i = 0; i < n_threads; i++) {
		int start = i * HEIGHT / n_threads;
		int end = (i + 1) * HEIGHT / n_threads;
		std::cout << "thread " << i << " assigned to height " << start << "-" << end << "\n";

		//threads[i] = std::thread(renderSegment, start, end);
		threads[i] = std::thread(rendersegment, start, end);
		//threads[i] = std::thread(renderPixelsInOrder);
	}

	std::cout << "Rendering started:\n";

	for (int i = 0; i < n_threads; i++)
		threads[i].join();

	// Loopa igenom pixlarna igen f�r att hitta de starkaste intensiteterna os�
	int i, j;
	double i_max = 0;
	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < HEIGHT; j++) {
			for (int index = 0; index < 3; index++) {
				if (intensityImage[i][j][index] > i_max)
					i_max = intensityImage[i][j][index];
			}	
		}
	}
	
	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < HEIGHT; j++) {
			for (int index = 0; index < 3; index++) {
				image[i][j][index] = intensityImage[i][j][index] * 255.99/i_max;
			}
		}
	}

	std::cout << "Image rendered." << std::endl;

	// Save image into bitmap
	generateBitmapImage((unsigned char*)image, WIDTH, HEIGHT, imageFileName);
	printf("Image generated!!");
}
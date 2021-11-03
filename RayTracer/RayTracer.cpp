#include <iostream>
#include <cstdlib>
#include <time.h>
#include <glm/glm.hpp>
#include "BmpSave.h"
#include "Scene.h"
#include "Camera.h"
#include "Ray.h"

int main()
{
	// Creates an empty room
	Scene scene;

	// Create the camera
	Camera camera;
	const glm::vec3 cameraPos = camera.eye2;

	// Specs for bitmap
	const int HEIGHT = 800;
	const int WIDTH = 800;

	// Init image file
	static unsigned char image[HEIGHT][WIDTH][BYTES_PER_PIXEL];
	char* imageFileName = (char*) "bitmapImage.bmp";

	srand(static_cast <unsigned> (time(0))); // Init rand

	// TODO: Render the scene from camera
	//camera.render(scene);

	// Draw/store image
	std::cout << "Rendering image..." << std::endl;

	int i, j;
	double i_max = 0;
	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < HEIGHT; j++) {
			//std::cout << "new pixel" << std::endl;

			// Get pixelcoords from pixel index in image
			glm::vec3 pixelCoord = glm::vec3(0.0, (j - 401.0 + ((double)rand() / (RAND_MAX)))*0.0025, (i - 401.0 + ((double)rand() / (RAND_MAX))) * 0.0025);

			// Get rays direction from camera and pixel coordinates
			Ray firstRay;
			firstRay.startPoint = cameraPos;
			firstRay.direction = glm::normalize(pixelCoord - firstRay.startPoint);
			
			double t;
			double t_nearest = INFINITY;
	
			// Find intersection point between ray and implicit sphere
			glm::vec3 sphereC = glm::vec3(10.0, -3.0, -1.0);
			double sphereR = 1.5;
			double b = glm::dot(2.0f*glm::normalize(firstRay.direction.direction), (firstRay.startPoint - sphereC));
			double c = glm::dot((firstRay.startPoint - sphereC), (firstRay.startPoint - sphereC)) - sphereR * sphereR;
			double delta = (b * b/4) - c;
			
			// Ray intersects with sphere
			if (delta > 0) {
				t = -b/2 - sqrt(delta);

				if (t_nearest > t) {
					t_nearest = t;
					firstRay.rgb = ColorDbl(1.0, 0.0, 0.0);
				}
			}
			// Ray touches sphere
			else if (delta == 0) {
				t = -b/2;

				if (t_nearest > t) {
					t_nearest = t;
					firstRay.rgb = ColorDbl(1.0, 0.0, 0.0);
				}
			}

			// Loopa through all triangles in scene
			for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
				Triangle currentTriangle = *it;

				// Rename traingles vertices
				glm::vec3 p_s = firstRay.startPoint;
				glm::vec3 p_e = pixelCoord;
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
					if (t > 0 && u >= 0 && v >= 0 && u + v <= 1) {

						// Need only color from triangle with nearest intersection point to p_s
						if (t_nearest > t) {
							t_nearest = t;

							// Get calculated intersectionpoint and color for pixel from traced ray
							firstRay.endPoint = firstRay.startPoint + (firstRay.direction * t).direction;
							firstRay.rgb = currentTriangle.rgb;
						}
					}
				}
			}

			// Save sphere or triangle index when intersection done on them?
			// Then check if shadow ray intersects with itself?

			// Shoot shadow ray
			Ray shadowRay;
			shadowRay.startPoint = firstRay.endPoint;
			shadowRay.endPoint = scene.lightSource;
			shadowRay.direction = Direction(glm::normalize(scene.lightSource - firstRay.endPoint));

			//glm::vec3 sphereC = glm::vec3(10.0, 2.0, 1.0);
			//double sphereR = 1.5;
			//double b = glm::dot(2.0f * glm::normalize(firstRay.direction.direction), (firstRay.startPoint - sphereC));
			//double c = glm::dot((firstRay.startPoint - sphereC), (firstRay.startPoint - sphereC)) - sphereR * sphereR;

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

			// Summerar färg från flera rays (BRDF?)
			//firstRay.rgb = shadowRay.rgb;

			// Store found color in rendered image
			image[i][j][2] = firstRay.rgb.R;
			image[i][j][1] = firstRay.rgb.G;
			image[i][j][0] = firstRay.rgb.B;

			if (image[i][j][2] > i_max) {
				i_max = image[i][j][2];
			}
			if (image[i][j][1] > i_max) {
				i_max = image[i][j][1];
			}
			if (image[i][j][0] > i_max) {
				i_max = image[i][j][0];
			}
		}
	}


	// Loopa igenom pixlarna igen för att hitta de starkaste intensiteterna oså
	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < HEIGHT; j++) {
			for (int index = 0; index < 3; index++) {
				image[i][j][index] *= 255.99/i_max;
			}
		}
	}

	std::cout << "Image rendered." << std::endl;

	// Save image into bitmap
	generateBitmapImage((unsigned char*)image, WIDTH, HEIGHT, imageFileName);
	printf("Image generated!!");
}
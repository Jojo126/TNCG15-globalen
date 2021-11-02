#include <iostream>
#include <cstdlib>
#include <time.h>
#include <glm/glm.hpp>
#include "BmpSave.h"
#include "Scene.h"
#include "Camera.h"

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

			// Get pixelcoords from pixel index in image
			glm::vec3 pixelCoord = glm::vec3(0.0, (j - 401.0 + ((double)rand() / (RAND_MAX)))*0.0025, (i - 401.0 + ((double)rand() / (RAND_MAX))) * 0.0025);

			// Get rays direction from camera and pixel coordinates
			glm::vec3 rayStart = cameraPos;
			glm::vec3 rayDirection = pixelCoord - rayStart;
			
			ColorDbl color; 
			double t;
			double t_nearest = INFINITY;
	
			// Find intersection point between ray and implicit sphere
			glm::vec3 sphereC = glm::vec3(10.0, 2.0, 1.0);
			double sphereR = 1.5;
			double b = glm::dot(2.0f*glm::normalize(rayDirection), (rayStart - sphereC));
			double c = glm::dot((rayStart - sphereC), (rayStart - sphereC)) - sphereR * sphereR;
			double delta = (b * b/4) - c;
			
			// Ray intersects with sphere
			if (delta > 0) {
				t = -b/2 - sqrt(delta);

				if (t_nearest > t) {
					t_nearest = t;
					color = ColorDbl(1.0, 0.0, 0.0);
				}
			}
			// Ray touches sphere
			else if (delta == 0) {
				t = -b/2;

				if (t_nearest > t) {
					t_nearest = t;
					color = ColorDbl(1.0, 0.0, 0.0);
				}
			}

			// Loopa through all triangles in scene
			for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
				Triangle currentTriangle = *it;

				// Rename traingles vertices
				glm::vec3 p_s = cameraPos;
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
							// Get calculated color for pixel from traced ray
							color = currentTriangle.rgb;
						}
					}
				}
			}
			// Store found color in rendered image
			image[i][j][2] = color.R;
			image[i][j][1] = color.G;
			image[i][j][0] = color.B;

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
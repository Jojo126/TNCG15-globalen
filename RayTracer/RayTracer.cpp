#define _USE_MATH_DEFINES
#include <cmath>
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
//		?. add material properties?? istället för att hårdkoda att sfären är en perfect reflector
// 2. hålla på med avstånd till ljuset o grejs
// 3. shadow rays
// 4. lambertian
// 5. monte carlo estimator
// 6. triangel-ljuskälla i taket
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

// Helps remove shadow acne (dark noise) by moving the intersection point by a small distance away from the intersecting surface
// Can displace the shadows location by a small amount as an effect from displacing the intersection point
// theory from scratchapixel.com (link: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/ligth-and-shadows)
float shadowBias = 1e-4;

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
			bool isIntersectingMirror = false;

			// Loop through all triangles in scene
			for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
				Triangle currentTriangle = *it;

				if (currentTriangle.getIntersectionPoint(firstRay, t_nearest)) {
					isIntersectingMirror = false;
					// Get triangles normal and compare with light source normal to get local shadow
					glm::vec3 lightDirection = glm::normalize(scene.lightSource - firstRay.endPoint);
					//firstRay.rgb = getColor(currentTriangle.rgb, currentTriangle.normal.direction, lightDirection);

					// TODO: make sure correct falloff value
					float r = glm::length(firstRay.endPoint - scene.lightSource);
					float lightIntensity = 1;
					glm::vec3 shadedRGB = glm::vec3(1.0, 1.0, 1.0) * lightIntensity / (4 * float(M_PI) * r);

					double shadowFact = std::max(0.0f, glm::dot(lightDirection, currentTriangle.normal.direction));

					firstRay.rgb.R = currentTriangle.rgb.R * shadedRGB.r * shadowFact;
					firstRay.rgb.G = currentTriangle.rgb.G * shadedRGB.g * shadowFact;
					firstRay.rgb.B = currentTriangle.rgb.B * shadedRGB.b * shadowFact;

					// Remove shadow acne
					firstRay.endPoint += currentTriangle.normal.direction * shadowBias;
				}
			}

			// Add sphere into scene
			Sphere sphere;
			sphere.position = glm::vec3(4.0, -2.0, 1.0);
			sphere.radius = 1.0;

			if (sphere.getIntersectionPoint(firstRay, t_nearest)) {
				isIntersectingMirror = true;
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
						//reflectionRay.rgb = getColor(currentTriangle.rgb, currentTriangle.normal.direction, lightDirection);

						// TODO: make sure correct falloff value
						float r = glm::length(firstRay.endPoint - scene.lightSource);
						float lightIntensity = 1;
						glm::vec3 shadedRGB = glm::vec3(1.0, 1.0, 1.0) * lightIntensity / (4 * float(M_PI) * r);

						double shadowFact = std::max(0.0f, glm::dot(lightDirection, currentTriangle.normal.direction));

						reflectionRay.rgb.R = currentTriangle.rgb.R * shadedRGB.r * shadowFact;
						reflectionRay.rgb.G = currentTriangle.rgb.G * shadedRGB.g * shadowFact;
						reflectionRay.rgb.B = currentTriangle.rgb.B * shadedRGB.b * shadowFact;
					}
				}

				firstRay.rgb = reflectionRay.rgb;

				// Removes shadow acne
				firstRay.endPoint += sphereNorm * shadowBias;

				//firstRay.rgb = getColor(sphere.rgb, sphereNorm, lightDirection);
			}

			/*********** Shadow Rays (direct light) *************/
			Ray shadowRay = Ray();
			shadowRay.startPoint = firstRay.endPoint;
			shadowRay.endPoint = scene.lightSource;
			shadowRay.direction.direction = glm::normalize(shadowRay.endPoint - shadowRay.startPoint);
			float temp_t = INFINITY;
			float distanceRaystartToLight = glm::length(shadowRay.endPoint - shadowRay.startPoint);
			bool isOccluded = false;

			// Check if triangle is occluding
			for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
				Triangle currentTriangle = *it;

				if (currentTriangle.getIntersectionPoint(shadowRay, temp_t) && 0 < temp_t && temp_t < distanceRaystartToLight) {
					isOccluded = true;
					break;
				}
			}
			// Check if sphere is occluding
			if (sphere.getIntersectionPoint(shadowRay, temp_t) && 0 < temp_t && temp_t < distanceRaystartToLight) {
				isOccluded = true;
			}

			if (isOccluded && !isIntersectingMirror) {
				firstRay.rgb = ColorDbl(0.0, 0.0, 0.0);
			}

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

	// Loopa igenom pixlarna igen för att hitta de starkaste intensiteterna oså
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
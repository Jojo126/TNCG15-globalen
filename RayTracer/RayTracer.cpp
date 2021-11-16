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


// 1. DONE - Fixa perfect reflector
//		?. add material properties?? ist�llet f�r att h�rdkoda att sf�ren �r en perfect reflector
// 2. DONE - h�lla p� med avst�nd till ljuset o grejs
// 3. DONE - shadow rays
// 4. DONE? - lambertian
// 5. monte carlo estimator (indirect light)
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

// Helps remove shadow acne (dark noise) by moving the intersection point by a small distance away from the intersecting surface
// Can displace the shadows location by a small amount as an effect from displacing the intersection point
// theory from scratchapixel.com (link: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/ligth-and-shadows)
float shadowBias = 1e-4;

// Could be moved into triangle class as a method since only Triangle uses it. But, it should be more generic and also work for spheres.
ColorDbl getLightColor(Ray ray, glm::vec3 light, Triangle surfaceObject, float lightIntensity = 1) {

	// TODO: make sure correct falloff value
	// Decrease intensity for incoming light when surface is far away from the lightsource 
	float r = glm::length(ray.endPoint - light);
	glm::vec3 distanceShade = glm::vec3(1.0, 1.0, 1.0) * lightIntensity / (4 * float(M_PI) * r);

	// Get surface normal and compare with light source normal to decrease incoming light from an angle
	glm::vec3 lightDirection = glm::normalize(light - ray.endPoint);
	double angleShade = std::max(0.0f, glm::dot(lightDirection, surfaceObject.normal.direction));

	return ColorDbl(surfaceObject.rgb.R * distanceShade.r * angleShade, surfaceObject.rgb.G * distanceShade.g * angleShade, surfaceObject.rgb.B * distanceShade.b * angleShade);
}

void findIntersection(Ray& ray) {
	float t;
	float t_nearest = INFINITY;

	// Loop through all triangles in scene
	for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
		Triangle currentTriangle = *it;

		if (currentTriangle.getIntersectionPoint(ray, t_nearest)) {
			ray.isIntersectingMirror = false;
			ray.intersectingTriangle = currentTriangle;

			ray.rgb = getLightColor(ray, scene.lightSource, currentTriangle);

			// Remove shadow acne
			ray.endPoint += currentTriangle.normal.direction * shadowBias;
		}
	}

	// Check if nearest intersection is on the sphere
	if (scene.sphere.getIntersectionPoint(ray, t_nearest)) {
		ray.isIntersectingMirror = true;
		glm::vec3 sphereNorm = glm::normalize(ray.endPoint - scene.sphere.position);
		glm::vec3 lightDirection = glm::normalize(scene.lightSource - ray.endPoint);

		/* Send a reflection ray to get color from reflecting objects around the sphere
		 * because the sphere uses a perfect reflection material */

		 // Create a reflected ray
		Ray reflectionRay;
		reflectionRay.startPoint = ray.endPoint;
		reflectionRay.direction.direction = glm::vec3(ray.direction.direction - sphereNorm * (2.0f * glm::dot(ray.direction.direction, sphereNorm)));

		// Loop through all other objects in the scene (i.e. currently only triangles) and find nearest intersectionpoint for the reflected ray
		// Then get the color from the found intersectionpoint and use it on the sphere
		for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
			Triangle currentTriangle = *it;

			t_nearest = INFINITY;
			if (currentTriangle.getIntersectionPoint(reflectionRay, t_nearest)) {
				reflectionRay.intersectingTriangle = currentTriangle;
				reflectionRay.rgb = getLightColor(reflectionRay, scene.lightSource, currentTriangle);
			}
		}
		ray.rgb = reflectionRay.rgb;

		// Removes shadow acne
		ray.endPoint += sphereNorm * shadowBias;
	}
}

ColorDbl getDirectLight(Ray ray) {
	// Create the shadow ray
	Ray shadowRay = Ray();
	shadowRay.startPoint = ray.endPoint;
	shadowRay.endPoint = scene.lightSource;
	shadowRay.direction.direction = glm::normalize(shadowRay.endPoint - shadowRay.startPoint);

	// IntersectingObject

	float temp_t = INFINITY;
	float distanceRaystartToLight = glm::length(shadowRay.endPoint - shadowRay.startPoint);
	bool isOccluded = false; // Flag to prevent the sphere (mirror) to become occluded

	// Check if triangle is occluding
	for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
		Triangle currentTriangle = *it;

		if (currentTriangle.getIntersectionPoint(shadowRay, temp_t) && 0 < temp_t && temp_t < distanceRaystartToLight) {
			isOccluded = true;
			break;
		}
	}
	// Check if sphere is occluding
	if (scene.sphere.getIntersectionPoint(shadowRay, temp_t) && 0 < temp_t && temp_t < distanceRaystartToLight) {
		isOccluded = true;
	}

	// Change color of ray to black because occluded by other object(s)
	if (isOccluded && !ray.isIntersectingMirror) {
		ray.rgb = ColorDbl(0.0, 0.0, 0.0);
	}

	return ray.rgb;
}

ColorDbl castRay(Ray ray) {
	// Base case: if max depth, compute combined color and return

	findIntersection(ray);

	/*********** Shadow Rays (direct light) *************/
	/* When nearest color for nearest intersection is found, check if occluded by other objects in the scene i.e. no illumination from the direct light */
	ColorDbl directLight = getDirectLight(ray);


	/************* Monte carlo estimator (indirect light) ************/

	// intersectingTriangle.normal

	/******************************************************/

	return directLight;
}

void renderPixel(int i, int j) {
	// Get pixelcoords from pixel index in image
	glm::vec3 pixelCoord = glm::vec3(0.0, (j - 401.0 + ((double)rand() / (RAND_MAX))) * 0.0025, (i - 401.0 + ((double)rand() / (RAND_MAX))) * 0.0025);

	// Create first ray to be casted into scene
	Ray firstRay;
	firstRay.startPoint = cameraPos;
	firstRay.direction = glm::normalize(pixelCoord - firstRay.startPoint);

	ColorDbl  finalColor = castRay(firstRay);

	// Store found color of pixel in rendered image
	intensityImage[i][j][2] = finalColor.R * 255;
	intensityImage[i][j][1] = finalColor.G * 255;
	intensityImage[i][j][0] = finalColor.B * 255;
}

void rendersegment(int s, int e) {
	int i, j;
	double i_max = 0;
	for (i = s; i < e; i++) {
		for (j = 0; j < HEIGHT; j++) {
			renderPixel(i, j);
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
	
	// Convert pixel values into RGB and save in bitmap-file
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
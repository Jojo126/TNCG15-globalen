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
double intensityImage[WIDTH][HEIGHT][3];

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
const int MAX_DEPTH = 1;

// Could be moved into triangle class as a method since only Triangle uses it. But, it should be more generic and also work for spheres.
ColorDbl getLightColor(Ray ray, glm::vec3 light, Triangle surfaceObject, float lightIntensity = 1) {

	// TODO: make sure correct falloff value
	// Decrease intensity for incoming light when surface is far away from the lightsource 
	float r = glm::length(ray.endPoint - light);
	glm::vec3 distanceShade = glm::vec3(1.0, 1.0, 1.0) * lightIntensity / (4 * float(M_PI) * sqrt(r));

	// Get surface normal and compare with light source normal to decrease incoming light from an angle
	glm::vec3 lightDirection = glm::normalize(light - ray.endPoint);
	double angleShade = std::max(0.0f, glm::dot(lightDirection, surfaceObject.normal.direction));

	return ColorDbl(surfaceObject.rgb.R * distanceShade.r * angleShade, surfaceObject.rgb.G * distanceShade.g * angleShade, surfaceObject.rgb.B * distanceShade.b * angleShade);
}

Ray findIntersection(Ray ray) {
	//float t;
	float t_nearest = INFINITY;

	// Loop through all triangles in scene
	//for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
	for (int index = 0; index < scene.mTriangles.size(); index++) {
		//Triangle currentTriangle = *it;
		Triangle currentTriangle = scene.mTriangles[index];
		//std::cout << "currentTriangle = " << currentTriangle.normal.direction.x << "  " << currentTriangle.normal.direction.y << "  " << currentTriangle.normal.direction.z << std::endl;
		if (currentTriangle.getIntersectionPoint(ray, t_nearest)) {
			
			ray.isIntersectingMirror = false;
			//std::cout << "before ray.intersect.. = " << ray.intersectingTriangle.normal.direction.x << "  " << ray.intersectingTriangle.normal.direction.y << "  " << ray.intersectingTriangle.normal.direction.z << std::endl;
			ray.intersectingTriangle = currentTriangle;
			ray.intersectingTriangle.normal.direction = currentTriangle.normal.direction;
			//std::cout << "after ray.intersect..= " << ray.intersectingTriangle.normal.direction.x << "  " << ray.intersectingTriangle.normal.direction.y << "  " << ray.intersectingTriangle.normal.direction.z << std::endl;
			Ray testRay = ray;
			//std::cout << "testRay.intersect..= " << testRay.intersectingTriangle.normal.direction.x << "  " << testRay.intersectingTriangle.normal.direction.y << "  " << testRay.intersectingTriangle.normal.direction.z << std::endl;
			ray.rgb = getLightColor(ray, scene.lightSource, currentTriangle);

			// Remove shadow acne
			ray.endPoint += currentTriangle.normal.direction * shadowBias;
		}
	}
	/**/
	// Check if nearest intersection is on the sphere
	if (scene.sphere.getIntersectionPoint(ray, t_nearest)) {
		ray.isIntersectingMirror = true;
		glm::vec3 sphereNorm = glm::normalize(ray.endPoint - scene.sphere.position);
		glm::vec3 lightDirection = glm::normalize(scene.lightSource - ray.endPoint);

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
	
	return ray;
}

ColorDbl getDirectLight(Ray ray) {
	// Create the shadow ray
	Ray shadowRay;
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

Ray getNewReflectedRay(Ray oldRay, double& cosTheta) {
	/* Outlines for finding a new reflected ray
	* 1. Create local coordinate system
	*  - Project incoming ray onto surfaces plane (x or y...)
	*  - Surfaces normal (z)
	*  - crossproduct between two previous base-vectors (obs, want a right handed system)
	* 2. Find transformation matrix M
	* 3. Generate random spherical coordinates on intersections hemisphere
	* 4. Convert spherical coordinates to local carteisan coordinates with spherical-to-carteisan equations
	* 5. Convert local vector to global coordinate system with inverted M matrix
	* 6. Return the found vector!
	*/

	// Create local system
	glm::vec3 localSysAxisZ = oldRay.intersectingTriangle.normal.direction; // possibly very wrong :'D
	// From incoming ray
	glm::vec3 localSysAxisX = glm::normalize(oldRay.direction.direction - oldRay.intersectingTriangle.normal.direction * (oldRay.direction.direction, oldRay.intersectingTriangle.normal.direction));
	glm::vec3 localSysAxisY = glm::cross(localSysAxisX, localSysAxisZ);

	/*std::cout << "X = " << localSysAxisX.x << " " << localSysAxisX.y << " " << localSysAxisX.z << std::endl;
	std::cout << "Y = " << localSysAxisY.x << " " << localSysAxisY.y << " " << localSysAxisY.z << std::endl;
	std::cout << "Z = " << localSysAxisZ.x << " " << localSysAxisZ.y << " " << localSysAxisZ.z << std::endl;*/

	// Get world to local transformation matrix
	glm::mat4 M_1 = { localSysAxisX.x, localSysAxisY.x, localSysAxisZ.x, 0.0f,
					  localSysAxisX.y, localSysAxisY.y, localSysAxisZ.y, 0.0f,
					  localSysAxisX.z, localSysAxisY.z, localSysAxisZ.z, 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f };
	glm::mat4 M_2 = { 1.0f, 0.0f, 0.0f, -oldRay.endPoint.x,
					  0.0f, 1.0f, 0.0f, -oldRay.endPoint.y,
					  0.0f, 0.0f, 1.0f, -oldRay.endPoint.z,
					  0.0f, 0.0f, 0.0f, 1.0f };
	glm::mat4 M = M_1 * M_2;

	
	/*
	std::cout << M_1[0][0] << " " << M_1[1][0] << " " << M_1[2][0] << " " << M_1[3][0] << std::endl;
	std::cout << M_1[0][1] << " " << M_1[1][1] << " " << M_1[2][1] << " " << M_1[3][1] << std::endl;
	std::cout << M_1[0][2] << " " << M_1[1][2] << " " << M_1[2][2] << " " << M_1[3][2] << std::endl;
	std::cout << M_1[0][3] << " " << M_1[1][3] << " " << M_1[2][3] << " " << M_1[3][3] << std::endl;*/

	// Randomize new reflected ray direction
	float theta = ((double)rand() / (RAND_MAX)) * M_PI / 2;
	float phi = ((double)rand() / (RAND_MAX)) * 2 * M_PI;

	cosTheta = cos(theta);

	// Convert randomized spherical coordinates to carteisan coordinates in the local system	
	float x_cart = glm::cos(phi) * glm::sin(theta); // x = cos phi * sin theta
	float y_cart = glm::sin(phi) * glm::sin(theta); // y = sin phi * sin theta
	float z_cart = glm::cos(theta); // z = cos theta

	// Convert ray coordinates to global system
	glm::vec4 reflected_local = { x_cart, y_cart, z_cart, 1.0f };
	//std::cout << "reflected_local = " << reflected_local.x << " " << reflected_local.y << " " << reflected_local.z << std::endl;
	glm::vec3 reflected_global = reflected_local * glm::inverse(M);
	//std::cout << "reflected_global = " << reflected_global.x << " " << reflected_global.y << " " << reflected_global.z << std::endl;

	Ray reflectedRay;
	reflectedRay.startPoint = oldRay.endPoint;
	reflectedRay.direction = glm::normalize(reflected_global);
	reflectedRay.depth = oldRay.depth;

	//std::cout << "reflected ray dir: " << reflectedRay.direction << std::endl;

	return reflectedRay;
}

ColorDbl castRay(Ray ray) {
	ColorDbl accLight = ColorDbl(0.0, 0.0, 0.0);

	//std::cout << "BEFORE: castRay normal = " << ray.intersectingTriangle.normal.direction.x << "  " << ray.intersectingTriangle.normal.direction.y << "  " << ray.intersectingTriangle.normal.direction.z << std::endl;

	// Need to find rays endpoint before incoming direct light on intersecting point can be found
	Ray newRay = findIntersection(ray);
	//std::cout << "AFTER: castRay normal = " << newRay.intersectingTriangle.normal.direction.x << "  " << newRay.intersectingTriangle.normal.direction.y << "  " << newRay.intersectingTriangle.normal.direction.z << std::endl;
	// Base case: If reached max recursive depth, don't look for indirect light
	if (newRay.depth >= MAX_DEPTH) {
		ColorDbl accLight = getDirectLight(newRay);
		return accLight;
	}
	newRay.depth++;

	// Direct Light (Shadow Rays)	
	ColorDbl directLight = getDirectLight(newRay);
	//std::cout << "directLight = " << directLight << std::endl;

	// Indirect Light (Monte carlo estimator)
	double cosTheta = 0;
	Ray reflectedRay = getNewReflectedRay(newRay, cosTheta);
	accLight += castRay(reflectedRay) * cosTheta;
	accLight += getLightColor(newRay, scene.lightSource, newRay.intersectingTriangle);
	accLight += directLight;
	//std::cout << directLight << std::endl;
	accLight.R *= newRay.intersectingTriangle.rgb.R / M_PI;
	accLight.G *= newRay.intersectingTriangle.rgb.G / M_PI;
	accLight.B *= newRay.intersectingTriangle.rgb.B / M_PI;
	//accLight = ColorDbl(0.001, 0.0, 0.0); // temp color
	//std::cout << accLight << std::endl; // keeps getting mainly black indirect diffuse light...
	return accLight;
}

void renderPixel(int i, int j) {
	float delta = 2.0f / WIDTH;
	// Get pixelcoords from pixel index in image
	glm::vec3 pixelCoord = glm::vec3(0.0, (j - (WIDTH/2+1.0) + ((double)rand() / (RAND_MAX))) * delta, (i - (WIDTH / 2 + 1.0) + ((double)rand() / (RAND_MAX))) * delta);

	// Create first ray to be casted into scene
	Ray firstRay;
	firstRay.startPoint = cameraPos;
	firstRay.direction = glm::normalize(pixelCoord - firstRay.startPoint);
	firstRay = findIntersection(firstRay);
	ColorDbl directLight = getDirectLight(firstRay);
	//directLight = ColorDbl(0.0, 0.0, 0.0);

	ColorDbl indirectLight = castRay(firstRay); // broken AF!!!!!!
	//indirectLight = ColorDbl(0.0, 0.0, 0.0);

	ColorDbl combinedLight; // = (directDiffuse / M_PI + 2 * indirectDiffuse) * object->albedo;
	combinedLight.R = (directLight.R / M_PI + 2 * indirectLight.R) * firstRay.intersectingTriangle.rgb.R;
	combinedLight.G = (directLight.G / M_PI + 2 * indirectLight.G) * firstRay.intersectingTriangle.rgb.G;
	combinedLight.B = (directLight.B / M_PI + 2 * indirectLight.B) * firstRay.intersectingTriangle.rgb.B;

	// Store found color of pixel in rendered image
	intensityImage[i][j][2] = combinedLight.R * 255;
	intensityImage[i][j][1] = combinedLight.G * 255;
	intensityImage[i][j][0] = combinedLight.B * 255;
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

	const int n_threads = 1;
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
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
//		?. add material properties?? istället för att hårdkoda att sfären är en perfect reflector
// 2. DONE - hålla på med avstånd till ljuset o grejs
// 3. DONE - shadow rays
// 4. DONE - lambertian
// 5. WIP - monte carlo estimator (indirect light)
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

// Max recursion depth for indirect light
const int maxBounces = 3;

// Helps remove shadow acne (dark noise) by moving the intersection point by a small distance away from the intersecting surface
// Can displace the shadows location by a small amount as an effect from displacing the intersection point
// theory from scratchapixel.com (link: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/ligth-and-shadows)
float shadowBias = 1e-4;

// Forward declaration for getIndirectLight()
ColorDbl castRay(Ray& ray, int currentBounces);

// To be deprecated
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

void findIntersectionPoint(Ray& firstRay, Triangle& intersectingTriangle, bool& isIntersectingMirror) {
	float t_nearest = INFINITY;

	// Loop through all triangles in scene
	for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
		Triangle currentTriangle = *it;

		if (currentTriangle.getIntersectionPoint(firstRay, t_nearest)) {
			isIntersectingMirror = false;
			intersectingTriangle = currentTriangle;

			firstRay.rgb = getLightColor(firstRay, scene.lightSource, currentTriangle);

			// Remove shadow acne
			firstRay.endPoint += currentTriangle.normal.direction * shadowBias;
		}
	}

	// Check if nearest intersection is on the sphere
	if (scene.sphere.getIntersectionPoint(firstRay, t_nearest)) {
		isIntersectingMirror = true;
		glm::vec3 sphereNorm = glm::normalize(firstRay.endPoint - scene.sphere.position);
		glm::vec3 lightDirection = glm::normalize(scene.lightSource - firstRay.endPoint);

		/* Send a reflection ray to get color from reflecting objects around the sphere
		 * because the sphere uses a perfect reflection material */

		/*************** TODO: move to castRay? ****************/
		// Create a reflected ray
		Ray reflectionRay;
		reflectionRay.startPoint = firstRay.endPoint;
		reflectionRay.direction.direction = glm::vec3(firstRay.direction.direction - sphereNorm * (2.0f * glm::dot(firstRay.direction.direction, sphereNorm)));

		//castRay(reflectionRay);
		/******* TODO: let castRay indirectly handle? *******/

		// Loop through all other objects in the scene (i.e. currently only triangles) and find nearest intersectionpoint for the reflected ray
		// Then get the color from the found intersectionpoint and use it on the sphere
		for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
			Triangle currentTriangle = *it;

			t_nearest = INFINITY;
			if (currentTriangle.getIntersectionPoint(reflectionRay, t_nearest)) {
				intersectingTriangle = currentTriangle;
				reflectionRay.rgb = getLightColor(reflectionRay, scene.lightSource, currentTriangle);
			}
		}

		/*******************************/

		firstRay.rgb = reflectionRay.rgb;

		// Removes shadow acne
		firstRay.endPoint += sphereNorm * shadowBias;
	}
}

// Shadow rays
ColorDbl getDirectLight(Ray& firstRay) {
	/* When nearest color for nearest intersection is found, check if occluded by other objects in the scene i.e. no illumination from the direct light */

	// Create the shadow ray
	Ray shadowRay = Ray();
	shadowRay.startPoint = firstRay.endPoint;
	shadowRay.endPoint = scene.lightSource;
	shadowRay.direction.direction = glm::normalize(shadowRay.endPoint - shadowRay.startPoint);

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
	if (isOccluded) {
		firstRay.rgb = ColorDbl(0.0, 0.0, 0.0);
	}

	return firstRay.rgb;
}

// Monte Carlo estimator
ColorDbl getIndirectLight(Ray firstRay, Triangle intersectingTriangle, int currentBounces) {
	// Create local coordinate system:
	// - surface normal
	// - projected ray onto surface plane
	// - crossproduct between previous two vectors
	// Create transformation matrix M with found basis axis of local system and the intersection point (local origin)
	// Get new ray direction
	// - randomize values for theta & phi (new reflected ray direction)
	// - use spherical-to-carteisan coordinates equations on theta & phi to get reflected direction in carteisan coordinates
	// - translate the reflected ray from local to global coordinates with the inverse of M
	// Cast the reflected ray into scene and repeat previous process
	// - return when X bounces has been made (threshold reached)
	// Compute outgoing light from intersection point (direct+indirect light)*albedo
	// When back at firstRay (pixels final color), divide total accumulated light with N (samples)

	ColorDbl accumulatedIndirectLight(0.0,0.0,0.0);

	// Create local system
	glm::vec3 localSysAxisY = intersectingTriangle.normal.direction;
	// From incoming ray
	glm::vec3 localSysAxisX = glm::normalize(firstRay.direction.direction - intersectingTriangle.normal.direction * (firstRay.direction.direction, intersectingTriangle.normal.direction));
	glm::vec3 localSysAxisZ = glm::cross(localSysAxisX, localSysAxisY);

	// Get world to local transformation matrix
	glm::mat4 M_1 = { localSysAxisX.x, localSysAxisY.x, localSysAxisZ.x, 0.0f,
					  localSysAxisX.y, localSysAxisY.y, localSysAxisZ.y, 0.0f,
					  localSysAxisX.z, localSysAxisY.z, localSysAxisZ.z, 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f };
	glm::mat4 M_2 = { 1.0f, 0.0f, 0.0f, -firstRay.endPoint.x,
					  0.0f, 1.0f, 0.0f, -firstRay.endPoint.y,
					  0.0f, 0.0f, 1.0f, -firstRay.endPoint.z,
					  0.0f, 0.0f, 0.0f, 1.0f };
	glm::mat4 M = M_1 * M_2;

	// Randomize new reflected ray direction
	float theta = ((double)rand() / (RAND_MAX)) * M_PI / 2;
	float phi = ((double)rand() / (RAND_MAX)) * 2 * M_PI;

	// Convert randomized spherical coordinates to carteisan coordinates in the local system	
	float x_cart = glm::cos(phi) * glm::sin(theta); // x = cos phi * sin theta
	float y_cart = glm::sin(phi) * glm::sin(theta); // y = sin phi * sin theta
	float z_cart = glm::cos(theta); // z = cos theta

	// Convert ray coordinates to global system
	glm::vec4 reflected_local = { x_cart, y_cart, z_cart, 1.0f };
	glm::vec3 reflected_global = reflected_local * glm::inverse(M);

	// Create new reflected ray
	Ray reflectedRay;
	reflectedRay.startPoint = firstRay.endPoint;
	reflectedRay.direction = glm::normalize(reflected_global);

	// Follow new ray for indirect light recursively
	// TODO: make sure indirectlight gets accumulated between bounces
	ColorDbl temp = castRay(reflectedRay, currentBounces);
	accumulatedIndirectLight.R += temp.R;
	accumulatedIndirectLight.G += temp.G;
	accumulatedIndirectLight.B += temp.B;
	//return accumulatedIndirectLight;
	//return castRay(reflectedRay, currentBounces);

	return ColorDbl(0.001, 0.0, 0.0);
	//return reflectedRay.rgb;
}

// Shoot a ray into the scene and get the intersecting points color
ColorDbl castRay(Ray& ray, int currentBounces) {
	// Max depth in recursion for indirect light
	currentBounces++;

	if (currentBounces > maxBounces) {
		return ray.rgb;
	}

	bool isIntersectingMirror = false;
	Triangle intersectingTriangle;

	// Find nearest intersection point in scene for ray
	findIntersectionPoint(ray, intersectingTriangle, isIntersectingMirror);

	// If intersecting mirror, cast reflected ray and skip shadowray/montecarlo
	if (isIntersectingMirror) {
		return castRay(ray, currentBounces);
	}
	else {
		// Shadow Rays
		ColorDbl directDiffuse = getDirectLight(ray);

		// Monte carlo estimator
		ColorDbl indirectDiffuse = getIndirectLight(ray, intersectingTriangle, currentBounces);

		// Combine direct and indirect light for final color in this ray
		// Scratchapixel.com: hitColor = (directDiffuse / M_PI + 2 * indirectDiffuse) * object->albedo;
		//ray.rgb.R = (directDiffuse.R / M_PI + 2 * indirectDiffuse.R) * intersectingTriangle.rgb.R;
		//ray.rgb.G = (directDiffuse.G / M_PI + 2 * indirectDiffuse.G) * intersectingTriangle.rgb.G;
		//ray.rgb.B = (directDiffuse.B / M_PI + 2 * indirectDiffuse.B) * intersectingTriangle.rgb.B;
		return ColorDbl((directDiffuse.R / M_PI + 2 * indirectDiffuse.R) * intersectingTriangle.rgb.R, (directDiffuse.G / M_PI + 2 * indirectDiffuse.G) * intersectingTriangle.rgb.G, (directDiffuse.B / M_PI + 2 * indirectDiffuse.B) * intersectingTriangle.rgb.B);
	}
}

void rendersegment(int s, int e) {
	// Loop through all pixels in image plane
	int i, j;
	for (i = s; i < e; i++) {
		for (j = 0; j < HEIGHT; j++) {

			int currentBounces = 0;

			// Get pixelcoords from pixel index in image
			glm::vec3 pixelCoord = glm::vec3(0.0, (j - 401.0 + ((double)rand() / (RAND_MAX))) * 0.0025, (i - 401.0 + ((double)rand() / (RAND_MAX))) * 0.0025);

			// Get rays direction from camera and pixel coordinates
			Ray firstRay;
			firstRay.startPoint = cameraPos;
			firstRay.direction = glm::normalize(pixelCoord - firstRay.startPoint);

			// Cast ray into scene and get the pixels color
			ColorDbl finalColor = castRay(firstRay, currentBounces);

			// Store found color of pixel in rendered image
			intensityImage[i][j][2] = finalColor.R * 255;
			intensityImage[i][j][1] = finalColor.G * 255;
			intensityImage[i][j][0] = finalColor.B * 255;
		}
	}
}

int main()
{
	srand(static_cast <unsigned> (time(0))); // Init rand

	// Init image file
	static unsigned char image[HEIGHT][WIDTH][BYTES_PER_PIXEL];
	char* imageFileName = (char*)"bitmapImage.bmp";

	// TODO: Render the scene from camera
	//camera.render(scene);

	// Draw/store image
	std::cout << "Rendering image..." << std::endl;

	// Utilize multiple threads in CPU for faster rendering time
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

	// Find the highest intensity value in image plane
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
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
// 4. DONE? - lambertian
// 5. monte carlo estimator (indirect light)
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

void rendersegment(int s, int e) {
	int i, j;
	double i_max = 0;
	for (i = s; i < e; i++) {
		for (j = 0; j < HEIGHT; j++) {

			// Get pixelcoords from pixel index in image
			glm::vec3 pixelCoord = glm::vec3(0.0, (j - 401.0 + ((double)rand() / (RAND_MAX))) * 0.0025, (i - 401.0 + ((double)rand() / (RAND_MAX))) * 0.0025);

			/**************** Casting rays ******************/

			/*** Get nearest intersection from camera on a scene object ***/
			// Get rays direction from camera and pixel coordinates
			Ray firstRay;
			firstRay.startPoint = cameraPos;
			firstRay.direction = glm::normalize(pixelCoord - firstRay.startPoint);

			//float t;
			float t_nearest = INFINITY;
			bool isIntersectingMirror = false;
			Triangle intersectingTriangle;

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

			// Add sphere into scene
			Sphere sphere;
			sphere.position = glm::vec3(4.0, -2.0, 1.0);
			sphere.radius = 1.0;

			// Check if nearest intersection is on the sphere
			if (sphere.getIntersectionPoint(firstRay, t_nearest)) {
				isIntersectingMirror = true;
				glm::vec3 sphereNorm = glm::normalize(firstRay.endPoint - sphere.position);
				glm::vec3 lightDirection = glm::normalize(scene.lightSource - firstRay.endPoint);

				/* Send a reflection ray to get color from reflecting objects around the sphere 
				 * because the sphere uses a perfect reflection material */
				
				// Create a reflected ray
				Ray reflectionRay;
				reflectionRay.startPoint = firstRay.endPoint;
				reflectionRay.direction.direction = glm::vec3(firstRay.direction.direction - sphereNorm * (2.0f * glm::dot(firstRay.direction.direction, sphereNorm)));				

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
				firstRay.rgb = reflectionRay.rgb;

				// Removes shadow acne
				firstRay.endPoint += sphereNorm * shadowBias;
			}

			/*********** Shadow Rays (direct light) *************/
			/* When nearest color for nearest intersection is found, check if occluded by other objects in the scene i.e. no illumination from the direct light */
			
			// Create the shadow ray
			Ray shadowRay = Ray();
			shadowRay.startPoint = firstRay.endPoint;
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
			if (sphere.getIntersectionPoint(shadowRay, temp_t) && 0 < temp_t && temp_t < distanceRaystartToLight) {
				isOccluded = true;
			}

			// Change color of ray to black because occluded by other object(s)
			if (isOccluded && !isIntersectingMirror) {
				firstRay.rgb = ColorDbl(0.0, 0.0, 0.0);
			}

			/******************************************************/

			/************* Monte carlo estimator (indirect light) ************/

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

			glm::vec3 accumulatedIndirectLight = glm::vec3(0.0, 0.0, 0.0);
			int currentBounces = 0;
			const int maxBounces = 1;

			// Create local system
			glm::vec3 localSysAxisY = intersectingTriangle.normal.direction;
			// From incoming ray
			glm::vec3 localSysAxisX = glm::normalize(firstRay.direction.direction - intersectingTriangle.normal.direction * (firstRay.direction.direction, intersectingTriangle.normal.direction));
			glm::vec3 localSysAxisZ = glm::cross(localSysAxisX, localSysAxisY);

			// Transformation matrix
			glm::mat4 M_1 = { localSysAxisX.x, localSysAxisY.x, localSysAxisZ.x, 0.0f,
							  localSysAxisX.y, localSysAxisY.y, localSysAxisZ.y, 0.0f,
							  localSysAxisX.z, localSysAxisY.z, localSysAxisZ.z, 0.0f,
							  0.0f, 0.0f, 0.0f, 1.0f };

			glm::mat4 M_2 = { 1.0f, 0.0f, 0.0f, -firstRay.endPoint.x,
							  0.0f, 1.0f, 0.0f, -firstRay.endPoint.y,
							  0.0f, 0.0f, 1.0f, -firstRay.endPoint.z,
							  0.0f, 0.0f, 0.0f, 1.0f };
			
			glm::mat4 M = M_1 * M_2;

			float theta = ((double)rand() / (RAND_MAX)) * M_PI /2;
			float phi = ((double)rand() / (RAND_MAX)) * 2 * M_PI;

			// x = cos phi * sin theta
			// y = sin phi * sin theta
			// z = cos theta
			float x_cart = glm::cos(phi) * glm::sin(theta);
			float y_cart = glm::sin(phi) * glm::sin(theta);
			float z_cart = glm::cos(theta);

			glm::vec4 reflected_local = { x_cart, y_cart, z_cart, 1.0f };
			glm::vec3 reflected_global = reflected_local * glm::inverse(M);

			// Create new ray
			Ray reflectedRay;
			reflectedRay.startPoint = firstRay.endPoint;
			reflectedRay.direction = glm::normalize(reflected_global);

			//float t;
			t_nearest = INFINITY;
			//bool isIntersectingMirror = false;
			//Triangle intersectingTriangle;

			// Loop through all triangles in scene
			for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
				Triangle currentTriangle = *it;

				if (currentTriangle.getIntersectionPoint(reflectedRay, t_nearest)) {
					//isIntersectingMirror = false;
					//intersectingTriangle = currentTriangle;

					reflectedRay.rgb = getLightColor(reflectedRay, scene.lightSource, currentTriangle);

					// Remove shadow acne
					reflectedRay.endPoint += currentTriangle.normal.direction * shadowBias;
				}
			}

			firstRay.rgb.R = (firstRay.rgb.R + reflectedRay.rgb.R) * intersectingTriangle.rgb.R;
			firstRay.rgb.G = (firstRay.rgb.G + reflectedRay.rgb.G) * intersectingTriangle.rgb.G;
			firstRay.rgb.B = (firstRay.rgb.B + reflectedRay.rgb.B) * intersectingTriangle.rgb.B;

			// from scratchaipxel
			// Vec3f Nt, Nb; 
			/*void createCoordinateSystem(const Vec3f & N, Vec3f & Nt, Vec3f & Nb)
			{
				if (std::fabs(N.x) > std::fabs(N.y))
					Nt = Vec3f(N.z, 0, -N.x) / sqrtf(N.x * N.x + N.z * N.z);
				else
					Nt = Vec3f(0, -N.z, N.y) / sqrtf(N.y * N.y + N.z * N.z);
				Nb = N.crossProduct(Nt);
			}*/
			/*float pdf = 1 / (2 * M_PI); 
                for (uint32_t n = 0; n < N; ++n) { 
                    float r1 = distribution(generator); 
                    float r2 = distribution(generator); 
                    Vec3f sample = uniformSampleHemisphere(r1, r2); 
                    Vec3f sampleWorld( 
                        sample.x * Nb.x + sample.y * hitNormal.x + sample.z * Nt.x, 
                        sample.x * Nb.y + sample.y * hitNormal.y + sample.z * Nt.y, 
                        sample.x * Nb.z + sample.y * hitNormal.z + sample.z * Nt.z); 
                    // don't forget to divide by PDF and multiply by cos(theta)
                    indirectLigthing += r1 * castRay(hitPoint + sampleWorld * options.bias, 
                        sampleWorld, objects, lights, options, depth + 1) / pdf; 
                } 
                // divide by N
                indirectLigthing /= (float)N; 
				*/

			/******************************************************/

			/**************** Casting rays - end ******************/
			
			// Store found color of pixel in rendered image
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
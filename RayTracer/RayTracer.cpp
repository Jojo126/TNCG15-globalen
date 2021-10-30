#include <iostream>
#include "Scene.h"
#include "Camera.h"
#include "BmpSave.h"
#include "Vertex.h"
#include <math.h>
#include <glm/glm.hpp>

int main()
{
	// Creates an empty room
	Scene scene;

	// TODO: Create a camera
	Camera camera;
	const Vertex cameraPos = camera.eye1;

	// Specs for bitmap
	const int HEIGHT = 800;
	const int WIDTH = 800;
	static unsigned char image[HEIGHT][WIDTH][BYTES_PER_PIXEL];
	char* imageFileName = (char*) "bitmapImage.bmp";

	// TODO: Render the scene
	//camera.render(scene);

	/*std::vector<Vertex> imagePlaneCoords = std::vector<Vertex>();
	imagePlaneCoords.push_back(Vertex(0.0, -1.0, -1.0, 1.0));
	imagePlaneCoords.push_back(Vertex(0.0, 1.0, -1.0, 1.0));
	imagePlaneCoords.push_back(Vertex(0.0, 1.0, 1.0, 1.0));
	imagePlaneCoords.push_back(Vertex(0.0, -1.0, 1.0, 1.0));*/

	// Draw/store image
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			//image[i][j][2] = (unsigned char)(i * 255 / HEIGHT);					///red
			//image[i][j][1] = (unsigned char)(j * 255 / WIDTH);				    ///green
			//image[i][j][0] = (unsigned char)((i + j) * 255 / (HEIGHT + WIDTH)); ///blue


			Vertex pixelCoord = Vertex(0.0, (i - 401.0)*0.0025, (j - 401.0) * 0.0025, 1.0);
			Vertex startPoint = cameraPos;

			Vertex rayDirection = pixelCoord - startPoint;
			Direction rayDir = Direction(rayDirection.x, rayDirection.y, rayDirection.z);
			Direction rayStart = Direction(cameraPos.x, cameraPos.y, cameraPos.z); // origo
			
			
			// Loopa igenom alla trianglar i scenen
			for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {

				Triangle current = *it;

				// Marks method:
				Direction v0 = Direction(current.v1.x, current.v1.y, current.v1.z);
				Direction v1 = Direction(current.v2.x, current.v2.y, current.v2.z);
				Direction v2 = Direction(current.v3.x, current.v3.y, current.v3.z);

				Direction T = Direction(cameraPos.x, cameraPos.y, cameraPos.z) - v0; // T = P_s - v_0
				Direction E1 = v1 - v0;
				Direction E2 = v2 - v0;
				Direction D = rayDir - Direction(cameraPos.x, cameraPos.y, cameraPos.z);
				Direction P = D.crossProduct(E2);
				Direction Q = T.crossProduct(E1);

				double mark_t = Q.dotProduct(E2) / P.dotProduct(E1);

				if (P.dotProduct(E1) != 0) {
					double u = P.dotProduct(T) / P.dotProduct(E1);
					double v = Q.dotProduct(D) / P.dotProduct(E1);

					/*
					std::cout << "P.dotProduct(T): " << P.dotProduct(T) << ", Q.dotProduct(D): " << Q.dotProduct(D) << std::endl;
					std::cout << "denominator = " << P.dotProduct(E1) << std::endl;
					std::cout << "(" << u << "," << v << "," << u + v << ")" << std::endl;
					*/

					if (u >= 0 && v >= 0 && u + v <= 1) {

						ColorDbl color = current.rgb;

						image[i][j][2] = color.R; //(unsigned char)(i * 255 / HEIGHT);					///red
						image[i][j][1] = color.G; //(unsigned char)(j * 255 / WIDTH);				    ///green
						image[i][j][0] = color.B; //(unsigned char)((i + j) * 255 / (HEIGHT + WIDTH)); ///blue
					}
				}

				/*float denominator = current.normal.dotProduct(rayDir);
				if (denominator != 0) { // if ray not parallell with triangle
					float D = current.normal.dotProduct(Direction(current.v1.x, current.v1.y, current.v1.z));
					float t = -(current.normal.dotProduct(rayStart) + D) / denominator; // P = O + t*R (R = ray vektorn)
					
					// Unders�k ifall triangel bakom kameran
					if (t > 0) {*/
						//Direction P_hit = rayStart + (rayDir*t); // punkten d�r ray sk�r traingeln
				/*
						Direction v0 = Direction(current.v1.x, current.v1.y, current.v1.z);
						Direction v1 = Direction(current.v2.x, current.v2.y, current.v2.z);
						Direction v2 = Direction(current.v3.x, current.v3.y, current.v3.z);*/
						
						//std::cout << "v0: (" << v0.x << "," << v0.y << "," << v0.z << ")" << std::endl;
						//std::cout << "v1: (" << v1.x << "," << v1.y << "," << v1.z << ")" << std::endl;
						//std::cout << "v2: (" << v2.x << "," << v2.y << "," << v2.z << ")" << std::endl;
						/*Vertex edge0 = current.v2 - current.v1;
						Vertex edge1 = current.v3 - current.v2;
						Vertex edge2 = current.v1 - current.v3;
						Direction e0 = Direction(edge0.x, edge0.y, edge0.z);
						Direction e1 = Direction(edge1.x, edge1.y, edge1.z);
						Direction e2 = Direction(edge2.x, edge2.y, edge2.z);
						Direction C0 = P - Direction(current.v1.x, current.v1.y, current.v1.z);
						Direction C1 = P - Direction(current.v2.x, current.v2.y, current.v2.z);
						Direction C2 = P - Direction(current.v3.x, current.v3.y, current.v3.z);*/

						//Direction P_hit = rayStart + (rayDir * t);

						/*Direction T = Direction(camera.eye1.x, camera.eye1.y, camera.eye1.z) - v0; // T = P_s - v_0
						Direction E1 = v1 - v0;
						Direction E2 = v2 - v0;
						Direction D = rayDir - Direction(camera.eye1.x, camera.eye1.y, camera.eye1.z);
						Direction P = D.crossProduct(E2);
						Direction Q = T.crossProduct(E1);

						double mark_t = Q.dotProduct(E2) / P.dotProduct(E1);*/
						//std::cout << "mark_t: " << mark_t << std::endl;
						//std::cout << "our t: " << t << std::endl;

						
						

						/*if (P.dotProduct(E1) != 0) {
							double u = P.dotProduct(T) / P.dotProduct(E1);
							double v = Q.dotProduct(D) / P.dotProduct(E1);

							
							std::cout << "P.dotProduct(T): " << P.dotProduct(T) << ", Q.dotProduct(D): " << Q.dotProduct(D) << std::endl;
							std::cout << "denominator = " << P.dotProduct(E1) << std::endl;
							std::cout << "(" << u << "," << v << "," << u + v << ")" << std::endl;
							

							if (u >= 0 && v >= 0 && u + v <= 1) {

								ColorDbl color = current.rgb;

								image[i][j][2] = color.R; //(unsigned char)(i * 255 / HEIGHT);					///red
								image[i][j][1] = color.G; //(unsigned char)(j * 255 / WIDTH);				    ///green
								image[i][j][0] = color.B; //(unsigned char)((i + j) * 255 / (HEIGHT + WIDTH)); ///blue
							}
						}*/
						

						/*if (current.normal.dotProduct(e0.crossProduct(C0)) > 0 &&
							current.normal.dotProduct(e1.crossProduct(C1)) > 0 &&
							current.normal.dotProduct(e2.crossProduct(C2)) > 0) {*/ // P is inside the triangle
							///std::cout << "Entering -P inside the triangle-" << std::endl;
							// H�mta f�rgv�rde
							
							//image[i][j][2] = 1; ///red
							//image[i][j][1] = 255; ///green
							//image[i][j][0] = 255; ///blue

							//std::cout << color << std::endl;
						//}

					//}
				//}
				
			
			}

			//Triangle iter = scene.mTriangles.next();

			// Bildplanets coordinater: 

			// TODO: get corresponding colorDbl
			// 1. berkna rays riktning fr�n camera�gat till pixelns koordinat
			//Ray pixelRay = Ray(camera.eye1);
			
			//pixelns koordinat. i,j [-1,1] +-1/400, y,z
			// y = i/400, z = j/400  
			//camera.eye1
		}
	}

	// Save image into bitmap
	generateBitmapImage((unsigned char*)image, HEIGHT, WIDTH, imageFileName);
	printf("Image generated!!");
}
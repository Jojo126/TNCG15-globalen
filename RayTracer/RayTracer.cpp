#include <iostream>
#include "Scene.h"
#include "Camera.h"
#include "BmpSave.h"
#include "Vertex.h"
#include <glm/glm.hpp>

int main()
{
	// Creates an empty room
	Scene scene;

	// TODO: Create a camera
	Camera camera;

	// Specs for bitmap
	//const int HEIGHT = 800;
	//const int WIDTH = 800;
	const int HEIGHT = 10;
	const int WIDTH = 10;
	static unsigned char image[HEIGHT][WIDTH][BYTES_PER_PIXEL];
	char* imageFileName = (char*) "bitmapImage.bmp";

	// TODO: Render the scene
	//camera.render(scene);

	std::vector<Vertex> imagePlaneCoords = std::vector<Vertex>();
	imagePlaneCoords.push_back(Vertex(0.0, -1.0, -1.0, 1.0));
	imagePlaneCoords.push_back(Vertex(0.0, 1.0, -1.0, 1.0));
	imagePlaneCoords.push_back(Vertex(0.0, 1.0, 1.0, 1.0));
	imagePlaneCoords.push_back(Vertex(0.0, -1.0, 1.0, 1.0));

	// Draw/store image
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			image[i][j][2] = (unsigned char)(i * 255 / HEIGHT);					///red
			image[i][j][1] = (unsigned char)(j * 255 / WIDTH);				    ///green
			image[i][j][0] = (unsigned char)((i + j) * 255 / (HEIGHT + WIDTH)); ///blue

			float y = -1.0 + float(i) / WIDTH;
			float z = -1.0 + float(j) / HEIGHT;

			Vertex pixelCoord = Vertex(0.0, y, z, 1.0);
			Vertex startPoint = camera.eye1;

			Vertex rayDirection = pixelCoord - startPoint;

			for (std::vector<Triangle>::iterator it = scene.mTriangles.begin(); it != scene.mTriangles.end(); ++it) {
				Triangle current = *it;
				//current.normal;
			
			}

			//Triangle iter = scene.mTriangles.next();

			// Bildplanets coordinater: 

			// TODO: get corresponding colorDbl
			// 1. berkna rays riktning från cameraögat till pixelns koordinat
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
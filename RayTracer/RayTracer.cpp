#include <iostream>
#include "BmpSave.h"
#include "Camera.h"
#include "Vertex.h"
#include "Pixel.h"

int main()
{
	Camera myCamera(Vertex(-2,0,0,1),Vertex(-1,0,0,1), true);

	// Specs for bitmap
	const int HEIGHT = 800;
	const int WIDTH = 800;
	static unsigned char image[HEIGHT][WIDTH][BYTES_PER_PIXEL];
	char* imageFileName = (char*) "bitmapImage.bmp";

	// Draw/store image
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			// intensiteterna behöver ändras till att trunkeras genom att hitta i_max och sen multiplicera kanalerna med 255.99/i_max och avrunda talet neråt
			// i_max = högsta intensiteten funnen mellan alla de tre färg-kanalerna rgb 
			image[i][j][2] = (unsigned char)(i * 255 / HEIGHT);					///red
			image[i][j][1] = (unsigned char)(j * 255 / WIDTH);				    ///green
			image[i][j][0] = (unsigned char)((i + j) * 255 / (HEIGHT + WIDTH)); ///blue
		}
	}

	// Save image into bitmap
	generateBitmapImage((unsigned char*)image, HEIGHT, WIDTH, imageFileName);
	printf("Image generated!!");
}
#include <iostream>
#include "BmpSave.h"

int main()
{
	// Specs for bitmap
	const int HEIGHT = 800;
	const int WIDTH = 800;
	static unsigned char image[HEIGHT][WIDTH][BYTES_PER_PIXEL];
	char* imageFileName = (char*) "bitmapImage.bmp";

	// Draw/store image
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		for (j = 0; j < WIDTH; j++) {
			image[i][j][2] = (unsigned char)(i * 255 / HEIGHT);					///red
			image[i][j][1] = (unsigned char)(j * 255 / WIDTH);				    ///green
			image[i][j][0] = (unsigned char)((i + j) * 255 / (HEIGHT + WIDTH)); ///blue
		}
	}

	// Save image into bitmap
	generateBitmapImage((unsigned char*)image, HEIGHT, WIDTH, imageFileName);
	printf("Image generated!!");
}
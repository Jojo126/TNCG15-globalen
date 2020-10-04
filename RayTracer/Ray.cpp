#include "Ray.h"

Ray::Ray(Vertex startP, int i, int j) {
	startPoint = startP;
	//triangle = ;
	double deltaY = rand() / RAND_MAX, deltaZ = rand() / RAND_MAX;
	endPoint = Vertex(0.0, (i - 401 + deltaY) * 0.0025, (j - 401 + deltaZ) * 0.0025, 1.0); // slutar på pixeln (i,j)
	//triangle = dummyTriangle;
}
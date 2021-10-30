#include "Ray.h"


/*Ray::Ray(Vertex startP, int i, int j) {

	startPoint = glm::vec3(startP.coords);
	//triangle = ;

	double deltaY = rand() / RAND_MAX, deltaZ = rand() / RAND_MAX;

	endPoint = Vertex(0.0, (i - 401 + deltaY) * 0.0025, (j - 401 + deltaZ) * 0.0025, 1.0); // slutar på pixeln (i,j)

	// sqrt(a*a + b*b)
	//dir = Direction(endPoint-startPoint).normalize();
	Vertex tempDir = Vertex(endPoint - startPoint);
	dir = Direction(tempDir.x, tempDir.y, tempDir.z);

	dir.x /= sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	dir.y /= sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	dir.z /= sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
}*/
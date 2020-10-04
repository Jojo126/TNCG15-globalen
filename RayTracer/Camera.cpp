#include "Camera.h"

void Camera::render() {

	for (int i = 0; i < 800; i++) {
		for (int j = 0; j < 800; j++) {

			if (useFirstEye)
				Ray::Ray(eye1, i, j);
			else
				Ray::Ray(eye2, i, j);
				
		}
	}
}

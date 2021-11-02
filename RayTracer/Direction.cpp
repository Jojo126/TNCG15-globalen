#include "Direction.h"

Direction Direction::operator*(float inD) {
	this->direction.x *= inD;
	this->direction.y *= inD;
	this->direction.z *= inD;

	return *this;
}

/*Direction Direction::crossProduct(Direction inD) {
	
	Direction crossP;
	crossP.direction.x = this->direction.y * inD.direction.z - this->direction.z * inD.direction.y;
	crossP.direction.y = this->direction.z * inD.direction.x - this->direction.x * inD.direction.z;
	crossP.direction.z = this->direction.x * inD.direction.y - this->direction.y * inD.direction.x;

	return crossP;
}

double Direction::dotProduct(Direction inD) {
	// x1*x2 + y1*y2 + z1*z2
	return this->direction.x * inD.x + this->y * inD.y + this->z * inD.z;
}

Direction Direction::operator+(Direction inD) {
	this->x += inD.x;
	this->y += inD.y;
	this->z += inD.z;

	return *this;
}

Direction Direction::operator-(Direction inD) {
	this->x -= inD.x;
	this->y -= inD.y;
	this->z -= inD.z;

	return *this;
}*/
#include "Direction.h"

Direction Direction::crossProduct(Direction inD) {
	this->x = this->x * inD.x;
	this->y = this->y * inD.y;
	this->z = this->z * inD.z;

	return *this;
}

double Direction::dotProduct(Direction inD) {
	// x1*x2 + y1*y2 + z1*z2
	return this->x * inD.x + this->y * inD.y + this->z * inD.z;
}

Direction Direction::operator*(float inD) {
	this->x *= inD;
	this->y *= inD;
	this->z *= inD;

	return *this;
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
}
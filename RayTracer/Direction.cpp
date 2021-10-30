#include "Direction.h"

Direction Direction::crossProduct(Direction inD) {
	
	Direction crossP;
	/*this->x = this->x * inD.x;
	this->y = this->y * inD.y;
	this->z = this->z * inD.z;*/

	/*cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
	cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
	cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];*/
	crossP.x = this->y * inD.z - this->z * inD.y;
	crossP.y = this->z * inD.x - this->x * inD.z;
	crossP.z = this->x * inD.y - this->y * inD.x;

	return crossP;
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
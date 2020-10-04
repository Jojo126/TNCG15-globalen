#include "Vertex.h"

Vertex Vertex::operator-(Vertex inV) {
	this->x -= inV.x;
	this->y -= inV.y;
	this->z -= inV.z;

	return *this;
}
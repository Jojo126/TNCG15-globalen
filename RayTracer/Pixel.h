/*
Pixel contains one instance of ColorDbl that holds the color and intensity
for this pixel with a high dynamic range.
Pixel has references to the rays that go through it. We use for now one.*/

#pragma once
#include "ColorDbl.h"
#include "Ray.h"

class Pixel
{
	private:
		ColorDbl rgb;
		Ray& interRays;
};


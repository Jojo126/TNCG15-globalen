/*
It has three double precision variables that contain the intensities in the
red, green and blue channel.
*/

#pragma once

class ColorDbl
{
	private:
		double R, G, B;
	public:
		// Constructors
		ColorDbl() : R{ 1.0 }, G{ 1.0 }, B{ 1.0 } {};
		ColorDbl(double R, double G, double B);
		
		// Destructors
		~ColorDbl();
};


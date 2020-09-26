/*
It has three double precision variables that contain the intensities in the
red, green and blue channel.
*/

#pragma once

class ColorDbl
{
	public:
		double R, G, B;

		ColorDbl() 
		: R{ 1.0 }, G{ 1.0 }, B{ 1.0 }{};

		ColorDbl(double R, double G, double B)
		: R{ R }, G{ G }, B{ B } {};
		
		~ColorDbl() {};
};


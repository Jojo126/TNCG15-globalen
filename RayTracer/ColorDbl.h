/*
It has three double precision variables that contain the intensities in the
red, green and blue channel.
*/
#include <iostream>
#pragma once


class ColorDbl
{
	public:
		double R, G, B;

		ColorDbl() 
		: R{ 1.0 }, G{ 1.0 }, B{ 1.0 }{};

		ColorDbl(double R, double G, double B)
		: R{ R }, G{ G }, B{ B } {};
		
		friend std::ostream& operator<<(std::ostream& os, const ColorDbl& cd) {
			os << '(' << cd.R << ',' << cd.G << ',' << cd.B << ')';
			return os;
		}

		ColorDbl operator+=(ColorDbl inRGB) {
			this->R += inRGB.R;
			this->G += inRGB.G;
			this->B += inRGB.B;
			return *this;
		}

		~ColorDbl() {};
};


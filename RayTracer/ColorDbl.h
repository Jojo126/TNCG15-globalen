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

		ColorDbl operator+(ColorDbl rgb) {
			this->R += rgb.R;
			this->G += rgb.G;
			this->B += rgb.B;
			return *this;
		}

		ColorDbl operator*(double val) {
			this->R *= val;
			this->G *= val;
			this->B *= val;
			return *this;
		}

		~ColorDbl() {};
};


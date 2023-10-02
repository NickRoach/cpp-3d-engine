#include <math.h>
#include "typedefs.h"


int hsv2rgb(hsv HSV)
{
	rgb RGB;
	double H = HSV.h, S = HSV.s, V = HSV.v,
		P, Q, T,
		fract;

	(H == 360.) ? (H = 0.) : (H /= 60.);
	fract = H - floor(H);

	P = V * (1. - S);
	Q = V * (1. - S * fract);
	T = V * (1. - S * (1. - fract));

	if (0. <= H && H < 1.) {
		RGB.r = V;
		RGB.g = T;
		RGB.b = P;
	}
	else if (1. <= H && H < 2.) {
		RGB.r = Q;
		RGB.g = V;
		RGB.b = P;
	}
	else if (2. <= H && H < 3.) {
		RGB.r = P;
		RGB.g = V;
		RGB.b = T;
	}
	else if (3. <= H && H < 4.) {
		RGB.r = P;
		RGB.g = Q;
		RGB.b = V;
	}
	else if (4. <= H && H < 5.) {
		RGB.r = T;
		RGB.g = P;
		RGB.b = V;
	}
	else if (5. <= H && H < 6.) {
		RGB.r = V;
		RGB.g = P;
		RGB.b = Q;
	}
	else {
		RGB.r = 0.;
		RGB.g = 0.;
		RGB.b = 0.;
	};

	int rgbHex;
	rgbHex = RGB.r * 256;
	rgbHex <<= 8;
	rgbHex += RGB.g * 256;
	rgbHex <<= 8;
	rgbHex += RGB.b * 256;

	return rgbHex;
}
#include "typedefs.h"

hsv makeHsvData(float h) {
	hsv data;
	data.h = h;
	data.s = 1;
	data.v = 0.9;
	return data;
}
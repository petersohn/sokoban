#include "Point.h"

const Point Point::p10(1, 0);
const Point Point::p01(0, 1);
const Point Point::pm10(-1, 0);
const Point Point::p0m1(0, -1);

size_t hash_value(const Point &p) {
	size_t seed = 0;
	boost::hash_combine(seed, p.x);
	boost::hash_combine(seed, p.y);
	return seed;
}



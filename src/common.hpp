#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// usings
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
	// convert value in degrees and return value in radians
	return degrees * pi / 180.0;
}

inline double random_double() {
	// generate a random double-precision number in the range [0, 1)
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	// generate a random double-precision number in the range [min, max)
	return min + (max - min) * random_double();
}

#endif
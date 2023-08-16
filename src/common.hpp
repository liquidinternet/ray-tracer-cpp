#pragma once
#include <memory>
#include <numbers>
#include <random>

// common usings
using std::make_shared;
using std::shared_ptr;

// common constants
const double infinity = std::numeric_limits<double>::infinity();

// convert value in degrees and return value in radians
inline double degreesToRadians(double degrees) {
	return degrees * (std::numbers::pi / 180.0);
}

// generate a random double-precision number in the range [0, 1)
inline double randomDouble() {
	// define a static distribution that generates random doubles in the range [0.0, 1.0)
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	// define a static random number generator initialised with a seed from std::random_device
    static std::mt19937 generator(std::random_device{}());
	// return random double using distribution and generator
    return distribution(generator);
}

// generate a random double-precision number in the range [min, max)
inline double randomDouble(double min, double max) {
	return min + (max - min) * randomDouble();
}

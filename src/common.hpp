#pragma once
#include <memory>
#include <numbers>
#include <random>

// usings
using std::make_shared;
using std::shared_ptr;

// constants
const double infinity = std::numeric_limits<double>::infinity();

inline double degreesToRadians(double degrees) {
	// convert value in degrees and return value in radians
	return degrees * (std::numbers::pi / 180.0);
}

inline double randomDouble() {
	// generate a random double-precision number in the range [0, 1)
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator(std::random_device{}());
    return distribution(generator);
}

inline double randomDouble(double min, double max) {
	// generate a random double-precision number in the range [min, max)
	return min + (max - min) * randomDouble();
}

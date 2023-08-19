#pragma once
#include "interval.hpp"
#include "vec3.hpp"
#include <iostream>

// colour is an alias for vec3, useful for code clarity
using colour = vec3;

// convert from linear to gamma colourspace using a square root transformation
// parameters:
//   linear_component: component to be converted
// returns:
//   gamma-corrected component
inline double linearToGamma(double linear_component) {
	return sqrt(linear_component);
}

// write a colour value to an output stream with gamma correction and scaling
// parameters:
//   out: the output stream to write to
//   pixel_colour: the colour value to be written
//   samples_per_pixel: the number of samples per pixel for scaling
void writeColour(std::ostream &out, colour pixel_colour, int samples_per_pixel) {
	// extract colour components
	auto r = pixel_colour.x();
	auto g = pixel_colour.y();
	auto b = pixel_colour.z();

	// divide colour by number of samples
	auto scale = 1.0 / samples_per_pixel;
	r *= scale;
	g *= scale;
	b *= scale;

	// apply linear to gamma correction
	r = linearToGamma(r);
	g = linearToGamma(g);
	b = linearToGamma(b);

	// write translated (0,255) value of each colour component
	static const interval intensity(0.000, 0.999);
	out << static_cast<int>(256 * intensity.clamp(r)) << " "
		<< static_cast<int>(256 * intensity.clamp(g)) << " "
		<< static_cast<int>(256 * intensity.clamp(b)) << "\n";

}

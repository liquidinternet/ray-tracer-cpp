#pragma once
#include "material.hpp"

// a derived class representing a metal material
class metal : public material {
public:

	// constructor to initialize the metal material
	// parameters:
	//   a: the albedo colour of the surface
	//   f: the fuzziness of the material (reflection blur), values larger than 1 result in perfect reflection
	metal(const colour &a, double f)
		: albedo(a), fuzz(f < 1 ? f : 1) { }

	// scatter function for simulating interaction between a ray and a material
	// parameters:
	//   r_in: the ray
	//   rec: the record containing intersection information
	//   attenuation: the color attenuation due to the material's interaction
	//   scattered: the scattered ray after interaction with the material
	// returns:
	//   true if scattering occurs (ray is absorbed and/or redirected), else false
	bool scatter(const ray &r_in, const hit_record &rec, colour &attenuation, ray &scattered) const override {
		// get unit vector from ray direction
		auto unit_vector = unitVector(r_in.direction());
		// calculate reflected direction based on ray direction and surface normal
		auto reflected = reflect(unit_vector, rec.normal);
		// calculate a random scattering direction with a slight random deviation (fuzziness) for reflection blur
		auto scatter_direction = reflected + fuzz * randomUnitVector();
		// create the scattered ray
		scattered = ray(rec.p, scatter_direction);
		// set attenuation to the material's albedo
		attenuation = albedo;
		// angle between the scattered ray direction and normal
		auto angle = dot(scattered.direction(), rec.normal);
        // returns true angle is positive (eg. above the surface)
		return (angle > 0);
	}

private:

	colour albedo;			// albedo colour of the metal
	double fuzz;			// fuzziness factor for reflection blur

};

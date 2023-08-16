#pragma once
#include "material.hpp"

// a derived class representing a diffuse material
class lambertian : public material {
public:

	// constructor to initialize the diffuse material
	// parameters:
	//   a: the albedo colour of the surface
	lambertian(const colour &a) : _albedo(a) {}

	// scatter function for simulating interaction between a ray and a material
	// parameters:
	//   r_in: the ray
	//   rec: the record containing intersection information
	//   attenuation: the color attenuation due to the material's interaction
	//   scattered: the scattered ray after interaction with the material
	// returns:
	//   true if scattering occurs (and it always does)
	bool scatter(const ray &r_in, const hit_record &rec, colour &attenuation, ray &scattered) const override {
		// calculate a random scattering direction using normal as base direction
		auto scatter_direction = rec.normal + randomUnitVector();
		// catch degenerate scatter direction
		if (scatter_direction.nearZero()) {
			scatter_direction = rec.normal;
		}
		// create the scattered ray
		scattered = ray(rec.p, scatter_direction);
		// set attenuation to the material's albedo
		attenuation = _albedo;
		// always scatters
		return true;
	}

private:

	colour _albedo;		// the albedo colour of the surface

};

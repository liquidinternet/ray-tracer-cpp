#pragma once
#include "hittable_list.hpp"

// an absract class representing materials
class material {
public:

	// scatter function for simulating interaction between a ray and a material
	// parameters:
	//   r_in: the ray
	//   rec: the record containing intersection information
	//   attenuation: the colour attenuation due to the material's interaction
	//   scattered: the scattered ray after interaction with the material
	// returns:
	//   true if scattering occurs (ray is absorbed and/or redirected), else false
	virtual bool scatter(const ray &r_in, const hit_record &rec, colour &attenuation, ray &scattered) const = 0;

	// destructor to ensure cleanup in derived classes
	virtual ~material() = default;

};

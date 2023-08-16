#pragma once
#include "material.hpp"

// a derived class representing a dielectric material
class dielectric : public material {
public:

	// constructor to initialize the dielectric material
	// parameters:
	//   index_of_refraction: the index of refraction
	dielectric(double index_of_refraction) : _ior(index_of_refraction) {}

	// scatter function for simulating interaction between a ray and a material
	// parameters:
	//   r_in: the ray
	//   rec: the record containing intersection information
	//   attenuation: the color attenuation due to the material's interaction
	//   scattered: the scattered ray after interaction with the material
	// returns:
	//   true if scattering occurs (and it always does)
	bool scatter(const ray &r_in, const hit_record &rec, colour &attenuation, ray &scattered) const override {
		// calculate the ratio of indices of refraction.
		auto refraction_ratio = rec.front_face ? (1.0 / _ior) : _ior;
		// compute values needed for refraction and reflection
		auto unit_direction = unitVector(r_in.direction());
		auto cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		auto sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		// check for total internal reflection or choose reflection / refraction
		auto cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 direction;
		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > randomDouble()) {
			direction = reflect(unit_direction, rec.normal);
		} else {
			direction = refract(unit_direction, rec.normal, refraction_ratio);
		}
  		// create the scattered ray
		scattered = ray(rec.p, direction);
		// set attenuation to white
		attenuation = colour(1.0, 1.0, 1.0);
		// always scatters
		return true;
	}

private:

	double _ior;			// the index of refraction

	// calculate schlick's approximation for reflectance
	// parameters:
	//   cosine: cosine of the angle between the ray and the normal
	//   ior: index of refraction
	// returns:
	//   estimated reflectance value
	static double reflectance(double cosine, double ior) {
		// calculate the fresnel reflectance using schlick's approximation
		// r0 = reflection coefficient at normal incidence
		auto r0 = (1 - ior) / (1 + ior);
		r0 = r0 * r0;
		// calculate reflectance based on schlick's formula (considering the cosine of the angle of incidence)
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}

};

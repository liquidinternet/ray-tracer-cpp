#ifndef METAL_H
#define METAL_H

#include "material.hpp"

class metal : public material {
public:

	metal(const colour &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	bool scatter(const ray &r_in, const hit_record &rec, colour &attenuation, ray &scattered) const override {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_unit_vector());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:

	colour albedo;
	double fuzz;

};

#endif

#pragma once
#include "material.hpp"

class lambertian : public material {
public:

	lambertian(const colour &a) : albedo(a) {}

	bool scatter(const ray &r_in, const hit_record &rec, colour &attenuation, ray &scattered) const override {
		auto scatter_direction = rec.normal + randomUnitVector();

		// catch degenerate scatter direction
		if (scatter_direction.nearZero()) {
			scatter_direction = rec.normal;
		}
		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

private:

	colour albedo;

};

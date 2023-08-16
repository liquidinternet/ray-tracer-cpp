#pragma once
#include "hittable_list.hpp"

class material {
public:

	virtual ~material() = default;

	virtual bool scatter(const ray &r_in, const hit_record &rec, colour &attenuation, ray &scattered) const = 0;

};

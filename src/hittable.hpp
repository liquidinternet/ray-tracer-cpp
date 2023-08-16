#pragma once
#include "hit_record.h"

class hittable {
public:

	virtual ~hittable() = default;

	virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;

};

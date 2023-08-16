#pragma once
#include "ray.hpp"

class material;

class hit_record {
public:

	point3 p;
	vec3 normal;
	shared_ptr<material> mat;
	double t;
	bool front_face;

	void setFaceNormal(const ray &r, const vec3 &outward_normal) {
		// sets the hit record normal vector
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};
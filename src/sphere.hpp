#pragma once
#include "hittable.hpp"

// a class representing a sphere object that can be intersected by rays
class sphere : public hittable {
public:

	// constructor to initialise sphere with a centre, radius, and material
	sphere(point3 _centre, double _radius, shared_ptr<material> _material)
		: _centre(_centre), _radius(_radius), _material(_material) { }

	// check for ray / sphere intersection
	// parameters:
	//   r: the ray
	//	 ray_t: an interval representing the range of intersection values
	//   rec: the record containing intersection information
	// returns:
	//   true if an intersection matched else false
	bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
		// calculate vector from ray origin to sphere centre
		vec3 oc = r.origin() - _centre;
		// coefficients for ray-sphere intersection
		auto a = r.direction().lengthSquared();
		auto half_b = dot(oc, r.direction());
		auto c = oc.lengthSquared() - _radius * _radius;
		// calculate the discriminant to determine if there are real roots
		auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0) {
			return false;
		}
		auto sqrtd = sqrt(discriminant);
		// find nearest root that lies within acceptable range
		auto root = (-half_b - sqrtd) / a;
		// check if root is not within interval range
		if (!ray_t.surrounds(root)) {
			// calculate alternate root
			root = (-half_b + sqrtd) / a;
			// check if alternate root is not within interval range
			if (!ray_t.surrounds(root)) {
				// both roots are outside acceptable range
				return false;
			}
		}
 		// update hit_record
		rec.distance = root;
		rec.point = r.at(rec.distance);
		auto outward_normal = (rec.point - _centre) / _radius;
		rec.setFaceNormal(r, outward_normal);
		rec.material = _material;
		// intersection found
		return true;
	}

private:

	point3 _centre;							// sphere centre
	double _radius;							// sphere radius
	shared_ptr<material> _material;			// sphere material

};

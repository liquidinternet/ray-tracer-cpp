#pragma once
#include "hittable.hpp"

class sphere : public hittable {
public:

	sphere(point3 _center, double _radius, shared_ptr<material> _material) : _centre(_center), _radius(_radius), _material(_material) {}

	bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
		vec3 oc = r.origin() - _centre;
		auto a = r.direction().lengthSquared();
		auto half_b = dot(oc, r.direction());
		auto c = oc.lengthSquared() - _radius * _radius;

		auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0) {
			return false;
		}
		auto sqrtd = sqrt(discriminant);

		// find nearest root that lies within acceptable range
		auto root = (-half_b - sqrtd) / a;
		if (!ray_t.surrounds(root)) {
			root = (-half_b + sqrtd) / a;
			if (!ray_t.surrounds(root)) {
				return false;
			}
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - _centre) / _radius;
		rec.setFaceNormal(r, outward_normal);
		rec.mat = _material;

		return true;
	}

private:

	point3 _centre;
	double _radius;
	shared_ptr<material> _material;

};

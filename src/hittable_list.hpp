#pragma once
#include "hittable.hpp"
#include <memory>
#include <vector>

// a derived class representing a collection of hittable objects
class hittable_list : public hittable {
public:

	// vector of shared pointers to hittable objects
	std::vector<shared_ptr<hittable>> objects;

	// default constructor
	hittable_list() { }

	// constructor that adds an object to the list upon creation
	hittable_list(shared_ptr<hittable> object) {
		add(object);
	}

	// add object to the list
	void add(shared_ptr<hittable> object) {
		objects.push_back(object);
	}

	// clear list of objects
	void clear() {
		objects.clear();
	}

	// check for intersections within an interval and update hit_record with matches
	// parameters:
	//   r: the ray
	//	 ray_t: an interval representing the range of intersection values
	//   rec: the record containing intersection information
	// returns:
	//   true if an intersection matched else false
	bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
		// placeholders for hit detection
		auto hit_anything = false;
		auto closest_so_far = ray_t.max;
		// hit_record to store intersection information
		hit_record temp_rec;
		// loop through all objects in list
		for (const auto &object : objects) {
			// check for intersection with current object
			if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
				// hit something
				hit_anything = true;
				closest_so_far = temp_rec.distance;
				// update record
				rec = temp_rec;
			}
		}
		// intersection matched
		return hit_anything;
	}
};

#pragma once
#include "hit_record.h"

// an absract class representing objects that can be hit by rays
class hittable {
public:

	// parameters:
	//   r: the ray
	//	 ray_t: an interval representing the range of intersection values
	//   rec: the record containing intersection information
	// returns:
	//   true if an intersection matched else false
	virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;

	// destructor to ensure cleanup in derived classes
	virtual ~hittable() = default;

};

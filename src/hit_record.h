#pragma once
#include "ray.hpp"

// bring in materal class
class material;

// a class representing a record of a ray hit
class hit_record {
public:

	// position of the point that was hit
	point3 point;
	// normal of the point that was hit
	vec3 normal;
	// pointer to the material of the hit object
	shared_ptr<material> material;
	// hit distance along the ray
	double distance;
	// was hit on front face of object
	bool front_face;

	// sets the hit record normal vector
	// parameters:
	//   r: ray information
	//   outward_normal: outward normal vector
	void setFaceNormal(const ray &r, const vec3 &outward_normal) {
		// calculate if hit occurred on the front face of the object (dot product of ray and normal)
		front_face = dot(r.direction(), outward_normal) < 0;
		// set normal direction (in or out)
		normal = front_face ? outward_normal : -outward_normal;
	}

};
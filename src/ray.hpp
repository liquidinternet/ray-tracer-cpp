#pragma once
#include "point3.hpp"

// a class representing a ray in 3d space
class ray {
public:

	// default constructor.
	ray() { }

    // constructor to initialise the ray with an origin and direction
    // parameters:
    //   origin: the origin point of the ray
    //   direction: the direction vector of the ray
	ray(const point3& origin, const vec3& direction)
		: _origin(origin), _direction(direction) { }

	// return the origin point of the ray
	point3 origin() const { return _origin; }

	// return the direction vector of the ray
	vec3 direction() const { return _direction; }

	// compute a point along the ray at a given value
    // parameters:
    //   t: the value along the ray
    // returns:
    //   the point at the specified value
	point3 at(double t) const { return _origin + t * _direction; }

private:

    point3 _origin;					// origin point of the ray
    vec3 _direction;				// direction vector of the ray

};

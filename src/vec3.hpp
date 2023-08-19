#pragma once
#include "common.hpp"
#include <cmath>
#include <iostream>

// a class representing a 3d vector
class vec3 {
public:

	double e[3];			// three components of the vector (x, y, z)

	// default constructor
	vec3()
		: e{ 0, 0, 0 } { }

	// constructor to initialise the vector with three values
	// parameters:
	//   e0: x component
	//   e1: y component
	//   e2: z component
	vec3(double e0, double e1, double e2)
		: e{ e0, e1, e2} { }

	// return component parts
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	// read and write vector components
	double operator[](int i) const { return e[i]; }
	double &operator[](int i) { return e[i]; }

	// overload operator and return negative vector
	vec3 operator-() const {
		return vec3(-e[0], -e[1], -e[2]);
	}

	// return vector after addition
	vec3& operator+=(const vec3 &v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	// return vector after multiplication
	vec3& operator*=(double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	// return vector after division
	vec3& operator/=(double t) {
		return *this *= 1 / t;
	}

	// square the length of vector
	double lengthSquared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	// length of vector
	double length() const {
		return sqrt(lengthSquared());
	}

	// return true if vector is close to zero in all dimensions
	bool nearZero() const {
		// threshold for near zero comparison
		auto s = 1e-8;
		// check if near zero
		auto near_zero = (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
		// return if near zero or not
		return near_zero;
	}

	// return a random vector in the range [0, 1)
	static vec3 random() {
		return vec3(randomDouble(), randomDouble(), randomDouble());
	}

	// return a random vector in the range [min, max)
	static vec3 random(double min, double max) {
		return vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
	}

};

// add two vectors
inline vec3 operator+(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// subtract two vectors
inline vec3 operator-(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// multiply two vectors
inline vec3 operator*(const vec3 &u, const vec3 &v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// multiply double by a vector
inline vec3 operator*(double t, const vec3 &v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

// multiply a vector by a double
inline vec3 operator*(const vec3 &v, double t) {
	return t * v;
}

// divide a vector by a double
inline vec3 operator/(vec3 v, double t) {
	return (1 / t) * v;
}

// compute the dot product of two vectors
inline double dot(const vec3 &u, const vec3 &v) {
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// compute the cross product of two vectors
inline vec3 cross(const vec3 &u, const vec3 &v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// return a unit vector in the direction of the input vector
// parameters:
//   v: input vector
// returns:
//   a unit vector in the direction of the input vector
inline vec3 unitVector(vec3 v) {
	return v / v.length();
}

// continuously generate random points until a valid point within a unit sphere is found
// returns:
//   a random point within the unit sphere
inline vec3 randomPointInUnitSphere() {
	// loop continuously
	while (true) {
		// generate a random point in three dimensions within the range [-1, 1)
		auto point = vec3::random(-1, 1);
		// check if the squared length of generated point is less than 1 (meaning point is within unit sphere)
		if (point.lengthSquared() < 1) {
			// return point which is within unit sphere
			return point;
		}
	}
}

// continuously generate random points until a valid point within a unit disk is found
// returns:
//   a random point within the unit disk
inline vec3 randomPointInUnitDisk() {
	// loop continuously
	while (true) {
		// generate a random point in two dimensions within the range [-1, 1) (setting z to 0)
		auto point = vec3(randomDouble(-1, 1), randomDouble(-1, 1), 0);
		// check if the squared length of generated point is less than 1 (meaning point is within unit sphere)
		if (point.lengthSquared() < 1) {
			// return point which is within unit disk
			return point;
		}
	}
}

// generate a random unit vector by normalising a random point within the unit sphere
// returns:
//   a random unit vector
inline vec3 randomUnitVector() {
	// get a random point within unit sphere
	auto point = randomPointInUnitSphere();
	// return unit vector
	return unitVector(point);
}

// computes the reflection of a vector across a surface normal
// parameters:
//   v: input vector
//   n: surface normal
// returns:
//	 a reflected vector
vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}

// computes the refraction of a vector through a surface normal
// parameters:
//   u: input vector
//   n: surface normal
//   etai_over_etat: ratio of refractive indices
// returns:
//   a refracted vector
inline vec3 refract(const vec3& v, const vec3& n, double etai_over_etat) {
	// compute the dot product
	auto d = dot(-v, n);
	// compute cosine of angle between vector and surface normal
	auto cosine = fmin(d, 1.0);
	// compute perpendicular component of refracted vector (snell's law)
	auto perpendicular = etai_over_etat * (v + cosine * n);
	// computre squared length
	auto squared_length = perpendicular.lengthSquared();
	// compute parallel component of the refracted vector
	auto parallel = -sqrt(fabs(1.0 - squared_length)) * n;
	// combine perpendicular and parallel components
	return perpendicular + parallel;
}

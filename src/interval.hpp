#pragma once
#include "common.hpp"

// a class representing an interval between two values
class interval {
public:

    double min;				// minimum value of the interval
    double max;				// maximum value of the interval

	// default constructor
	interval()
		: min(+infinity), max(-infinity) { }

	// constructor to initialise interval with min and max values
	interval(double _min, double _max)
		: min(_min), max(_max) { }

  	// check if interval contains a specific value
	bool contains(double x) const {
		return min <= x && x <= max;
	}

 	// check if the interval surrounds a specific value
	bool surrounds(double x) const {
		return min < x && x < max;
	}

 	// clamp value to range of interval
	double clamp(double x) const {
		return std::clamp(x, min, max);
	}

};
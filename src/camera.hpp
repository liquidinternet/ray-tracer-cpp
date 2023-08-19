#pragma once
#include "colour.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include <iostream>

// a class representing a camera used to render a scene
class camera {
public:

	int samples_per_pixel = 10;					// number of random samples per pixel
	int ray_depth = 10;							// maximum number of bounces around scene
	int image_width = 100;						// rendered image width in pixels
	double aspect_ratio = 1.0;					// ratio of image width over height
	double v_fov = 90;							// vertical field of view
	point3 look_from = point3(0, 0, -1);		// point camera is looking from
	point3 look_at = point3(0, 0, 0);			// point camera is looking at
	vec3 v_up = vec3(0, 1, 0);					// camera-relative "up" direction
	double defocus_angle = 0;					// variation angle of rays through each pixel
	double focus_distance = 10;					// distance from camera 'from point' to plane of focus

	// render the scene
	// parameters:
	//   world: the specified hittable world
	void render(const hittable& world) {
		// initialise camera parameters
		initialise();
		// image header (.ppm format)
		std::cout << "P3\n"
				  << image_width << " " << _image_height << "\n255\n";
		// loop through pixels
		for (int j = 0; j < _image_height; ++j) {
			// log progress
			std::clog << "\rScanlines remaining: " << (_image_height - j) << " " << std::flush;
			// loop through pixels
			for (int i = 0; i < image_width; ++i) {
				// calculate pixel colour by accumulating samples
				colour pixel_colour(0, 0, 0);
				// loop through samples
				for (int sample = 0; sample < samples_per_pixel; ++sample) {
					// get camera ray for the pixel
					auto r = getRay(i, j);
					// set colour
					pixel_colour += rayColour(r, ray_depth, world);
				}
				// write colour
				writeColour(std::cout, pixel_colour, samples_per_pixel);
			}
		}
		// log completion
		std::clog << "\rRender complete                     \n";
	}

private:

	int _image_height;							// rendered image height
	point3 _centre;								// camera centre
	point3 _pixel_zero_location;				// location of pixel 0, 0
	vec3 _pixel_delta_u;						// offset to pixel to the right
	vec3 _pixel_delta_v;						// offset to pixel below
	vec3 _u, _v, _w;							// camera frame basis vectors
	vec3 _defocus_disk_u;						// defocus disk horizontal radius
	vec3 _defocus_disk_v;						// defocus disk vertical radius

	// initialise camera parameters
	void initialise() {

		// calculate image height based on the aspect ratio
		_image_height = static_cast<int>(image_width / aspect_ratio);
		_image_height = (_image_height < 1) ? 1 : _image_height;
		_centre = look_from;

		// calculate viewport dimensions
		auto theta = degreesToRadians(v_fov);
		auto h = tan(theta / 2);
		auto viewport_height = 2 * h * focus_distance;
		auto viewport_width = viewport_height * (static_cast<double>(image_width) / _image_height);

		// calculate u, v, w unit basis vectors for camera coordinate frame
		_w = unitVector(look_from - look_at);
		_u = unitVector(cross(v_up, _w));
		_v = cross(_w, _u);

		// calculate the vectors across the horizontal and down the vertical viewport edges
		auto viewport_u = viewport_width * _u;
		auto viewport_v = viewport_height * -_v;

		// calculate the horizontal and vertical delta vectors from pixel to pixel
		_pixel_delta_u = viewport_u / image_width;
		_pixel_delta_v = viewport_v / _image_height;

		// calculate the location of the upper left pixel
		auto viewport_upper_left = _centre - (focus_distance * _w) - viewport_u / 2 - viewport_v / 2;
		_pixel_zero_location = viewport_upper_left + 0.5 * (_pixel_delta_u + _pixel_delta_v);

		// calculate the camera defocus disk basis vectors
		auto defocus_radius = focus_distance * tan(degreesToRadians(defocus_angle / 2));
		_defocus_disk_u = _u * defocus_radius;
		_defocus_disk_v = _v * defocus_radius;
	}

	// calculate colour of a ray by tracing interactions within the scene
	// parameters:
	//   r: the ray
	//   depth: ray bounce limit
	//   world: the specified hittable world
	// returns:
	//   ray colour
	colour rayColour(const ray& r, int depth, const hittable& world) const {
		// placeholder for record
		hit_record record;
		// check if exceeded the ray bounce limit (no more light gathered)
		if (depth <= 0) {
			return colour(0, 0, 0);
		}
		// check for ray / object intersection
		if (world.hit(r, interval(0.001, infinity), record)) {
			ray scattered;
			colour attenuation;
			 // if material of the hit object scatters the ray, calculate the scattered ray and attenuation
			if (record.material->scatter(r, record, attenuation, scattered)) {
				// recursively trace scattered rays and calculate colour
				return attenuation * rayColour(scattered, depth - 1, world);
			}
			// return colour
			return colour(0, 0, 0);
		}
		// no intersection found, create a simple gradient background
		auto unit_direction = unitVector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		// linear interpolation between white and blue based on the ray's vertical direction.
		return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1.0);
	}

	// generate randomly-sampled camera ray
	// parameters:
	//   i:	input i
	//   j: input j
	// returns:
	//   a camera ray
	ray getRay(int i, int j) const {
		// calculate the centre of the pixel
		auto pixel_centre = _pixel_zero_location + (i * _pixel_delta_u) + (j * _pixel_delta_v);
		// generate a random offset within the pixel
		auto pixel_sample = pixel_centre + pixelSampleSquare();
		// calculate the ray origin and direction
		auto ray_origin = (defocus_angle <= 0) ? _centre : defocusDiskSample();
		auto ray_direction = pixel_sample - ray_origin;
		// return camera ray
		return ray(ray_origin, ray_direction);
	}

	// generate a random vector in the square surrounding a pixel at the origin
	// returns:
	//   a vector within the square surrounding the pixel at the origin
	vec3 pixelSampleSquare() const {
		auto px = -0.5 + randomDouble();
		auto py = -0.5 + randomDouble();
		return (px * _pixel_delta_u) + (py * _pixel_delta_v);
	}

	// generate a random point in the camera defocus disk
	// returns:
	//   a random point
	point3 defocusDiskSample() const {
		// generate a random point in the unit disk
		auto p = randomPointInUnitDisk();
		// calculate the point within the defocus disk
		return _centre + (p[0] * _defocus_disk_u) + (p[1] * _defocus_disk_v);
	}

};

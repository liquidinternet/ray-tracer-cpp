#pragma once
#include "colour.hpp"
#include "material.hpp"
#include "sphere.hpp"
#include <iostream>

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

	void render(const hittable& world) {
		// initialise camera
		initialise();

		// image header
		std::cout << "P3\n"
				  << image_width << ' ' << image_height << "\n255\n";

		for (int j = 0; j < image_height; ++j) {
			// log output
			std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
			for (int i = 0; i < image_width; ++i) {
				colour pixel_colour(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; ++sample) {
					ray r = getRay(i, j);
					pixel_colour += rayColour(r, ray_depth, world);
				}
				writeColour(std::cout, pixel_colour, samples_per_pixel);
			}
		}

		// log completion
		std::clog << "\rDone.\n";
	}

private:

	int image_height;							// rendered image height
	point3 center;								// camera center
	point3 pixel_00_location;					// location of pixel 0, 0
	vec3 pixel_delta_u;							// offset to pixel to the right
	vec3 pixel_delta_v;							// offset to pixel below
	vec3 u, v, w;								// camera frame basis vectors
	vec3 defocus_disk_u;						// defocus disk horizontal radius
	vec3 defocus_disk_v;						// defocus disk vertical radius

	void initialise() {

		image_height = static_cast<int>(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;
		center = look_from;

		// calculate viewport dimensions.
		auto theta = degreesToRadians(v_fov);
		auto h = tan(theta / 2);
		auto viewport_height = 2 * h * focus_distance;
		auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

		// calculate u, v, w unit basis vectors for camera coordinate frame
		w = unitVector(look_from - look_at);
		u = unitVector(cross(v_up, w));
		v = cross(w, u);

		// calculate the vectors across the horizontal and down the vertical viewport edges
		vec3 viewport_u = viewport_width * u;
		vec3 viewport_v = viewport_height * -v;

		// calculate the horizontal and vertical delta vectors from pixel to pixel
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// calculate the location of the upper left pixel
		auto viewport_upper_left = center - (focus_distance * w) - viewport_u / 2 - viewport_v / 2;
		pixel_00_location = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

		// calculate the camera defocus disk basis vectors
		auto defocus_radius = focus_distance * tan(degreesToRadians(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;
	}

	colour rayColour(const ray& r, int depth, const hittable& world) const {
		// placeholder for record
		hit_record record;

		// check if exceeded the ray bounce limit (no more light gathered)
		if (depth <= 0) {
			return colour(0, 0, 0);
		}

		if (world.hit(r, interval(0.001, infinity), record)) {
			ray scattered;
			colour attenuation;
			if (record.mat->scatter(r, record, attenuation, scattered)) {
				return attenuation * rayColour(scattered, depth - 1, world);
			}
			return colour(0, 0, 0);
		}

		vec3 unit_direction = unitVector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1.0);
	}

	ray getRay(int i, int j) const {
		// get randomly-sampled camera ray for the pixel at location i, j originating from the camera defocus disk
		auto pixel_center = pixel_00_location + (i * pixel_delta_u) + (j * pixel_delta_v);
		auto pixel_sample = pixel_center + pixelSampleSquare();

		auto ray_origin = (defocus_angle <= 0) ? center : defocusDiskSample();
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	vec3 pixelSampleSquare() const {
		// returns a random point in the square surrounding a pixel at the origin
		auto px = -0.5 + randomDouble();
		auto py = -0.5 + randomDouble();
		return (px * pixel_delta_u) + (py * pixel_delta_v);
	}

	point3 defocusDiskSample() const {
		// returns a random point in the camera defocus disk
		auto p = randomInUnitDisk();
		return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
	}

};

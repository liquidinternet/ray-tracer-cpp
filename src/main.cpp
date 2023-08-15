#include "colour.hpp"
#include "camera.hpp"
#include "dielectric.hpp"
#include "lambertian.hpp"
#include "metal.hpp"

int main() {

	// create world
	hittable_list scene;

	// add ground to scene
	auto ground_material = make_shared<lambertian>(colour(0.5, 0.5, 0.5));
	scene.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	// add small spheres to scene
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {

			auto material_selector = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {

				shared_ptr<material> sphere_material;

				if (material_selector < 0.8) {
					// diffuse
					auto albedo = colour::random() * colour::random();
					sphere_material = make_shared<lambertian>(albedo);
					scene.add(make_shared<sphere>(center, 0.2, sphere_material));
				} else if (material_selector < 0.95) {
					// metal
					auto albedo = colour::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					scene.add(make_shared<sphere>(center, 0.2, sphere_material));
				} else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					scene.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}

		}
	}

	// add three large spheres to scene
	auto material1 = make_shared<dielectric>(1.5);
	scene.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(colour(0.4, 0.2, 0.1));
	scene.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
	scene.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	// create camera
	camera camera;

	// override camera defaults
	camera.aspect_ratio = 16.0 / 9.0;
	camera.image_width = 400;					// use 1920 for final render
	camera.samples_per_pixel = 10;				// use 500 samples for final quality render
	camera.ray_depth = 20;

	camera.v_fov = 20;
	camera.look_from = point3(13, 2, 3);
	camera.look_at = point3(0, 0, 0);
	camera.v_up = vec3(0, 1, 0);

	camera.defocus_angle = 0.6;
	camera.focus_distance = 10.0;

	// render
	camera.render(scene);

}

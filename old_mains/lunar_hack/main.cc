#include <iostream>
#include <algorithm>
#include "tuple.h"
#include "canvas.h"
#include "utils.h"
#include "material.h"
#include "light.h"
#include "intersection.h"
#include "colors.h"
#include <Eigen/Dense>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace Eigen;

// Dull plastic
// s.material.shininess = 20;
// s.material.specular = 0.4;

Tuple CANVAS_COLOR = colors::sky;
Tuple LIGHT_COLOR = colors::white;
Tuple MATERIAL_COLOR = colors::sky;

// Ambient is the scatteryness of light.
// You get sharper shadows when its lower, kind of like how on the surface of the moon you get really sharp shadows since theres no atmosphere to refract light

int
main()
{
	Sphere s;

	s.material.color = MATERIAL_COLOR;
	s.material.shininess = 100;
	s.material.specular = 0.1;
	s.material.ambient = 0.0;

	// Affine3f transform(Translation3f(-0.8,1.1,8));
	// Matrix4f translation_matrix = transform.matrix();
	// s.transform = translation_matrix;

	Point3D light_posisiton = Point3D(-10, 10, -10);
	Light light = Light(light_posisiton, LIGHT_COLOR);

	int origin_z = -7;

	int wall_z = 10;
	int wall_size = 7;
	int canvas_pixels = 1500;
	float pixel_size = (float)wall_size / canvas_pixels; // 0.07
	float half = (float)wall_size / 2.0f; // 3.5



	Point3D ray_origin_point = Point3D(0.0, 0.0, origin_z);
	Canvas canvas = Canvas(canvas_pixels, canvas_pixels, CANVAS_COLOR);
	Tuple color = Tuple::color(1, 0.1, 0.1);
	Tuple empty_color = Tuple::color(0.0, 0.0, 0.0);

	for (int y = 0; y < canvas_pixels - 1; y++) {
		float world_y = half - pixel_size * y;

		for (int x = 0; x < canvas_pixels - 1; x++) {

			float world_x = -half + pixel_size * x;

			Point3D world_position = Point3D(world_x, world_y, wall_z);


			Vector4f direction = world_position.cords - ray_origin_point.cords;
			Vector4f normalized_direction = direction.normalized();

			Ray r = {
				Point3D(ray_origin_point), 
				Vector3D(normalized_direction)
			};


			vector<Intersection> intersections = intersect(s, r);

			// cout << intersections[0].t << endl;

			if (intersections.size() > 0) {
				// cout << "Intersection found at t=" << intersections[0].t << endl;

				Point3D intersect_pos = position(r, intersections[0].t);
				Vector3D intersect_normal = normal_at(intersections[0].object, intersect_pos);
				Vector3D eye = Vector3D(r.direction.cords * -1); 

				Tuple final_color = lighting(intersections[0].object.material, light, intersect_pos, eye, intersect_normal);


				canvas.write_pixel(x, y, final_color, 1);
			} else {
				// canvas.write_pixel(canvas_x, canvas_y, empty_color, 1);
			}
		}
	}

	canvas.save_to_PPM("final.ppm");

	return 0;
}



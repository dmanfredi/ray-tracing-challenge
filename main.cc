#include <iostream>
#include <algorithm>
#include "tuple.h"
#include "canvas.h"
#include "utils.h"
#include "intersection.h"
#include <Eigen/Dense>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace Eigen;

int
main()
{
	Sphere s;

	int origin_z = -5;

	int wall_z = 10;
	int wall_size = 7;
	int canvas_pixels = 100;
	float pixel_size = (float)wall_size / canvas_pixels; // 0.07
	float half = (float)wall_size / 2.0f; // 3.5

	Point3D ray_origin_point = Point3D(0.0, 0.0, origin_z);
	Canvas canvas = Canvas(canvas_pixels, canvas_pixels);
	Tuple color = Tuple::color(1, 0.1, 0.1);
	Tuple empty_color = Tuple::color(0.0, 0.0, 0.0);

	for (int y = 0; y < canvas_pixels - 1; y++) {
		float world_y = half - pixel_size * y;

		for (int x = 0; x < canvas_pixels - 1; x++) {

			float world_x = -half + pixel_size * x;

			Point3D position = Point3D(world_x, world_y, wall_z);


			Vector4f direction = position.cords - ray_origin_point.cords;
			Vector4f normalized_direction = direction.normalized();

			Ray r = {
				Point3D(ray_origin_point), 
				Vector3D(normalized_direction)
			};

			vector<Intersection> intersections = intersect(s, r);

			// cout << intersections[0].t << endl;

			if (intersections.size() > 0) {
				cout << "Intersection found at t=" << intersections[0].t << endl;

				canvas.write_pixel(x, y, color, 1);
			} else {
				// canvas.write_pixel(canvas_x, canvas_y, empty_color, 1);
			}
		}
	}


	// for (float world_x = -half; world_x < (half + 0.0001f); world_x += pixel_size) { 
	// 	// cout << world_x << endl;

	// 	for (float world_y = -half; world_y < (half + 0.0001f); world_y += pixel_size) { 
	// 		// counter++;
	// 		Ray r = {
	// 			Point3D(world_x, world_y, origin_z),
	// 			Vector3D(0, 0, 1)
	// 		};
	// 		vector<Intersection> intersections = intersect(s, r);


	// 		int canvas_x = round(world_x / pixel_size) + 500;
	// 		int canvas_y = round(world_y / pixel_size) + 500;

	// 		// cout << canvas_x << endl;
	// 		// cout << canvas_y << endl;

	// 		if (intersections.size() > 0) {
	// 			canvas.write_pixel(canvas_x, canvas_y, color, 1);
	// 		} else {
	// 			// canvas.write_pixel(canvas_x, canvas_y, empty_color, 1);
	// 		}
	// 	}
	// }

	canvas.save_to_PPM("final.ppm");

	return 0;
}



// This isnt quite right re: Rays.
// But its close and provides some decent insight.
// writing out my thought process really helps.


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
	int canvas_pixels = 1000;
	float pixel_size = (float)wall_size / canvas_pixels; // 0.07
	float half = (float)wall_size / 2.0f; // 3.5

	Canvas canvas = Canvas(canvas_pixels, canvas_pixels);
	Tuple color = Tuple::color(1, 0.1, 0.1);

	Tuple empty_color = Tuple::color(0.0, 0.0, 0.0);

	// we start at the back wall, -5 z. -5 what? -5 units.
	// we need the top left corner. We know we have a canvas of 100 by 100 px.
	// In world space out wall is 7. 7 what? 7 units.
	// 7 by 7. What does 7 equal in pixels? Take wall_size and divde by canvas pixels.
	// Each pixel is worth 0.07 units.
	
	// Okay who is casting the ray? Its in world space. So we need to start at -5 z, but what x and y? Starting in the top left its -x +y. -50 and +50 pixels to start?
	// So in world space I have -3.5 x and 3.5 y. I need to cast a ray to see if I intersect.
	// Then I need to add pixel_size to -3.5, I would slowly work across the first row until I reach +3.5 on the other side.
	// 100 rays would be cast for one row of X, and if I intersect the sphere I color the corresponding pixel.
	// Then I start on the next row by adding pixel_size to Y, and scan the next row.

	// we start at the top left corner, -5(z), -half(x), +half(y)
	// int counter = 0;

	for (float world_x = -half; world_x < (half + 0.0001f); world_x += pixel_size) { 
		// cout << world_x << endl;

		for (float world_y = -half; world_y < (half + 0.0001f); world_y += pixel_size) { 
			// counter++;
			Ray r = {
				Point3D(world_x, world_y, origin_z),
				Vector3D(0, 0, 1)
			};
			vector<Intersection> intersections = intersect(s, r);


			int canvas_x = round(world_x / pixel_size) + 500;
			int canvas_y = round(world_y / pixel_size) + 500;

			// cout << canvas_x << endl;
			// cout << canvas_y << endl;

			if (intersections.size() > 0) {
				canvas.write_pixel(canvas_x, canvas_y, color, 1);
			} else {
				// canvas.write_pixel(canvas_x, canvas_y, empty_color, 1);
			}
		}
	}

	canvas.save_to_PPM("final.ppm");

	return 0;
}



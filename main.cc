#include <iostream>
#include <algorithm>
#include "tuple.h"
#include "canvas.h"
#include "utils.h"
#include <Eigen/Dense>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace Eigen;



int
main()
{

	Canvas canvas = Canvas(900, 550);
	Tuple color = Tuple::color(0.980, 0.649, 0.118);

	// canvas.write_pixel(canvas_x, canvas_y, color, 10);


	for (int i = 0; i < 12; i++) {
		Vector4f pointy;
		pointy << 0.0f, 200.0f, 0.0f, 1.0f;
		int degrees = 360 - (i * 30);
		float radians = degrees * (M_PI / 180);

		Matrix4f y_rotation = create_Z_rotation_matrix(i * (M_PI / 6));
		pointy = y_rotation * pointy;

		int canvas_y = canvas.height - 1 -  static_cast<int>(pointy[1]);
		int canvas_x = static_cast<int>(pointy[0]);

		canvas_x += 250;   
		canvas_y -= 250;
	
		float result = pointy[1];

		canvas.write_pixel(canvas_x, canvas_y, color, 10);
	}

	canvas.save_to_PPM("final.ppm");

	cout << endl;

	return 0;
}


// tick
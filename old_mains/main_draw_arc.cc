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

struct Projectile {
	Tuple position;
	Tuple velocity;
};

struct Environment {
	Tuple gravity;
	Tuple wind;
};

void 
tick(Projectile& proj, const Environment& env) 
{
	proj.position = proj.position + proj.velocity;
	proj.velocity = proj.velocity + env.gravity + env.wind;


}



void
testing_something()
{
	double result = M_PI;
}

int
main()
{
	Projectile proj = {
		Tuple::point(0, 1, 0),
		Tuple::vector(1, 1.8, 0).normalize() * 9
	};

	// Our environment
	Environment env = {
		Tuple::vector(0, -0.1, 0),
		Tuple::vector(-0.01, 0, 0)
	};

	Canvas canvas = Canvas(900, 550);
	Tuple color = Tuple::color(1, 0.1, 0.1);

	int ticks = 0;
	while (proj.position.y > 0) {
		// cout << "Tick " << ticks << ": ";
		// proj.position.print();
		// cout << endl;

		tick(proj, env);

		// Flip the y-coordinate when writing to the canvas
        int canvas_y = canvas.height - 1 - static_cast<int>(round(proj.position.y));
        int canvas_x = static_cast<int>(round(proj.position.x));

		canvas.write_pixel(canvas_x, canvas_y, color, 2);

		ticks++;

		if (ticks > 1000) {  // Safety check to prevent infinite loop
			cout << "Simulation stopped after 1000 ticks." << endl;
			break;
		}  
	}

	cout << "The projectile hit the ground after " << ticks << " ticks." << endl;
	cout << "Final position: ";
	proj.position.print();

	// canvas.save_to_PPM("final.ppm");

	cout << endl;

	return 0;
}


// tick
#include <iostream>
#include <algorithm>
#include "tuple.h"
#include "canvas.h"
#include <Eigen/Dense>

using namespace std;

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
	Eigen::Matrix4d identityMatrix = Eigen::Matrix4d::Identity();
	Eigen::Matrix4d matrixA;

	matrixA << 	-2, -8, 3, 5,
				-3, 1, 7, 3,
				1, 2, -9, 6,
				-6, 7, 7, -9;

	Eigen::Matrix4d inverted_matrixA = matrixA.inverse();

	cout << matrixA * inverted_matrixA << endl;
	cout << matrixA * inverted_matrixA << endl;
}

int
main()
{
	testing_something();

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

		canvas.write_pixel(canvas_x, canvas_y, color);

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
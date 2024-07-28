#include <iostream>
#include <algorithm>
#include "tuple.h"
#include "canvas.h"
#include "utils.h"
#include <Eigen/Dense>
#include <cmath>

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

Matrix4f create_scaling_matrix(float x, float y, float z)
{
	Matrix4f matrix;
	matrix  << 	x, 0, 0, 0,
				0, y, 0, 0,
				0, 0, z, 0,
				0, 0, 0, 1;

	return matrix;
}
Matrix4f reflection_matrix = create_scaling_matrix(-1, 1, 1);


Matrix4f create_X_rotation_matrix(float r)
{
	Matrix4f x_rotation = Matrix4f::Identity();
	x_rotation.block<3,3>(0,0) = AngleAxisf(r, Vector3f::UnitX()).matrix();
	 return x_rotation;
}

void
testing_something()
{
	// Matrix4d identityMatrix = Matrix4d::Identity();
	// Matrix4d matrixA;

	// Affine3f transform(Translation3f(1,2,3));
	// Matrix4f translation_matrix = transform.matrix();

	// Vector4f e_t1;
	// e_t1 << 1, 2.6, 0.6, 1;
	// Vector4f transformed = translation_matrix * e_t1;
	// cout << transformed << endl;


	// Affine3f scaling_transform(Scaling(2, 3, 4)); // Scale by 2 in x, 3 in y, and 4 in z



	Vector3f scaling_factors(2, 3, 4);
    Affine3f transform = Affine3f::Identity();
    transform.scale(scaling_factors);
    Matrix4f scaling_matrix = transform.matrix();

	cout << scaling_matrix << endl;

	Vector4f e_t1;
    e_t1 << -4, 6, 8, 1;
    Vector4f scaled = scaling_matrix * e_t1;
    float y_value = scaled(1);

		
	cout << scaled << endl;

	// Vector4f e_t1;
	// e_t1 << 1, 2.6, 0.6, 1;
	// Vector4f transformed = scaling_matrix * e_t1;

	
	// cout << scaledFella << endl;

	// matrixA << 	-2, -8, 3, 5,
	// 			-3, 1, 7, 3,
	// 			1, 2, -9, 6,
	// 			-6, 7, 7, -9;

	// Matrix4d inverted_matrixA = matrixA.inverse();

	// cout << matrixA * inverted_matrixA << endl;
	// cout << matrixA * inverted_matrixA << endl;
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
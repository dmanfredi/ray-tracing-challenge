#include <iostream>
#include <algorithm>


// I need to rename my old canvas stuff to image, I dont want to confuse it with my other world related canvas things

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

Tuple CANVAS_COLOR = colors::Black;
Tuple LIGHT_COLOR = colors::White;
// Tuple MATERIAL_COLOR = colors::sunset;

// Ambient is the scatteryness of light.
// You get sharper shadows when its lower, kind of like how on the surface of the moon you get really sharp shadows since theres no atmosphere to refract light

int
main()
{
	World world;
	world.light = Light(Point3D(-10, 10, -10), colors::White);

	Sphere floor;
	floor.transform = create_scaling_matrix(10, 0.01, 10);
	floor.material = Material();
	floor.material.color = colors::LightGoldenrodYellow;
	floor.material.specular = 0;
	world.objects.push_back(floor);

	Sphere left_wall;
	left_wall.transform = create_translation_matrix(0, 0, 5) * 
		create_Y_rotation_matrix(-M_PI/4) * 
		create_X_rotation_matrix(M_PI/2) * 
		create_scaling_matrix(10, 0.01, 10);
	left_wall.material = floor.material;
	world.objects.push_back(left_wall);

	Sphere right_wall;
	right_wall.transform = create_translation_matrix(0, 0, 5) * 
		create_Y_rotation_matrix(M_PI/4) * 
		create_X_rotation_matrix(M_PI/2) * 
		create_scaling_matrix(10, 0.01, 10);
	right_wall.material = floor.material;
	world.objects.push_back(right_wall);	

	Sphere middle;
	middle.transform = create_translation_matrix(-0.5, 1, 0.5);
	middle.material = Material();
	middle.material.color = colors::Tomato;
	middle.material.diffuse = 0.7;
	middle.material.specular = 0.3;
	world.objects.push_back(middle);

	Sphere right;
	right.transform = create_translation_matrix(1.5, 0.5, -0.5) * create_scaling_matrix(0.5, 0.5, 0.5);
	right.material = Material();
	right.material.color = colors::Aquamarine;
	right.material.diffuse = 0.7;
	right.material.specular = 0.3;
	world.objects.push_back(right);

	Sphere left;
	left.transform = create_translation_matrix(-1.5, 0.33, -0.75) * create_scaling_matrix(0.33, 0.33, 0.33);
	left.material = Material();
	left.material.color = colors::MistyRose;
	left.material.diffuse = 0.7;
	left.material.specular = 0.3;
	world.objects.push_back(left);

	Camera camera = Camera(1200, 600, M_PI / 5); // 1.57 = 90deg
	camera.transform = view_transform(Point3D(0, 1.5, -10), Point3D(0, 1, 0), Vector3D(0, 1, 0));

	Canvas canvas = render(camera, world);

	canvas.save_to_PPM("final.ppm");

	// Canvas canvas = Canvas(canvas_pixels, canvas_pixels, CANVAS_COLOR);

	// canvas.save_to_PPM("final.ppm");

	return 0;
}



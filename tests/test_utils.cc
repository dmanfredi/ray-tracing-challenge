#include <cmath>
#include "world.h"
#include "tests/test_utils.h"

const float SQRT2_OVER_2 = sqrt(2.0f) / 2.0f;
const float PI_OVER_2 = M_PI / 2.0f;

World create_default_world() {
	World default_world;
	
	default_world.light = Light(Point3D(-10, 10, -10), Tuple::color(1, 1, 1));
	
	Sphere s1;
	Material m1;
	m1.color = Tuple::color(0.8, 1.0, 0.6);
	m1.diffuse = 0.7;
	m1.specular = 0.2;
	s1.material = m1;
	default_world.objects.push_back(s1);
	
	Sphere s2;
	s2.transform = create_scaling_matrix(0.5, 0.5, 0.5);
	default_world.objects.push_back(s2);
	
	return default_world;
}
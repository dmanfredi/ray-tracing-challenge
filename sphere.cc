#include "sphere.h"
#include <cmath>
#include <iostream>

int Sphere::next_id = 0;

Sphere::Sphere(int r, Point3D o) : id(next_id++), radius(r), origin(o) {};

int Sphere::get_id() const {
	return id;
}

int Sphere::get_next_id() {
	return next_id;
}


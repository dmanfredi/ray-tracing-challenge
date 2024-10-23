#include "sphere.h"
#include <cmath>
#include <iostream>

int Sphere::next_id = 0;

Sphere::Sphere(int r, Point3D o, Matrix4f t) : id(next_id++), radius(r), origin(o), transform(t) {};

int Sphere::get_id() const {
	return id;
}

int Sphere::get_next_id() {
	return next_id;
}


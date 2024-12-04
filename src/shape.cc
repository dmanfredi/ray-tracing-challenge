#include "shape.h"
#include "material.h"
#include <cmath>
#include <iostream>

int Shape::next_id = 0;

Shape::Shape(Matrix4f t, Material m) 
	: id(next_id++),
	transform(t),
	material(m)
{}

int Shape::get_id() const {
	return id;
}

int Shape::get_next_id() {
	return next_id;
}

// Vector3D normal_at(Sphere sphere, Point3D point) {
// 	// point converted to object space
// 	Vector4f object_point = sphere.transform.inverse() * point.cords;

// 	// origin to surface of sphere
// 	Vector4f object_normal =  object_point - Point3D(0, 0, 0).cords;

// 	Vector4f  world_normal = (sphere.transform.inverse().transpose()) * object_normal;

// 	world_normal.w() = 0.0f;

// 	return Vector3D(world_normal.normalized());
// } 

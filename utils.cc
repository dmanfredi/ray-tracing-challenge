#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <algorithm>
#include <Eigen/Dense>
#include <cmath>

#include "tuple.h"
#include "sphere.h"

using namespace std;
using namespace Eigen;

struct Point3D {
	Vector4f data;
	Point3D(float x, float y, float z) : data(x, y, z, 1.0f) {}
	float x() const { return data[0]; }
	float y() const { return data[1]; }
	float z() const { return data[2]; }
};

struct Vector3D {
	Vector4f data;
	Vector3D(float x, float y, float z) : data(x, y, z, 0.0f) {}
	float x() const { return data[0]; }
	float y() const { return data[1]; }
	float z() const { return data[2]; }
};

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

Matrix4f create_Y_rotation_matrix(float r)
{
	Matrix4f y_rotation = Matrix4f::Identity();
	y_rotation.block<3,3>(0,0) = AngleAxisf(r, Vector3f::UnitY()).matrix();
	return y_rotation;
}

Matrix4f create_Z_rotation_matrix(float r)
{
	Matrix4f y_rotation = Matrix4f::Identity();
	y_rotation.block<3,3>(0,0) = AngleAxisf(r, Vector3f::UnitZ()).matrix();
	return y_rotation;
}

Matrix4f create_shearing_matrix(float xy, float xz, float yx, float yz, float zx, float zy)
{
	Matrix4f matrix;
	matrix  << 	1, xy, xz, 0,
				yx, 1, yz, 0,
				zx, zy, 1, 0,
				0,  0,  0, 1;

	return matrix;
}

struct Ray {
	Point3D origin;
	Vector3D direction;
	
	Ray(const Point3D& o, const Vector3D& d) : origin(o), direction(d) {}
};

Point3D position(Ray ray, float time) {
	Vector4f result = ray.origin.data + (ray.direction.data * time);
	return Point3D(result[0], result[1], result[2]);
}

vector<int> intersection(Ray ray, Sphere sphere) {
	Vector4f sphere_to_ray = ray.origin.data - Vector4f(0, 0, 0, 0);
	int a = sphere_to_ray.dot(ray.direction.data);
	int b = 2 * (ray.direction.data.dot(sphere_to_ray));
	int c = sphere_to_ray.dot(sphere_to_ray) - 1;

	int discriminant = pow(b, 2) - 4 * a * c; 

	if (discriminant < 0) {
		return {};
	}

	vector<int> intersections = {};

	int t1 = (-b - sqrt(discriminant) / (2 * a));
	int t2 = (-b + sqrt(discriminant) / (2 * a));
	
	// Intersections occur in increasing order
	if (t1 > t2) {
		intersections = {t2, t1};
	} else {
		intersections = {t1, t2};
	}



}

#endif // MATRIX_OPERATIONS_H
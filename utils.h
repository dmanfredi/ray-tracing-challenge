#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include "Eigen/Dense"

using namespace Eigen;

// Point3D structure
struct Point3D {
	Vector4f data;
	Point3D(float x, float y, float z) : data(x, y, z, 1.0f) {}

	float x() const { return data[0]; }
	float y() const { return data[1]; }
	float z() const { return data[2]; }
};

// Vector3D structure
struct Vector3D {
	Vector4f data;
	Vector3D(float x, float y, float z) : data(x, y, z, 0.0f) {}

	float x() const { return data[0]; }
	float y() const { return data[1]; }
	float z() const { return data[2]; }
};

// Function to create a scaling matrix
Matrix4f create_scaling_matrix(float x, float y, float z);

// Reflection matrix
extern Matrix4f reflection_matrix;

// Function to create a rotation matrix around the X-axis
Matrix4f create_X_rotation_matrix(float r);

// Function to create a rotation matrix around the Y-axis
Matrix4f create_Y_rotation_matrix(float r);

// Function to create a rotation matrix around the Z-axis
Matrix4f create_Z_rotation_matrix(float r);

// Function to create a shearing matrix that can shear by any axis realtive to any other axis
Matrix4f create_shearing_matrix(float xy, float xz, float yx, float yz, float zx, float zy);

// Ray
struct Ray {
	Point3D origin;
	Vector3D direction;
	
	Ray(const Point3D& o, const Vector3D& d) : origin(o), direction(d) {}
};

Point3D position(Ray ray, float time);

#endif // MATRIX_OPERATIONS_H
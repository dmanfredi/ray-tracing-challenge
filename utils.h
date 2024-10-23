#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <algorithm>
#include <Eigen/Dense>
#include <cmath>
#include <vector>

using namespace std;
using namespace Eigen;

// Foward declarations
class Sphere;
class Intersection;
struct Ray;
struct Point3D;
struct Vector3D;

struct Point3D {
	Vector4f cords;
	
	Point3D(float x, float y, float z) : cords(x, y, z, 1.0f) {}
	
	// New constructor
	explicit Point3D(const Vector4f& vec) : cords(vec) {
		cords[3] = 1.0f;  // Ensure w-component is 1.0f for a point
	}
	
	float x() const { return cords[0]; }
	float y() const { return cords[1]; }
	float z() const { return cords[2]; }
};

struct Vector3D {
	Vector4f cords;
	
	Vector3D(float x, float y, float z) : cords(x, y, z, 0.0f) {}
	
	// New constructor
	explicit Vector3D(const Vector4f& vec) : cords(vec) {
		cords[3] = 0.0f;  // Ensure w-component is 0.0f for a vector
	}
	
	float x() const { return cords[0]; }
	float y() const { return cords[1]; }
	float z() const { return cords[2]; }
};;

struct Ray {
	Point3D origin;
	Vector3D direction;
	
	Ray(const Point3D& o, const Vector3D& d) : origin(o), direction(d) {}
};

// Function declarations
Matrix4f create_scaling_matrix(float x, float y, float z);
extern Matrix4f reflection_matrix;
Matrix4f create_X_rotation_matrix(float r);
Matrix4f create_Y_rotation_matrix(float r);
Matrix4f create_Z_rotation_matrix(float r);
Matrix4f create_shearing_matrix(float xy, float xz, float yx, float yz, float zx, float zy);
Point3D position(Ray ray, float time);
Ray transform_ray(Ray ray, const Matrix4f& matrix);

#endif // UTILS_H
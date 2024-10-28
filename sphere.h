#ifndef SPHERE_H
#define SPHERE_H

#include "utils.h"
#include "material.h"

class Sphere {
private:
	static int next_id;
	int id;

public: 
	int radius;
	Point3D origin;
	Matrix4f transform;
	Material material;

	// Constructor with default values
	Sphere(int r = 1, Point3D o = Point3D(0, 0, 0), Matrix4f t = Matrix4f::Identity(), Material m = Material());

	// Destructor
	~Sphere() = default;

	// Getter for ID
	int get_id() const;

	// Static method to get next ID
	static int get_next_id();
};

Vector3D normal_at(Sphere sphere, Point3D point); 

#endif // SPHERE_H
#ifndef SPHERE_H
#define SPHERE_H

#include "utils.h"

class Sphere {
private:
	static int next_id;
	int id;

public: 
	int radius;
	Point3D origin;

	// Constructor with default values
	Sphere(int r = 1, Point3D o = Point3D(0, 0, 0));

	// Destructor
	~Sphere() = default;

	// Getter for ID
	int get_id() const;

	// Static method to get next ID
	static int get_next_id();

};

#endif // SPHERE_H
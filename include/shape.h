#ifndef SHAPE_H
#define SHAPE_H

#include "utils.h"
#include "material.h"

// Shape class
class Shape {
private:
	static int next_id;
	int id;

public: 
	Matrix4f transform;
	Material material;

	// Constructor with default values
	Shape(Matrix4f t = Matrix4f::Identity(), Material m = Material());

	// Destructor
	virtual  ~Shape() = default;

	// Getter for ID
	int get_id() const;

	// Static method to get next ID
	static int get_next_id();
};

#endif // SHAPE_H
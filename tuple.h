#include <iostream>
#include <cmath>
#include <limits>

#ifndef TUPLE_H
#define TUPLE_H

class Tuple
{
public:
	float x, y, z, w;

	struct RGB {
		int r, g, b;
	};

 	// Default
    Tuple() : x(0), y(0), z(0), w(0) {}

	// Constructor
	Tuple(float x, float y, float z, float w);

	// Factory methods for points, vectors, and colors
	static Tuple point(float x, float y, float z);
	static Tuple vector(float x, float y, float z);
	static Tuple color(float red, float green, float blue);

	// Colors
	Tuple hadamard_product(const Tuple& other) const;
	RGB to_RGB() const;
	float red() const { return x; }
	float green() const { return y; }
	float blue() const { return z; }

	// Overloaded operators
	Tuple operator+(const Tuple& other) const;
	Tuple operator-(const Tuple& other) const; // Subtraction
	Tuple operator*(float scalar) const;
	Tuple operator/(float scalar) const;
	bool operator==(const Tuple& other) const;

	// Refular functions
	Tuple operator-() const;  // Negation
	double magnitude() const;
	Tuple normalize() const;
	double dot(const Tuple& other) const;
	Tuple cross(const Tuple& other) const;

	void print() const;
};

#endif // TUPLE_H
#include <cmath>
#include <limits>
#include <algorithm>

#include "Tuple.h"

// Constructor
Tuple::Tuple(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

// Factory methods for points and vectors
Tuple Tuple::point(float x, float y, float z)
{
	return Tuple(x, y, z, 1.0f);
}

Tuple Tuple::vector(float x, float y, float z)
{
	return Tuple(x, y, z, 0.0f);
}

Tuple Tuple::color(float red, float green, float blue)
{
	return Tuple(red, green, blue, 0.0f);
}

// Hadamard product (component-wise multiplication)
Tuple Tuple::hadamard_product(const Tuple& other) const {
	return Tuple(red() * other.red(), green() * other.green(), blue() * other.blue(), 0.0f);
}


Tuple::RGB Tuple::to_RGB() const {
	return {
		static_cast<int>(std::round(std::clamp(red() * 255, 0.0f, 255.0f))),
		static_cast<int>(std::round(std::clamp(green() * 255, 0.0f, 255.0f))),
		static_cast<int>(std::round(std::clamp(blue() * 255, 0.0f, 255.0f)))
	};
}

// Overloaded operators
Tuple Tuple::operator+(const Tuple& other) const
{
	return Tuple(x + other.x, y + other.y, z + other.z, w + other.w);
}

Tuple Tuple::operator-(const Tuple& other) const
{
	return Tuple(x - other.x, y - other.y, z - other.z, w - other.w);
}

Tuple Tuple::operator*(float scalar) const
{
	return Tuple(x * scalar, y * scalar, z * scalar, w * scalar);
}

Tuple Tuple::operator/(float scalar) const
{
	return Tuple(x / scalar, y / scalar, z / scalar, w / scalar);
}

Tuple Tuple::operator-() const
{
	return Tuple(-x, -y, -z, -w);
}

bool Tuple::operator==(const Tuple& other) const {
    const float EPSILON = 0.00001f;
    return (std::abs(x - other.x) < EPSILON &&
            std::abs(y - other.y) < EPSILON &&
            std::abs(z - other.z) < EPSILON &&
            std::abs(w - other.w) < EPSILON);
}

double Tuple::magnitude() const {
	return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2) + std::pow(w, 2));  
}

Tuple Tuple::normalize() const {
	double mag = magnitude();

    if (mag == 0) {
        throw std::runtime_error("Cannot normalize a zero vector.");
    }

	return Tuple(x / mag, y / mag, z / mag, w / mag);
}

double Tuple::dot(const Tuple& other) const {
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

Tuple Tuple::cross(const Tuple& other) const {
	return Tuple::vector(
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x
	);
}

void Tuple::print() const {
	std::cout << "(" << x << ", " << y << ", " << z << ", " << w << ")";
}


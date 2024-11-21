#ifndef CONSOLE_H
#define CONSOLE_H

#include <iostream>
#include <Eigen/Dense>
#include <Eigen/LU>

using Eigen::Matrix4f;
using Eigen::Vector4f;

class Console {

public:
	// General template for most types
	template<typename T>
	void log(T value) {
		std::cout << value << std::endl;
	}

	// Specialization for Matrix4f
	void log(Matrix4f matrix) {
		log_matrix(matrix);
	}

	// Specialization for Vector4f
	void log(Vector4f vector) {
		log_vector(vector);
	}

private:
	void log_matrix(Matrix4f matrix);
	void log_vector(Vector4f vector);
};

// Global console instance
inline Console console;

#endif
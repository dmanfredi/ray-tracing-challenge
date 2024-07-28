#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <algorithm>
#include "tuple.h"
#include <Eigen/Dense>
#include <cmath>

using namespace std;
using namespace Eigen;

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

#endif // MATRIX_OPERATIONS_H
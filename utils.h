#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

#include "Eigen/Dense"

using namespace Eigen;

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

#endif // MATRIX_OPERATIONS_H
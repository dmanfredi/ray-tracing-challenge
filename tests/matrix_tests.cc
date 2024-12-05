#include <gtest/gtest.h>
#include <mutex>
#include <condition_variable>
#include <Eigen/Dense>
#include <Eigen/LU>
#define _USE_MATH_DEFINES
#include <math.h>

#include "utils.h"
#include "tuple.h"

using namespace std;
using namespace Eigen;

const float SQRT2_OVER_2 = sqrt(2.0f) / 2.0f;
const float PI_OVER_2 = M_PI / 2.0f;

/* MATRICES */
/* MATRICES */
/* MATRICES */
TEST(MatrixTests, MatrixCreation) {
	Eigen::Matrix4f matrix;
	Eigen::Matrix3f matrix3;
	Eigen::Matrix2f matrix2;

	matrix(0, 3) = 3.5;
	matrix(1, 3) = 6.7;
	matrix(1, 2) = 10.1;

	EXPECT_FLOAT_EQ(matrix(0, 3), 3.5);
	EXPECT_FLOAT_EQ(matrix(1, 3), 6.7);
	EXPECT_FLOAT_EQ(matrix(1, 2), 10.1);
}

TEST(MatrixTests, MatrixEquality) {
	Eigen::Matrix2f A;
	A << 1, 2,
		3, 4;

	Eigen::Matrix2f B;
	B << 1, 2,
		3, 4.00001f;

	Eigen::Matrix2f C;
	C << 5, 2,
		1, 4.1f;

	float epsilon = 1e-5f;
	bool is_approx = A.isApprox(B, epsilon);

	bool other_approx = A.isApprox(C, epsilon);

	EXPECT_TRUE(is_approx);
	EXPECT_FALSE(other_approx);
}


TEST(MatrixTests, MatrixMultiplication) {
	const float epsilon = 1e-6f;

	Eigen::Matrix2f A;
	A << 1, 2,
			3, 4;
	Eigen::Matrix2f B;
	B << 2, 0,
			1, 2;
	Eigen::Matrix2f Expected;
	Expected << 4, 4,
				10, 8;

	EXPECT_TRUE((A * B).isApprox(Expected, epsilon));
}

TEST(MatrixTests, MatrixMultiplicationByTuple) {
	Tuple t1 = Tuple::vector(1.0f, 2.0f, 3.0f);
	Eigen::Vector4f e_t1;
	e_t1 << t1.x, t1.y, t1.z, t1.z;

	Eigen::Matrix4f matrix;
	matrix << 	1, 2, 3, 4,
				1, 2, 3, 4,
				1, 2, 3, 4,
				1, 2, 3, 4;

	auto mult = matrix * e_t1;
}

TEST(MatrixTests, MatrixIndenity) {
	const float epsilon = 1e-6f;

	Eigen::Matrix4f matrix;
	matrix << 	1, 2, 3, 4,
				1, 2, 3, 4,
				1, 2, 3, 4,
				1, 2, 3, 4;

	Eigen::Matrix4f identityMatrix = Eigen::Matrix4f::Identity();
	auto result = identityMatrix * matrix;
	EXPECT_TRUE((identityMatrix * matrix).isApprox(matrix, epsilon));
}

TEST(MatrixTests, MatrixTranspose) {
	Eigen::Matrix4f matrix;
	matrix << 	1, 8, 9, 4.1,
				6, 2.4, 3, 2,
				0, 1.3, 3, 40,
				1, 2, 8.8, 4;

	matrix.transposeInPlace();

	EXPECT_FLOAT_EQ(matrix(1, 2), 1.3);
}

TEST(MatrixTests, MatrixDeterminate) {
	Eigen::Matrix4f matrix;
	matrix << 	-2, -8, 3, 5,
				-3, 1, 7, 3,
				1, 2, -9, 6,
				-6, 7, 7, -9;

	float det = matrix.determinant();
	EXPECT_EQ(det, -4071);
}

TEST(MatrixTests, MatrixInverse) {
	Eigen::Matrix4f matrixA;
	Eigen::Matrix4f matrixB;
	matrixA << 	-2, -8, 3, 5,
				-3, 1, 7, 3,
				1, 2, -9, 6,
				-6, 7, 7, -9;

	matrixB << 	-2, -8, 3, 5,
				-3, 1, 7, 3,
				1, 2, -9, 6,
				-6, 7, 7, -9;

	Eigen::Matrix4f matrixC = matrixA * matrixB;
	Eigen::Matrix4f inverted_matrixB = matrixB.inverse();

	Eigen::Matrix4f matrixD = matrixC * inverted_matrixB;

	bool is_approx = matrixD.isApprox(matrixA, 1e-6f);
	EXPECT_TRUE(is_approx);
}

TEST(MatrixTests, PointTransform) {
	Affine3f transform(Translation3f(1,2,3));
	Matrix4f translation_matrix = transform.matrix();
	Vector4f e_t1;
	e_t1 << 1, 2.6, 0.6, 1;
	Vector4f transformedFella = translation_matrix * e_t1;

	EXPECT_FLOAT_EQ(transformedFella(1), 4.6);
}

TEST(MatrixTests, VectorTransform) {
	Affine3f transform(Translation3f(1,2,3));
	Matrix4f translation_matrix = transform.matrix();
	Vector4f e_t1;
	e_t1 << 1, 2.6, 0.6, 0;
	Vector4f transformedFella = translation_matrix * e_t1;

	EXPECT_FLOAT_EQ(transformedFella(1), 2.6);
}

TEST(MatrixTests, VectorScale) {
	Vector4f e_t1;
	e_t1 << -4, 6, 8, 0;
	e_t1 = create_scaling_matrix(-2, 3, 4) * e_t1;
	float result = e_t1(1);
	EXPECT_FLOAT_EQ(result, 18);
}

TEST(MatrixTests, PointRotate) {
	Vector4f e_t1;
	e_t1 << 0, 0, 1, 1;
	Matrix4f half_quarter = create_Y_rotation_matrix(M_PI / 4);
	Matrix4f full_quarter = create_Y_rotation_matrix(M_PI / 2);
	e_t1 = half_quarter * full_quarter * e_t1;
	float result = e_t1(1);
	EXPECT_FLOAT_EQ(result, 0);
}

TEST(MatrixTests, PointShear) {
	Vector4f e_t1;
	e_t1 << 2, 3, 4, 1;
	Matrix4f shearer = create_shearing_matrix(0, 0, 0, 1, 0, 0);
	e_t1 = shearer * e_t1;

	float result = e_t1(1);
	EXPECT_FLOAT_EQ(result, 7);
}
/* MATRICES */
/* MATRICES */
/* MATRICES */

#include <gtest/gtest.h>
#include <mutex>
#include <condition_variable>
#include <Eigen/Dense>
#include <Eigen/LU>
#define _USE_MATH_DEFINES
#include <math.h>

#include "tuple.h"
#include "canvas.h"
#include "utils.h"
#include "sphere.h"

using namespace std;
using namespace Eigen;

TEST(TupleTests, PointCreation) {
	Tuple p1 = Tuple::point(1.5f, 5.6f, 2.7f);

	EXPECT_FLOAT_EQ(p1.x, 1.5f);
	EXPECT_FLOAT_EQ(p1.y, 5.6f);
	EXPECT_FLOAT_EQ(p1.z, 2.7f);
	EXPECT_FLOAT_EQ(p1.w, 1.0f);
}

TEST(TupleTests, VectorCreation) {
	Tuple p1 = Tuple::vector(1.5f, 5.6f, 2.7f);

	EXPECT_FLOAT_EQ(p1.x, 1.5f);
	EXPECT_FLOAT_EQ(p1.y, 5.6f);
	EXPECT_FLOAT_EQ(p1.z, 2.7f);
	EXPECT_FLOAT_EQ(p1.w, 0);
}

TEST(TupleTests, Addition) {
	Tuple t1 = Tuple::point(1.0f, 2.0f, 3.0f);
	Tuple t2 = Tuple::vector(4.0f, 5.0f, 6.0f);
	Tuple result = t1 + t2;

	EXPECT_FLOAT_EQ(result.x, 5.0f);
	EXPECT_FLOAT_EQ(result.y, 7.0f);
	EXPECT_FLOAT_EQ(result.z, 9.0f);
	EXPECT_FLOAT_EQ(result.w, 1.0f);
}

TEST(TupleTests, Subtraction) {
	Tuple t1 = Tuple::point(3.0f, 2.0f, 1.0f);
	Tuple t2 = Tuple::vector(1.0f, 2.0f, 3.0f);
	Tuple result = t1 - t2;

	EXPECT_FLOAT_EQ(result.x, 2.0f);
	EXPECT_FLOAT_EQ(result.y, 0.0f);
	EXPECT_FLOAT_EQ(result.z, -2.0f);
	EXPECT_FLOAT_EQ(result.w, 1.0f);
}

TEST(TupleTests, Multiplication) {
	Tuple t1 = Tuple::vector(1.0f, -2.0f, 3.0f);
	Tuple result = t1 * 2.0f;

	EXPECT_FLOAT_EQ(result.x, 2.0f);
	EXPECT_FLOAT_EQ(result.y, -4.0f);
	EXPECT_FLOAT_EQ(result.z, 6.0f);
	EXPECT_FLOAT_EQ(result.w, 0.0f);
}

TEST(TupleTests, Division) {
	Tuple t1 = Tuple::vector(4.0f, -2.0f, 2.0f);
	Tuple result = t1 / 2.0f;

	EXPECT_FLOAT_EQ(result.x, 2.0f);
	EXPECT_FLOAT_EQ(result.y, -1.0f);
	EXPECT_FLOAT_EQ(result.z, 1.0f);
	EXPECT_FLOAT_EQ(result.w, 0.0f);
}

TEST(TupleTests, Negation) {
	Tuple p1 = Tuple::point(1.5f, 5.6f, 2.7f);
	Tuple p2 = -p1;

	EXPECT_FLOAT_EQ(p2.x, -1.5f);
	EXPECT_FLOAT_EQ(p2.y, -5.6f);
	EXPECT_FLOAT_EQ(p2.z, -2.7f);
	EXPECT_FLOAT_EQ(p2.w, -1.0f);
}

TEST(TupleTests, Magnitude) {
	Tuple t1 = Tuple::vector(1.0f, 2.0f, 3.0f);
	double result = t1.magnitude();
	EXPECT_DOUBLE_EQ(result, std::sqrt(14.0));
}

TEST(TupleTests, Normalization) {
	Tuple t1 = Tuple::vector(1.0f, 2.0f, 3.0f);
	Tuple result = t1.normalize();
	
	double mag = t1.magnitude();

	EXPECT_FLOAT_EQ(result.x, 1.0f / mag);
	EXPECT_FLOAT_EQ(result.y, 2.0f / mag);
	EXPECT_FLOAT_EQ(result.z, 3.0f / mag);
	EXPECT_FLOAT_EQ(result.w, 0.0f);
}

TEST(TupleTests, CrossProduct) {
	Tuple a = Tuple::vector(1.0f, 2.0f, 3.0f);
	Tuple b = Tuple::vector(2.0f, 3.0f, 4.0f);

	Tuple result = a.cross(b);

	EXPECT_FLOAT_EQ(result.x, -1.0f);
	EXPECT_FLOAT_EQ(result.y, 2.0f);
	EXPECT_FLOAT_EQ(result.z, -1.0f);
	EXPECT_FLOAT_EQ(result.w, 0.0f);
}


/* COLORS */
/* COLORS */
/* COLORS */
TEST(TupleTests, ColorCreation) {
	Tuple c1 = Tuple::color(1.5f, 5.6f, 2.7f);

	EXPECT_FLOAT_EQ(c1.red(), 1.5f);
	EXPECT_FLOAT_EQ(c1.green(), 5.6f);
	EXPECT_FLOAT_EQ(c1.blue(), 2.7f);
	EXPECT_FLOAT_EQ(c1.w, 0.0f);
}
// If the vector and point operation tests work, then theyll work for colors.


TEST(TupleTests, ToRGB) {
	// Test with exact 0-1 range values
	Tuple color1 = Tuple::color(0.0f, 0.5f, 1.0f);
	Tuple::RGB rgb1 = color1.to_RGB();
	EXPECT_EQ(rgb1.r, 0);
	EXPECT_EQ(rgb1.g, 128);
	EXPECT_EQ(rgb1.b, 255);

	// Test with values that require rounding
	Tuple color2 = Tuple::color(0.2f, 0.8f, 0.4f);
	Tuple::RGB rgb2 = color2.to_RGB();
	EXPECT_EQ(rgb2.r, 51);  // 0.2 * 255 = 51
	EXPECT_EQ(rgb2.g, 204); // 0.8 * 255 = 204
	EXPECT_EQ(rgb2.b, 102); // 0.4 * 255 = 102

	// Test with out-of-range values (should be clamped)
	Tuple color3 = Tuple::color(-0.5f, 1.5f, 1.0f);
	Tuple::RGB rgb3 = color3.to_RGB();
	EXPECT_EQ(rgb3.r, 0);   // Clamped to 0
	EXPECT_EQ(rgb3.g, 255); // Clamped to 255
	EXPECT_EQ(rgb3.b, 255);
}
/* COLORS */
/* COLORS */
/* COLORS */


/* CCCANVAS */
/* CCCANVAS */
/* CCCANVAS */
TEST(CanvasTests, CanvasCreation) {
	Canvas c = Canvas(100, 200);
	// Tuple col = Tuple::color(1, 0.2, 0.2);
	// c.write_pixel(50, 50, col);

	EXPECT_EQ(c.width, 100);
	EXPECT_EQ(c.height, 200); 
}

TEST(CanvasTests, CanvasWritePixel) {
	Canvas c = Canvas(100, 200);
	Tuple col = Tuple::color(1, 0.2, 0.2);

	int x = 30;
	int y = 80;

	c.write_pixel(x, y, col, 1);

	EXPECT_EQ(c.pixels[y * c.width + x], col);
}

/* CCCANVAS */
/* CCCANVAS */
/* CCCANVAS */


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


/* SPHERES */
/* SPHERES */
/* SPHERES */
TEST(SphereTests, RayTest) {
	Ray r = {
		Point3D(5, 4, 6),
		Vector3D(1, 1, 1)
	};
	EXPECT_FLOAT_EQ(r.origin.data[1], 4);
}

TEST(SphereTests, PositionTest) {
	Ray r = {
		Point3D(2, 3, 4),
		Vector3D(1, 0, 0)
	};
	Point3D end = position(r, 2.5);
	EXPECT_FLOAT_EQ(end.x(), 4.5);
}

TEST(SphereTests, SphereCreationTest) {
	Sphere s;
	EXPECT_EQ(s.radius, 1);
}
/* SPHERES */
/* SPHERES */
/* SPHERES */



// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
}
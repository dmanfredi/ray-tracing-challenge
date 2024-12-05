#include <gtest/gtest.h>
#include <mutex>
#include <condition_variable>
#include <Eigen/Dense>
#include <Eigen/LU>
#define _USE_MATH_DEFINES
#include <math.h>

#include "utils.h"
#include "tuple.h"

#include "sphere.h"
#include "shape.h"
#include "intersection.h"

using namespace std;
using namespace Eigen;

const float SQRT2_OVER_2 = sqrt(2.0f) / 2.0f;
const float PI_OVER_2 = M_PI / 2.0f;

/* SPHERES */
/* SPHERES */
/* SPHERES */
TEST(SphereTests, RayTest) {
	Ray r = {
		Point3D(5, 4, 6),
		Vector3D(1, 1, 1)
	};
	EXPECT_FLOAT_EQ(r.origin.cords[1], 4);
}

TEST(SphereTests, PositionTest) {
	Ray r = {
		Point3D(2, 3, 4),
		Vector3D(1, 0, 0)
	};
	Point3D end = position(r, 2.5);
	EXPECT_FLOAT_EQ(end.x(), 4.5);
}

TEST(SphereTests, SphereCreationTestGlance) {
	Sphere s;
	EXPECT_EQ(s.radius, 1);
}

TEST(SphereTests, SphereIntersection) {
	Sphere s;
	Ray r = {
		Point3D(0, 1, -5),
		Vector3D(0, 0, 1)
	};
	vector<Intersection> intersections = intersect(s, r);
	EXPECT_EQ(intersections.size(), 2);
	EXPECT_FLOAT_EQ(intersections[0].t, 5.0);
	EXPECT_FLOAT_EQ(intersections[1].t, 5.0);
}

TEST(SphereTests, SphereIntersectionInside) {
	Sphere s;
	Ray r = {
		Point3D(0, 0, 0),
		Vector3D(0, 0, 1)
	};
	vector<Intersection> intersections = intersect(s, r);
	EXPECT_EQ(intersections.size(), 2);
	EXPECT_FLOAT_EQ(intersections[0].t, -1);
	EXPECT_FLOAT_EQ(intersections[1].t, 1);
}

TEST(SphereTests, SphereIntersectionMiss) {
	Sphere s;
	Ray r = {
		Point3D(0, 2, -5),
		Vector3D(0, 0, 1)
	};
	vector<Intersection> intersections = intersect(s, r);
	EXPECT_EQ(intersections.size(), 0);
}

TEST(SphereTests, SphereHasIdentityTransformation) {
	const float epsilon = 1e-6f;
	Sphere s;

	Matrix4f identityMatrix = Matrix4f::Identity();

	EXPECT_TRUE(identityMatrix.isApprox(s.transform, epsilon));
}


TEST(SphereTests, SphereNormals) {
	Sphere s;

	// The normal on a sphere at a point on the x axis
	Vector3D norm = normal_at(s, Point3D(1, 0, 0));
	EXPECT_TRUE(norm.cords.isApprox(Vector4f(1, 0, 0, 0)));

	// The normal on a sphere at the point on the y axis
	Vector3D norm2 =  normal_at(s, Point3D(0, 1, 0));
	EXPECT_TRUE(norm2.cords.isApprox(Vector4f(0, 1, 0, 0)));

	// The normal on a sphere at the point on the z axis
	Vector3D norm3 =  normal_at(s, Point3D(0, 0, 1));
	EXPECT_TRUE(norm3.cords.isApprox(Vector4f(0, 0, 1, 0)));

	const float SQRT3_OVER_3 = sqrt(3.0f) / 3.0f;

	// The normal on a sphere at a nonaxial point
	Vector3D norm4 =  normal_at(s, Point3D(SQRT3_OVER_3, SQRT3_OVER_3, SQRT3_OVER_3));
	EXPECT_TRUE(norm4.cords.isApprox(Vector4f(SQRT3_OVER_3, SQRT3_OVER_3, SQRT3_OVER_3, 0),  1e-5f));
}

TEST(SphereTests, NormalIsNormal) {
	Sphere s;
	const float SQRT3_OVER_3 = sqrt(3.0f) / 3.0f;
	Vector3D norm = normal_at(s, Point3D(SQRT3_OVER_3, SQRT3_OVER_3, SQRT3_OVER_3));
	EXPECT_TRUE(norm.cords.isApprox(Vector4f(norm.cords.normalized()), 1e-5f));
}

TEST(SphereTests, NormalOnTranslation) {
	Sphere s;
	Affine3f transform(Translation3f(0,1,0));
	Matrix4f translation_matrix = transform.matrix();

	s.transform = translation_matrix;

	Vector3D norm = normal_at(s, Point3D(0.0, 1.70711, -0.70711));
	EXPECT_TRUE(norm.cords.isApprox(Vector4f(0.0, 0.70711, -0.70711, 0.0), 1e-5f));
}

TEST(SphereTests, NormalOnTransformed) {
	Sphere s;
	Matrix4f scaling_matrix = create_scaling_matrix(1, 0.5, 1);
	Matrix4f z_rot = create_Y_rotation_matrix(M_PI / 5);
	Matrix4f transformation_matrix = scaling_matrix * z_rot;
	s.transform = transformation_matrix;


	Vector3D norm = normal_at(s, Point3D(0, SQRT2_OVER_2, -SQRT2_OVER_2));
	EXPECT_TRUE(norm.cords.isApprox(Vector4f(0.0, 0.97014, -0.24254, 0.0), 1e-5f));
}
/* SPHERES */
/* SPHERES */
/* SPHERES */
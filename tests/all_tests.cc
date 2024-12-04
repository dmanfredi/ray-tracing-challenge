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
#include "world.h"
#include "intersection.h"
#include "light.h"
#include "material.h"
#include "console.h"
#include "colors.h"


using namespace std;
using namespace Eigen;

const float SQRT2_OVER_2 = sqrt(2.0f) / 2.0f;
const float PI_OVER_2 = M_PI / 2.0f;

World create_default_world() {
	World default_world;
	
	default_world.light = Light(Point3D(-10, 10, -10), Tuple::color(1, 1, 1));
	
	Sphere s1;
	Material m1;
	m1.color = Tuple::color(0.8, 1.0, 0.6);
	m1.diffuse = 0.7;
	m1.specular = 0.2;
	s1.material = m1;
	default_world.objects.push_back(s1);
	
	Sphere s2;
	s2.transform = create_scaling_matrix(0.5, 0.5, 0.5);
	default_world.objects.push_back(s2);
	
	return default_world;
}


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


/* REFLECTION */
/* REFLECTION */
/* REFLECTION */
TEST(ReflectionTests, ReflectionBasic) {
	// Basic 45 degree approach
	Vector3D v = Vector3D(1, -1, 0);
	Vector3D n = Vector3D(0, 1, 0);

	Vector3D r = reflect(v, n);

	EXPECT_TRUE(r.cords.isApprox(Vector4f(1.0, 1.0, 0.0, 0.0), 1e-5f));
}

TEST(ReflectionTests, ReflectionAngled) {
	// Like dropping a ball onto a slanted surface
	Vector3D v = Vector3D(0, -1, 0);
	Vector3D n = Vector3D(SQRT2_OVER_2, SQRT2_OVER_2, 0);


	Vector3D r = reflect(v, n);

	EXPECT_TRUE(r.cords.isApprox(Vector4f(1.0, 0.0, 0.0, 0.0), 1e-5f));
}
/* REFLECTION */
/* REFLECTION */
/* REFLECTION */

/* LIGHTING */
/* LIGHTING */
/* LIGHTING */
TEST(LightingTests, LightCreation) {
	// Like dropping a ball onto a slanted surface
	Point3D pos = Point3D(0,0,0);
	Tuple intensity = Tuple::color(1.0, 1.0, 1.0);

	Light light = Light();


	EXPECT_TRUE(pos.cords.isApprox(light.posistion.cords, 1e-5f));
	EXPECT_TRUE(intensity == light.intensity);
}  

TEST(LightingTests, MaterialCreation) {
	Material m = Material();


	EXPECT_TRUE(m.color == Tuple::color(1.0, 1.0, 1.0));
	EXPECT_FLOAT_EQ(m.ambient, 0.1);
	EXPECT_FLOAT_EQ(m.diffuse, 0.9);
	EXPECT_FLOAT_EQ(m.specular, 0.9);
	EXPECT_FLOAT_EQ(m.shininess, 200.0);
} 

TEST(LightingTests, DirectLight) {
	// Lighting with the eye between the light and the surface
	Material m = Material();
	Point3D position = Point3D(0, 0, 0);

	Vector3D eyev = Vector3D(0, 0, -1);
	Vector3D normalv = Vector3D(0, 0, -1);
	Light light = Light(Point3D(0, 0, -10), Tuple::color(1, 1, 1));

	Tuple result = lighting(m, light, position, eyev, normalv, false);

	EXPECT_TRUE(result == Tuple::color(1.9, 1.9, 1.9));
}

TEST(LightingTests, DirectLightShadow) {
	// Lighting with the eye between the light and the surface
	Material m = Material();
	Point3D position = Point3D(0, 0, 0);

	Vector3D eyev = Vector3D(0, 0, -1);
	Vector3D normalv = Vector3D(0, 0, -1);
	Light light = Light(Point3D(0, 0, -10), Tuple::color(1, 1, 1));
	bool inShadow = true;

	Tuple result = lighting(m, light, position, eyev, normalv, inShadow);

	EXPECT_TRUE(result == Tuple::color(0.1, 0.1, 0.1));
}

TEST(LightingTests, OffsetEyeFromLight) {
	// Lighting with the eye between light and surface, angled at 45 degrees.
	Material m = Material();
	Point3D position = Point3D(0, 0, 0);

	Vector3D eyev = Vector3D(0, SQRT2_OVER_2, -SQRT2_OVER_2);
	Vector3D normalv = Vector3D(0, 0, -1);
	Light light = Light(Point3D(0, 0, -10), Tuple::color(1, 1, 1));

	Tuple result = lighting(m, light, position, eyev, normalv, false);
	EXPECT_TRUE(result == Tuple::color(1.0, 1.0, 1.0));
}

TEST(LightingTests, HeadOnLightOffset) {
	// Lighting with the eye opposite surface, light offset 45 degrees
	Material m = Material();
	Point3D position = Point3D(0, 0, 0);

	Vector3D eyev = Vector3D(0, 0, -1);
	Vector3D normalv = Vector3D(0, 0, -1);
	Light light = Light(Point3D(0, 10, -10), Tuple::color(1, 1, 1));

	Tuple result = lighting(m, light, position, eyev, normalv, false);
	EXPECT_TRUE(result == Tuple::color(0.7364, 0.7364, 0.7364));
}

TEST(LightingTests, EyeInPathOfReflectionVector) {
	// Lighting with the eye in the path of the reflection vector
	Material m = Material();
	Point3D position = Point3D(0, 0, 0);

	Vector3D eyev = Vector3D(0, -SQRT2_OVER_2, -SQRT2_OVER_2);
	Vector3D normalv = Vector3D(0, 0, -1);
	Light light = Light(Point3D(0, 10, -10), Tuple::color(1, 1, 1));

	Tuple result = lighting(m, light, position, eyev, normalv, false);
	EXPECT_TRUE(result == Tuple::color(1.6364, 1.6364, 1.6364));
}

TEST(LightingTests, LightObscured) {
	// Lighting with the light behind the surface
	Material m = Material();
	Point3D position = Point3D(0, 0, 0);

	Vector3D eyev = Vector3D(0, 0, -1);
	Vector3D normalv = Vector3D(0, 0, -1);
	Light light = Light(Point3D(0, 0, 10), Tuple::color(1, 1, 1));

	Tuple result = lighting(m, light, position, eyev, normalv, false);
	EXPECT_TRUE(result == Tuple::color(0.1, 0.1, 0.1));
}

TEST(ShadowTests, NoShadow) {
	// the light and point are in direct view of each other
	World w = create_default_world();
	Point3D p = Point3D(0, 10, 0);

	EXPECT_FALSE(is_shadowed(w, p));
}

TEST(ShadowTests, ThereIsShadow) {
	// there is an object between the light and the point
	World w = create_default_world();
	Point3D p = Point3D(10, -10, 10);

	EXPECT_TRUE(is_shadowed(w, p));
}

TEST(ShadowTests, NoShadowTwo) {
	// Point -> light -> object. No shadow
	World w = create_default_world();
	Point3D p = Point3D(-20, 20, -20);

	EXPECT_FALSE(is_shadowed(w, p));
}

TEST(ShadowTests, NoShadowThree) {
	// Light -> point -> object. No shadow
	World w = create_default_world();
	Point3D p = Point3D(-2, 2, -2);

	EXPECT_FALSE(is_shadowed(w, p));
}

TEST(ShadowTests, FullShadowTest) {
	World w = create_default_world();
	w.light = Light(Point3D(0, 0, -10), colors::White);
	Sphere s1 = Sphere();
	w.objects.push_back(s1);

	Sphere s2 = Sphere();
	s2.transform = create_translation_matrix(0, 0, 10);
	w.objects.push_back(s2);

	Ray r = Ray(Point3D(0, 0, 5), Vector3D(0, 0, 1));
	Intersection i = Intersection(4, s2);

	Computation comps = prepare_compuation(i, r);
	Tuple color = shade_hit(w, comps);

	EXPECT_TRUE(color == Tuple::color(0.1, 0.1, 0.1));
}

TEST(ShadowTests, OverPointTest) {
	float EPSILON = 1e-5f;
	Ray r = Ray(Point3D(0, 0, -5), Vector3D(0, 0, 1));
	Sphere shape = Sphere();
	shape.transform = create_translation_matrix(0, 0, 1);

	Intersection i = Intersection(5, shape);
	Computation comps = prepare_compuation(i, r);

	EXPECT_TRUE(comps.over_point.z() < -EPSILON / 2);
	EXPECT_TRUE(comps.point.z() > comps.over_point.z());
}

/* LIGHTING */
/* LIGHTING */
/* LIGHTING */

/* INTERSECTIONS & HITS */
/* INTERSECTIONS & HITS */
/* INTERSECTIONS & HITS */
TEST(IntersectionAndHitTests, IntersectionCreation) {
	Sphere s;
	Intersection i = Intersection(3.5, s);

	EXPECT_FLOAT_EQ(i.t, 3.5);
	EXPECT_EQ(s.origin.cords.x(), 0);
}

TEST(IntersectionAndHitTests, HitTest) {
	Sphere s;

	vector<Intersection> i1 = { Intersection(5, s) };
	vector<Intersection> i2 = { Intersection(7, s) };
	vector<Intersection> i3 = { Intersection(-3, s) };
	vector<Intersection> i4 = { Intersection(2, s) };

	vector<Intersection> all_intersections = flatten_intersections({i1, i2, i3, i4});

	// Test case 1: Hit should be the closest positive intersection
	auto result = hit(all_intersections);
	ASSERT_TRUE(result.has_value());
	EXPECT_EQ(result.value().t, 2);

	// Test case 2: All negative intersections
	vector<Intersection> negative_intersections = { Intersection(-1, s), Intersection(-2, s) };
	result = hit(negative_intersections);
	EXPECT_FALSE(result.has_value());

	// Test case 3: Mixed positive and negative intersections
	vector<Intersection> mixed_intersections = { Intersection(-1, s), Intersection(1, s), Intersection(2, s) };
	result = hit(mixed_intersections);
	ASSERT_TRUE(result.has_value());
	EXPECT_EQ(result.value().t, 1);

	// Test case 4: Empty intersection list
	vector<Intersection> empty_intersections;
	result = hit(empty_intersections);
	EXPECT_FALSE(result.has_value());
}

TEST(IntersectionAndHitTests, IntersectScaledSphere) {
	Ray r = {
		Point3D(0, 0, -5),
		Vector3D(0, 0, 1)
	};
	Sphere s;

	Matrix4f scaling_matrix = create_scaling_matrix(2, 2, 2);

	s.transform = scaling_matrix;

	vector<Intersection> intersects = intersect(s, r);


	EXPECT_EQ(intersects.size(), 2);
	EXPECT_FLOAT_EQ(intersects[0].t, 3.0);
	EXPECT_FLOAT_EQ(intersects[1].t, 7.0);
}

TEST(IntersectionAndHitTests, IntersectTranslatedSphere) {
	Ray r = {
		Point3D(0, 0, -5),
		Vector3D(0, 0, 1)
	};
	Sphere s;

	Affine3f transform(Translation3f(3,4,5));
	Matrix4f translation_matrix = transform.matrix();

	s.transform = translation_matrix;

	vector<Intersection> intersects = intersect(s, r);

	EXPECT_EQ(intersects.size(), 0);
}

TEST(IntersectionAndHitTests, IntersectWorld) {
	World w = create_default_world();
	Ray r = Ray{
		Point3D(0, 0, -5),
		Vector3D(0, 0, 1)
	};

	vector<Intersection> intersections = intersect_world(w, r);


	EXPECT_EQ(intersections.size(), 4);

	EXPECT_FLOAT_EQ(intersections[0].t, 4);
	EXPECT_FLOAT_EQ(intersections[1].t, 4.5);
	EXPECT_FLOAT_EQ(intersections[2].t, 5.5);
	EXPECT_FLOAT_EQ(intersections[3].t, 6);
}

TEST(IntersectionAndHitTests, Precomputations) {
	Ray r = Ray{
		Point3D(0, 0, -5),
		Vector3D(0, 0, 1)
	};
	Sphere shape = Sphere();
	Intersection i = Intersection(4, shape);

	Computation comps = prepare_compuation(i, r);


	EXPECT_FLOAT_EQ(comps.t, i.t);
	EXPECT_TRUE(shape.origin.cords.isApprox(comps.object.origin.cords));
	EXPECT_TRUE(comps.point.cords.isApprox(Point3D(0, 0, -1).cords));
	EXPECT_TRUE(comps.eyev.cords.isApprox(Vector3D(0, 0, -1).cords));
	EXPECT_TRUE(comps.normalv.cords.isApprox(Vector3D(0, 0, -1).cords));
	EXPECT_FALSE(comps.inside);
}

TEST(IntersectionAndHitTests, PrecomputationsInside) {
	Ray r = Ray{
		Point3D(0, 0, 0),
		Vector3D(0, 0, 1)
	};
	Sphere shape = Sphere();
	Intersection i = Intersection(4, shape);

	Computation comps = prepare_compuation(i, r);

	EXPECT_TRUE(comps.normalv.cords.isApprox(Vector3D(0, 0, -1).cords));
	EXPECT_TRUE(comps.inside);
}

TEST(IntersectionAndHitTests, ShadeHitOutside) {
	World w = create_default_world();
	Ray r = Ray{
		Point3D(0, 0, -5),
		Vector3D(0, 0, 1)
	};


	cout << "First sphere material color: ";
	w.objects[0].material.color.print();


	Intersection i = Intersection(4, w.objects[0]);
	Computation comps = prepare_compuation(i, r);
	Tuple color = shade_hit(w, comps);

	color.print();
	Tuple::color(0.38066, 0.47583, 0.90498).print();

	EXPECT_TRUE(color == Tuple::color(0.38066, 0.47583, 0.2855));
}

TEST(IntersectionAndHitTests, ShadeHitInside) {
	World w = create_default_world();
	w.light = Light(Point3D(0, 0.25, 0), Tuple::color(1, 1, 1));
	Ray r = Ray{
		Point3D(0, 0, 0),
		Vector3D(0, 0, 1)
	};
	Intersection i = Intersection(0.5, w.objects[1]);
	Computation comps = prepare_compuation(i, r);
	Tuple color = shade_hit(w, comps);

	// color.print();

	EXPECT_TRUE(color == Tuple::color(0.90498, 0.90498, 0.90498));
}

TEST(IntersectionAndHitTests, ColorAtMiss) {
	World w = create_default_world();
	Ray r = Ray{
		Point3D(0, 0, -5),
		Vector3D(0, 1, 0)
	};
	Tuple color = color_at(w, r);
	EXPECT_TRUE(color == Tuple::color(0.0, 0.0, 0.0));
}

TEST(IntersectionAndHitTests, ColorAtHit) {
	World w = create_default_world();
	Ray r = Ray{
		Point3D(0, 0, -5),
		Vector3D(0, 0, 1)
	};
	Tuple color = color_at(w, r);
	EXPECT_TRUE(color == Tuple::color(0.38066, 0.47583, 0.2855));
}

// The color with an intersection behind the ray pg 97
TEST(IntersectionAndHitTests, ColorAtIntersectionBehindRay) {
	World w = create_default_world();
	w.objects[0].material.ambient = 1;
	w.objects[1].material.ambient = 1;
	Ray r = Ray{
		Point3D(0, 0, 0.75),
		Vector3D(0, 0, -1)
	};
	Tuple color = color_at(w, r);
	EXPECT_TRUE(color == w.objects[1].material.color);
}
/* INTERSECTIONS & HITS */
/* INTERSECTIONS & HITS */
/* INTERSECTIONS & HITS */

/* RAYS */
/* RAYS */
/* RAYS */
TEST(RayTests, TranslateRay) {

	Affine3f transform(Translation3f(3,4,5));
	Matrix4f translation_matrix = transform.matrix();

	Ray r = {
		Point3D(1, 2, 3),
		Vector3D(0, 1, 0)
	};

	Ray translated_ray = transform_ray(r, translation_matrix);

	EXPECT_FLOAT_EQ(translated_ray.origin.x(), 4.0f);
	EXPECT_FLOAT_EQ(translated_ray.origin.y(), 6.0f);
	EXPECT_FLOAT_EQ(translated_ray.origin.z(), 8.0f);

	EXPECT_FLOAT_EQ(translated_ray.direction.x(), 0.0f);
	EXPECT_FLOAT_EQ(translated_ray.direction.y(), 1.0f);
	EXPECT_FLOAT_EQ(translated_ray.direction.z(), 0.0f);
}

TEST(RayTests, ScaleRay) {
	Matrix4f scaling_matrix = create_scaling_matrix(2, 3, 4);

	Ray r = {
		Point3D(1, 2, 3),
		Vector3D(0, 1, 0)
	};

	Ray scaled_ray = transform_ray(r, scaling_matrix);

	EXPECT_FLOAT_EQ(scaled_ray.origin.x(), 2.0f);
	EXPECT_FLOAT_EQ(scaled_ray.origin.y(), 6.0f);
	EXPECT_FLOAT_EQ(scaled_ray.origin.z(), 12.0f);

	EXPECT_FLOAT_EQ(scaled_ray.direction.x(), 0.0f);
	EXPECT_FLOAT_EQ(scaled_ray.direction.y(), 3.0f);
	EXPECT_FLOAT_EQ(scaled_ray.direction.z(), 0.0f);
}
/* RAYS */
/* RAYS */
/* RAYS */

/* WORLD */
/* WORLD */
/* WORLD */
TEST(WorldTests, BasicWorld) {
	World w = create_default_world();
	ASSERT_EQ(w.objects.size(), 2);
}

TEST(WorldTests, DefaultView) {
	Point3D from = Point3D(0, 0, 0);
	Point3D to = Point3D(0, 0, -1);
	Vector3D up = Vector3D(0, 1, 0);

	Matrix4f t = view_transform(from, to, up);

	EXPECT_TRUE(t.isApprox(Matrix4f::Identity()));
}

TEST(WorldTests, PostiveZView) {
	Point3D from = Point3D(0, 0, 0);
	Point3D to = Point3D(0, 0, 1);
	Vector3D up = Vector3D(0, 1, 0);

	Matrix4f t = view_transform(from, to, up);
	Matrix4f scale = create_scaling_matrix(-1.0, 1.0, -1.0);

	EXPECT_TRUE(t.isApprox(scale));
}

TEST(WorldTests, ViewFrom) {
	Point3D from = Point3D(0, 0, 8);
	Point3D to = Point3D(0, 0, 0);
	Vector3D up = Vector3D(0, 1, 0);

	Matrix4f t = view_transform(from, to, up);

	Affine3f transform(Translation3f(0, 0, -8));
	Matrix4f translation_matrix = transform.matrix();

	EXPECT_TRUE(t.isApprox(translation_matrix));
}

TEST(WorldTests, ArbitraryView) { // pg 99
	Point3D from = Point3D(1, 3, 2);
	Point3D to = Point3D(4, -2, 8);
	Vector3D up = Vector3D(1, 1, 0);

	Matrix4f t = view_transform(from, to, up);

	EXPECT_NEAR(t(0, 1), 0.50709, 1e-5);
	EXPECT_NEAR(t(1, 2), 0.12122, 1e-5);
	EXPECT_NEAR(t(2, 2), -0.71714, 1e-5);
	EXPECT_FLOAT_EQ(t(3, 3), 1.0);
}

TEST(WorldTests, CameraCreation) {
	Camera camera = Camera(160, 120, PI_OVER_2);

	EXPECT_EQ(camera.hsize, 160);
	EXPECT_EQ(camera.vsize, 120);
	EXPECT_FLOAT_EQ(camera.field_of_view, PI_OVER_2);
	EXPECT_TRUE(camera.transform.isApprox(Eigen::Matrix4f::Identity()));
}

TEST(WorldTests, CameraPixelSize) {
	// horizontal canvas
	Camera camera = Camera(200, 125, PI_OVER_2);
	EXPECT_FLOAT_EQ(camera.pixel_size, 0.01);

	// veritcal canvas
	Camera camera2 = Camera(125, 200, PI_OVER_2);
	EXPECT_FLOAT_EQ(camera2.pixel_size, 0.01);
}

TEST(WorldTests, RayThroughCanvas) {
	Camera camera = Camera(201, 101, PI_OVER_2);
	Ray r = ray_for_pixel(camera, 100, 50);

	EXPECT_TRUE(r.origin.cords.isApprox(Point3D(0, 0, 0).cords));
	EXPECT_TRUE(r.direction.cords.isApprox(Vector3D(0, 0, -1).cords));
}

TEST(WorldTests, RayThroughCanvasCorner) {
	Camera camera = Camera(201, 101, PI_OVER_2);
	Ray r = ray_for_pixel(camera, 0, 0);

	EXPECT_TRUE(r.origin.cords.isApprox(Point3D(0, 0, 0).cords));
	EXPECT_TRUE(r.direction.cords.isApprox(Vector3D(0.66519, .33259, -0.6685).cords, 1e-4f));
}

TEST(WorldTests, RayThroughTransformedCanvas) {
	Camera camera = Camera(201, 101, PI_OVER_2);
	camera.transform = create_Y_rotation_matrix(M_PI / 4) * create_translation_matrix(0, -2, 5);
	Ray r = ray_for_pixel(camera, 100, 50);
	
	EXPECT_TRUE(r.origin.cords.isApprox(Point3D(0, 2, -5).cords));
	EXPECT_TRUE(r.direction.cords.isApprox(Vector3D(SQRT2_OVER_2, 0, -SQRT2_OVER_2).cords, 1e-4f));
}

TEST(WorldTests, EndToEndWorld) {
	World w = create_default_world();

	Camera camera = Camera(11, 11, PI_OVER_2);
	Point3D from = Point3D(0, 0, -5);
	Point3D to = Point3D(0, 0, 0);
	Vector3D up = Vector3D(0, 1, 0);
	camera.transform = view_transform(from, to, up);
	Canvas image = render(camera, w); 


	Tuple who = image.pixel_at(5, 5);

	EXPECT_TRUE(who == Tuple::color(0.3806, 0.4758, 0.2855));
	// camera.transform = create_Y_rotation_matrix(M_PI / 4) * create_translation_matrix(0, -2, 5);
	// Ray r = ray_for_pixel(camera, 100, 50);
	
}

/* WORLD */
/* WORLD */
/* WORLD */


// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
}
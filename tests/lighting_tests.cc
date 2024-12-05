
#include <gtest/gtest.h>
#include <mutex>
#include <condition_variable>
#include <Eigen/Dense>
#include <Eigen/LU>
#define _USE_MATH_DEFINES
#include <math.h>

#include "sphere.h"
#include "intersection.h"
#include "tests/test_utils.h"
#include "colors.h"

using namespace std;
using namespace Eigen;

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
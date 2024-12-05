
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
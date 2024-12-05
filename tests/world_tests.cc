
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

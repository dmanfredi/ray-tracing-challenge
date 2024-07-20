#include <gtest/gtest.h>
#include <mutex>
#include <condition_variable>

#include "tuple.h"
#include "canvas.h"

TEST(TupleTest, PointCreation) {
	Tuple p1 = Tuple::point(1.5f, 5.6f, 2.7f);

	EXPECT_FLOAT_EQ(p1.x, 1.5f);
	EXPECT_FLOAT_EQ(p1.y, 5.6f);
	EXPECT_FLOAT_EQ(p1.z, 2.7f);
	EXPECT_FLOAT_EQ(p1.w, 1.0f);
}

TEST(TupleTest, VectorCreation) {
	Tuple p1 = Tuple::vector(1.5f, 5.6f, 2.7f);

	EXPECT_FLOAT_EQ(p1.x, 1.5f);
	EXPECT_FLOAT_EQ(p1.y, 5.6f);
	EXPECT_FLOAT_EQ(p1.z, 2.7f);
	EXPECT_FLOAT_EQ(p1.w, 0);
}

TEST(TupleTest, Addition) {
	Tuple t1 = Tuple::point(1.0f, 2.0f, 3.0f);
	Tuple t2 = Tuple::vector(4.0f, 5.0f, 6.0f);
	Tuple result = t1 + t2;

	EXPECT_FLOAT_EQ(result.x, 5.0f);
	EXPECT_FLOAT_EQ(result.y, 7.0f);
	EXPECT_FLOAT_EQ(result.z, 9.0f);
	EXPECT_FLOAT_EQ(result.w, 1.0f);
}

TEST(TupleTest, Subtraction) {
	Tuple t1 = Tuple::point(3.0f, 2.0f, 1.0f);
	Tuple t2 = Tuple::vector(1.0f, 2.0f, 3.0f);
	Tuple result = t1 - t2;

	EXPECT_FLOAT_EQ(result.x, 2.0f);
	EXPECT_FLOAT_EQ(result.y, 0.0f);
	EXPECT_FLOAT_EQ(result.z, -2.0f);
	EXPECT_FLOAT_EQ(result.w, 1.0f);
}

TEST(TupleTest, Multiplication) {
	Tuple t1 = Tuple::vector(1.0f, -2.0f, 3.0f);
	Tuple result = t1 * 2.0f;

	EXPECT_FLOAT_EQ(result.x, 2.0f);
	EXPECT_FLOAT_EQ(result.y, -4.0f);
	EXPECT_FLOAT_EQ(result.z, 6.0f);
	EXPECT_FLOAT_EQ(result.w, 0.0f);
}

TEST(TupleTest, Division) {
	Tuple t1 = Tuple::vector(4.0f, -2.0f, 2.0f);
	Tuple result = t1 / 2.0f;

	EXPECT_FLOAT_EQ(result.x, 2.0f);
	EXPECT_FLOAT_EQ(result.y, -1.0f);
	EXPECT_FLOAT_EQ(result.z, 1.0f);
	EXPECT_FLOAT_EQ(result.w, 0.0f);
}

TEST(TupleTest, Negation) {
	Tuple p1 = Tuple::point(1.5f, 5.6f, 2.7f);
	Tuple p2 = -p1;

	EXPECT_FLOAT_EQ(p2.x, -1.5f);
	EXPECT_FLOAT_EQ(p2.y, -5.6f);
	EXPECT_FLOAT_EQ(p2.z, -2.7f);
	EXPECT_FLOAT_EQ(p2.w, -1.0f);
}

TEST(TupleTest, Magnitude) {
	Tuple t1 = Tuple::vector(1.0f, 2.0f, 3.0f);
	double result = t1.magnitude();
	EXPECT_DOUBLE_EQ(result, std::sqrt(14.0));
}

TEST(TupleTest, Normalization) {
	Tuple t1 = Tuple::vector(1.0f, 2.0f, 3.0f);
	Tuple result = t1.normalize();
	
	double mag = t1.magnitude();

	EXPECT_FLOAT_EQ(result.x, 1.0f / mag);
	EXPECT_FLOAT_EQ(result.y, 2.0f / mag);
	EXPECT_FLOAT_EQ(result.z, 3.0f / mag);
	EXPECT_FLOAT_EQ(result.w, 0.0f);
}

TEST(TupleTest, CrossProduct) {
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
TEST(TupleTest, ColorCreation) {
	Tuple c1 = Tuple::color(1.5f, 5.6f, 2.7f);

	EXPECT_FLOAT_EQ(c1.red(), 1.5f);
	EXPECT_FLOAT_EQ(c1.green(), 5.6f);
	EXPECT_FLOAT_EQ(c1.blue(), 2.7f);
	EXPECT_FLOAT_EQ(c1.w, 0.0f);
}
// If the vector and point operation tests work, then theyll work for colors.


TEST(TupleTest, ToRGB) {
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
TEST(CanvasTest, CanvasCreation) {
	Canvas c = Canvas(100, 200);
	// Tuple col = Tuple::color(1, 0.2, 0.2);
	// c.write_pixel(50, 50, col);

	EXPECT_EQ(c.width, 100);
	EXPECT_EQ(c.height, 200); 
}

TEST(CanvasTest, CanvasWritePixel) {
	Canvas c = Canvas(100, 200);
	Tuple col = Tuple::color(1, 0.2, 0.2);

	int x = 30;
	int y = 80;

	c.write_pixel(x, y, col);

	EXPECT_EQ(c.pixels[y * c.width + x], col);
}

/* CCCANVAS */
/* CCCANVAS */
/* CCCANVAS */


/* MATRICES */
/* MATRICES */
/* MATRICES */
// TEST(CanvasTest, CanvasCreation) {
// 	Canvas c = Canvas(100, 200);
// 	// Tuple col = Tuple::color(1, 0.2, 0.2);
// 	// c.write_pixel(50, 50, col);

// 	EXPECT_EQ(c.width, 100);
// 	EXPECT_EQ(c.height, 200); 
// }

/* MATRICES */
/* MATRICES */
/* MATRICES */

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
}
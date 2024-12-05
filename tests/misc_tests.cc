#include <gtest/gtest.h>
#include "tuple.h"
#include "canvas.h"

using namespace std;

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
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


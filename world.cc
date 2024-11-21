#include "world.h"
#include "intersection.h"
#include "colors.h"
#include "console.h"
#include <cmath>
#include <iostream>

World::World() {};

Camera::Camera(int horizontal_size, int vertical_size, float fov, Matrix4f t)
	: hsize(horizontal_size)
	, vsize(vertical_size)
	, field_of_view(fov)
	, transform(t)
{
	compute_canvas_info();
}

void Camera::compute_canvas_info() {
	float half_view = tan(field_of_view / 2.0f);
	float aspect = static_cast<float>(hsize) / static_cast<float>(vsize);

	if (aspect >= 1) {
		half_width = half_view;
		half_height = half_view / aspect;
	} else {
		half_width = half_view * aspect;
		half_height = half_view;
	}

	pixel_size = (half_width * 2) / hsize;
}

Matrix4f view_transform(Point3D from, Point3D to, Vector3D up) { // pg 99-100
	Vector3f forward = (to.cords.head<3>() - from.cords.head<3>()).normalized();
	Vector3f up_normalized = up.cords.head<3>().normalized();
	Vector3f left = forward.cross(up_normalized).normalized();
	Vector3f true_up = left.cross(forward);

	Matrix4f orientation;
	orientation  << left.x(),       left.y(),       left.z(),       0,
					true_up.x(),    true_up.y(),    true_up.z(),    0,
					-forward.x(),   -forward.y(),   -forward.z(),   0,
					0,              0,              0,              1;

	// Scoot the scene away
	Affine3f transform(Translation3f(-from.x(), -from.y(), -from.z()));
	Matrix4f translation_matrix = transform.matrix();

	Matrix4f final_view = orientation * translation_matrix;

	return final_view;
}

Ray ray_for_pixel(Camera camera, float px, float py) {
	// offset of the edge of the canvas to this particular pixels center
	float xoffset = (px + 0.5) * camera.pixel_size;
	float yoffset = (py + 0.5) * camera.pixel_size;

	//the untransformed cords of the pixel in worldspace
	float world_x = camera.half_width - xoffset;
	float world_y = camera.half_height - yoffset;

	// adjust the canvas point and origin by the cameras transformation. i.e. if we rotated the camera then we need to "rotate" our everything else.
	Vector4f pixel = camera.transform.inverse() * (Point3D(world_x, world_y, -1.0).cords);
	Vector4f origin = camera.transform.inverse() * (Point3D(0, 0, 0).cords);
	Vector4f direction = (pixel - origin).normalized();

	return Ray(Point3D(origin), Vector3D(direction));

}

Canvas render(Camera camera, World world) {
	Tuple CANVAS_COLOR = colors::Black;

	Canvas canvas(camera.hsize, camera.vsize, CANVAS_COLOR);
	for (int y = 0; y < camera.vsize - 1; y++) {
		for (int x = 0; x < camera.hsize - 1; x++) {
			Ray r = ray_for_pixel(camera, x, y);
			Tuple color = color_at(world, r);
			canvas.write_pixel(x, y, color, 1);
		}
	}

	return canvas;
}
#ifndef WORLD_H
#define WORLD_H

#include "sphere.h"
#include "light.h"
#include "canvas.h"
#include <vector>
#include <cmath>

#define _USE_MATH_DEFINES
#include <math.h>


class World {
//Light l = Light(Point3D(-10, 10, -10), colors::White)
	public: 
		Light light;
		std::vector<Sphere> objects;

		// Constructor
		World();
		// Destructor
		~World() = default;
};

class Camera {
	public: 
		int hsize;
		int vsize;
		float field_of_view;
		float half_width;
		float half_height;
		float pixel_size;
		Matrix4f transform;

		// Constructor
		Camera(int horizontal_size = 160,
				int vertical_size = 120,
				float fov = M_PI / 2.0f,
				Matrix4f t = Eigen::Matrix4f::Identity());
		// Destructor
		~Camera() = default;

	private:
		// Helper method to compute derived values
		void compute_canvas_info();
};

Matrix4f view_transform(Point3D from, Point3D to, Vector3D up);

Ray ray_for_pixel(Camera camera, float px, float py);

Canvas render(Camera camera, World world);

#endif // WORLD_H
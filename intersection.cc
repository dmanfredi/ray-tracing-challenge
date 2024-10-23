#include "intersection.h"
#include <cmath>
#include <limits>

using namespace std;

int Intersection::next_id = 0;


Intersection::Intersection(float t, const Sphere& o) : id(next_id++), t(t), object(o) {}

int Intersection::get_id() const {
	return id;
}

int Intersection::get_next_id() {
	return next_id;
}

optional<Intersection> hit(const vector<Intersection>& intersections) {
    optional<Intersection> result;
    float min_t = numeric_limits<float>::max();

    for (const auto& intersection : intersections) {
        if (intersection.t >= 0 && intersection.t < min_t) {
            min_t = intersection.t;
            result = intersection;
        }
    }

    return result;
}


vector<Intersection> intersect(const Sphere& sphere, const Ray& ray) {

	Ray transformed_ray = transform_ray(ray, sphere.transform.inverse());

	Vector4f sphere_to_ray = transformed_ray.origin.cords - sphere.origin.cords;
	float a = transformed_ray.direction.cords.dot(transformed_ray.direction.cords);
	float b = 2 * (transformed_ray.direction.cords.dot(sphere_to_ray));
	float c = sphere_to_ray.dot(sphere_to_ray) - 1;
	float discriminant = b * b - 4 * a * c; 

	if (discriminant < 0) {
		return {};
	}

	float t1 = (-b - sqrt(discriminant)) / (2 * a);
	float t2 = (-b + sqrt(discriminant)) / (2 * a);
	
	return {
		Intersection(min(t1, t2), sphere),
		Intersection(max(t1, t2), sphere)
	};
}


vector<Intersection> intersections(const initializer_list<vector<Intersection>>& intersection_lists) {
	vector<Intersection> result;
	for (const auto& list : intersection_lists) {
		for (const auto& intersection : list) {
			result.push_back(intersection);
		}
	}
	return result;
}
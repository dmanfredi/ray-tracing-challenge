#include "intersection.h"
#include "colors.h"
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

Computation::Computation(float t_in, const Sphere& object_in, const Point3D& point_in,
						const Vector3D& eyev_in, const Vector3D& normalv_in, const bool inside)
	: t(t_in)
	, object(object_in)
	, point(point_in)
	, eyev(eyev_in)
	, normalv(normalv_in)
	, inside(inside)
{}

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


vector<Intersection> flatten_intersections(const vector<vector<Intersection>>& intersection_lists) {
	vector<Intersection> result;
	for (const auto& list : intersection_lists) {
		for (const auto& intersection : list) {
			result.push_back(intersection);
		}
	}
	return result;
}

vector<Intersection> intersect_world(World& w, Ray& r) {
	vector<Sphere> objects_list = w.objects;
	vector<vector<Intersection>> all_intersections;

	for (const auto& object : objects_list) {
		vector<Intersection> intersects =  intersect(object, r);

		if (intersects.size() > 0) {
			all_intersections.push_back(intersects);
		}
	}

	// Flatten all_intersections into a single vector
	vector<Intersection> all_intersections_combined = flatten_intersections(all_intersections);

	// Sort intersections from closest to farthest
	sort(all_intersections_combined.begin(), all_intersections_combined.end(), [](const Intersection& a, const Intersection& b) { return a.t < b.t; });

	return all_intersections_combined;
}

// Precompute and store values for later
Computation prepare_compuation(Intersection& intersection, Ray& ray) {
	Point3D pos = position(ray, intersection.t);
	Vector3D normal = normal_at(intersection.object, pos);
	Vector3D eyev = Vector3D(-1 * ray.direction.cords);

	bool isInside;

	if (normal.cords.dot(eyev.cords) < 0) {
		isInside = true;
		normal = Vector3D(-1 * normal.cords);
	} else {
		isInside = false;
	}

	//Computation = Computation();
	Computation comps = Computation(
		intersection.t,
		intersection.object,
		pos,
		eyev,
		normal,
		isInside
	);

	return comps;
}

Tuple shade_hit(World& world, Computation& comps) {
	return lighting(
		comps.object.material,
		world.light,
		comps.point,
		comps.eyev,
		comps.normalv
	);
}

// Get the FINAL color that a ray gives us given the world it travels through.
Tuple color_at(World& world, Ray& ray) {
	vector<Intersection> world_intersections = intersect_world(world, ray);
	optional<Intersection> world_hit = hit(world_intersections); // Could be a hit, could be nothing.

	if (!world_hit) {
		return colors::Black;
	}

	Computation world_computations = prepare_compuation(world_hit.value(), ray);
	return shade_hit(world, world_computations); // return the final color
}

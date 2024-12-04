#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <optional>
#include <vector>
#include "utils.h"
#include "world.h"
#include "sphere.h"

using namespace std;

// The Intersection object itself, holds intersections and the sphere it intersected
class Intersection {
private:
	static int next_id;
	int id;

public: 
	float t;
	Sphere object;

	Intersection(float t, const Sphere& o);
	~Intersection() = default;

	int get_id() const;
	static int get_next_id();
};

class Computation {
	public: 
		float t;
		Sphere object;
		Point3D point;
		Point3D over_point;
		Vector3D eyev;
		Vector3D normalv;
		bool inside;

		Computation(float t_in, const Sphere& object_in, const Point3D& point_in, const Point3D& over_point_in,
					const Vector3D& eyev_in, const Vector3D& normalv_in, bool inside);
					
		~Computation() = default;
};

// Finds any hits from a list of Intersections
optional<Intersection> hit(const vector<Intersection>& intersections);

// Calculates intersections.
vector<Intersection> intersect(const Sphere& sphere, const Ray& ray);

// Flatens intersections into one list. Can take any number of these lists to do so.
vector<Intersection> flatten_intersections(const vector<vector<Intersection>>& intersection_lists);

// Calculates intersections for all objects in a world given a ray.
vector<Intersection> intersect_world(World& w, Ray& r);

Computation prepare_compuation(Intersection& intersection, Ray& ray);

Tuple shade_hit(World& world, Computation& comps);

bool is_shadowed(World& world, Point3D& point);

Tuple color_at(World& world, Ray& ray);


#endif // INTERSECTION_H
#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <optional>
#include <vector>
#include "utils.h"
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

// Finds any hits from a list of Intersections
optional<Intersection> hit(const vector<Intersection>& intersections);

// Calculates intersections.
vector<Intersection> intersect(const Sphere& sphere, const Ray& ray);

// Flatens intersections into one list. Can take any number of these lists to do so.
vector<Intersection> intersections(const initializer_list<vector<Intersection>>& intersection_lists);

#endif // INTERSECTION_H
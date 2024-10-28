#ifndef LIGHT_H
#define LIGHT_H

#include "utils.h"
#include "tuple.h"
#include "material.h"

class Light {

    public: 
        int radius;
        Point3D posistion;
        Tuple intensity;

        // Constructor with default values
        Light(Point3D p = Point3D(0,0,0), Tuple i = Tuple::color(1.0, 1.0, 1.0));

        // Destructor
        ~Light() = default;

};

Tuple lighting(Material material, Light light, Point3D position, Vector3D eyev, Vector3D normalv);

#endif // LIGHT_H
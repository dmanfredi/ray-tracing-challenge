#ifndef MATERIAL_H
#define MATERIAL_H

#include "utils.h"
#include "tuple.h"

class Material {

    public: 
        Tuple color;
        float ambient;
        float diffuse;
        float specular;
        float shininess;

        // Constructor with default values
        Material( 
            Tuple col = Tuple::color(1.0, 1.0, 1.0),
            float amb = 0.1,
            float diff = 0.9,
            float spec = 0.9,
            float shininess = 200.0
        );

        // Destructor
        ~Material() = default;
};

#endif // MATERIAL_H
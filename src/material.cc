#include "Material.h"
#include <cmath>
#include <iostream>

Material::Material(Tuple col, float amb, float diff, float spec, float shine) :
      color(col),     
      ambient(amb),     
      diffuse(diff),   
      specular(spec),
      shininess(shine) 
{}
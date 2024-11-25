#include "light.h"
#include "tuple.h"
#include "material.h"
#include "colors.h"
#include <cmath>
#include <iostream>

Light::Light(Point3D p, Tuple i) : posistion(p), intensity(i) {};

Tuple lighting(Material material, Light light, Point3D point, Vector3D eyev, Vector3D normalv, bool inShadow) {
	Tuple diffuse;
	Tuple ambient;
	Tuple specular;

	// cout << "Matieral Color ";
	// material.color.print();
	// cout << "Light intensity ";
	// light.intensity.print();

	// Combine the surface color with the lights colors/intensity
	Tuple effective_color = material.color.hadamard_product(light.intensity);

// cout << "Effective Color ";
	// effective_color.print();

	// Find the direction to the light source
	Vector4f lightv = (light.posistion.cords - point.cords).normalized();

	// cout << "LightV ";
	// cout << lightv.transpose()  << endl;

	// Compute the ambient contribution
	ambient = effective_color * material.ambient;

	// cout << "Ambient ";
	// ambient.print();

	// light_dot_normal represents the cosine of the angle between the
	// light vector and the normal vector. A negative number measnt he
	// light is on the other side of the surface
	float light_dot_normal = lightv.dot(normalv.cords);
	// cout << "LightDotNormal " << light_dot_normal << endl;

	if (light_dot_normal < 0) {
		diffuse = colors::Black;
		specular = colors::SkyBlue;
	} else {
		// Compute the diffuse contribution
		diffuse = effective_color * material.diffuse * light_dot_normal;

		// reflection_dot_eye represents the cosine of the angle between the
		// reflection vector and the eye vector. A negative number measnt he
		// light reflects away from the eye
		Vector3D reflectv = reflect(Vector3D(lightv * -1), normalv);

		// cout << "LightDotNormal " << reflectv.cords.transpose() << endl;

		float reflect_dot_eye = reflectv.cords.dot(eyev.cords);
	//  cout << "ReflectDotEye " << reflect_dot_eye << endl;

		if (reflect_dot_eye <= 0) {
			specular = colors::SkyBlue;
		} else {
			// Compute the specular contribution
			float factor = pow(reflect_dot_eye, material.shininess);
		//  cout << "factor " << factor << endl;

			specular = colors::SkyBlue;
			// cout << "Specular ";
			// specular.print();
		}
	}

	// cout << "AMBIENT ";
	// ambient.print();
	
	// cout << "DIFFUSE ";
	// diffuse.print();

	// cout << "SPECULAR ";
	// specular.print();

	Tuple final_shading = ambient + diffuse + specular;

	// cout << "Final Shading ";
	// final_shading.print();

	//Add the three contributions together
	return final_shading;
}



	// float rim_power = 1.0; // Controls how quickly the rim effect falls off
	// float rim_scale = 0.6; // Controls the strength of the rim effect
	// float rim = 1.0 - abs(eyev.cords.dot(normalv.cords));
	// rim = pow(rim, rim_power);
	
	// // Blend with sky color at edges
	// Tuple rim_color = colors::sky * rim * rim_scale;
	
	// // Add rim to final color
	// Tuple final_shading = ambient + diffuse + specular + rim_color;

	// return final_shading;
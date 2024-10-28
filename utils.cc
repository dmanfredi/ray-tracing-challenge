#include "utils.h"
#include "sphere.h"
#include "intersection.h"

Matrix4f create_scaling_matrix(float x, float y, float z)
{
	Matrix4f matrix;
	matrix  << x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1;

	return matrix;
}

Matrix4f reflection_matrix = create_scaling_matrix(-1, 1, 1);

Matrix4f create_X_rotation_matrix(float r)
{
	Matrix4f x_rotation = Matrix4f::Identity();
	x_rotation.block<3,3>(0,0) = AngleAxisf(r, Vector3f::UnitX()).matrix();
	return x_rotation;
}

Matrix4f create_Y_rotation_matrix(float r)
{
	Matrix4f y_rotation = Matrix4f::Identity();
	y_rotation.block<3,3>(0,0) = AngleAxisf(r, Vector3f::UnitY()).matrix();
	return y_rotation;
}

Matrix4f create_Z_rotation_matrix(float r)
{
	Matrix4f z_rotation = Matrix4f::Identity();
	z_rotation.block<3,3>(0,0) = AngleAxisf(r, Vector3f::UnitZ()).matrix();
	return z_rotation;
}

Matrix4f create_shearing_matrix(float xy, float xz, float yx, float yz, float zx, float zy)
{
	Matrix4f matrix;
	matrix  << 1, xy, xz, 0,
			yx, 1, yz, 0,
			zx, zy, 1, 0,
			0,  0,  0, 1;

	return matrix;
}

Point3D position(Ray ray, float time) {
	Vector4f result = ray.origin.cords + (ray.direction.cords * time);
	return Point3D(result[0], result[1], result[2]);
}

Ray transform_ray(Ray ray, const Matrix4f& matrix) {
	Vector4f point_cords = ray.origin.cords;
	Vector4f transformed_point_cords =  matrix * point_cords;

	Vector4f vector_cords = ray.direction.cords;
	Vector4f transformed_vector_cords = matrix * vector_cords;


	Ray translated_ray = {
		Point3D(transformed_point_cords),
		Vector3D(transformed_vector_cords)
	};

	return translated_ray;
}

Vector3D reflect(Vector3D in, Vector3D normal) {
	Vector4f in_dir = in.cords;
	Vector4f normal_dir = normal.cords;

	return Vector3D(
		in_dir - normal_dir * 2 * (in_dir.dot(normal_dir)) // Math magic
	);
}
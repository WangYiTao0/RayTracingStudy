//Chapter 1: Output an image

#include <iostream>
#include <fstream>

#include "Ray.h"

/*
	point(x ,y,z)
	center (cx,cy,cz)
	dot((p-c),(p-c)) = (x-Cx)* (x-Cx)+ (y-Cy)* (y-Cy)+ (z-Cz)* (z-Cz)
	dot((p(t)-c),(p(t)-c)) = R*R

	dot((A-C+t*B),(A-C+t*B)) = R*R

	t2*dot(B,B) + 2t*dot(B,A-C) + dot(A-C,A-C)-R2 = 0;
*/
// sphere normal Normal(P-C)
bool hit_sphere(const vec3& center, float radius, const ray& r)
{
	vec3 oc = r.Origin() - center;
	float a = dot(r.Direction(), r.Direction());
	float b = 2.0 * dot(oc, r.Direction());
	float c = dot(oc, oc) - radius * radius;

	//positive (meaning two real solutions), 
	//negative (meaning no real solutions), 
	//or zero (meaning one real solution)
	float discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}


vec3 color(const ray& r)
{
	if (hit_sphere(vec3(0, 0, -1), 0.5, r))
		return vec3(1, 0, 0);

	vec3 unit_Direction = unit_vector(r.Direction());
	float t = 0.5 * (unit_Direction.y() + 1.0);

	//blended value = (1-t)*startValue + t*endValue
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main()
{
	std::ofstream outputFile;
	outputFile.open("HelloWorld.ppm", std::ios::binary | std::ios::out);

	int nx = 200;
	int ny = 100;

	//using right hand coordinate

	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);

	//P 
	outputFile << "P3\n" << nx << " " << ny << "\n255\n";

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 col = color(r);

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			outputFile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outputFile.close();
	return 0;
}
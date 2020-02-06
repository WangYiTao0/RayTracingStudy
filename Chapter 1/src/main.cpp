//Chapter 1: Output an image

#include <iostream>
#include <fstream>
#include <cfloat>

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "Material.h"




vec3 color(const ray& r, hittable *world, int depth)
{
	hit_record rec;
	// ignore hits very near zero  gets rid of the shadow acne problem. 
	if (world->hit(r, 0.001, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.pMat->scatter(r, rec, attenuation, scattered)) 
		{
			return attenuation* color(scattered, world, depth + 1);
		}
		else
		{
		return	vec3(0,0,0);
		}
	}
	else
	{
		vec3 unit_Direction = unit_vector(r.Direction());
		float t = 0.5 * (unit_Direction.y() + 1.0);
		//blended value = (1-t)*startValue + t*endValue
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main()
{
	std::ofstream outputFile;
	outputFile.open("HelloWorld.ppm", std::ios::binary | std::ios::out);

	int nx = 200;
	int ny = 100;
	int ns = 100; //sampler 100 
	hittable* list[5];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
	hittable* world = new hittable_list(list, 5);
	//camera 
	camera cam(vec3(-2, 2, 1), vec3(0, 0, -1), vec3(0, 1, 0), 90, float(nx) / float(ny));
	//P 
	outputFile << "P3\n" << nx << " " << ny << "\n255\n";

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random_double()) / float(nx);
				float v = float(j + random_double()) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r, world,0);
			}
			col /= float(ns);

			//Add gamma
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			outputFile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outputFile.close();
	return 0;
}
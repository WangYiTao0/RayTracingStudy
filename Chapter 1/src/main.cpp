//Chapter 1: Output an image

#include <iostream>
#include <fstream>

#include <cfloat>

#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "random.h"

vec3 color(const ray& r, hittable *world)
{
	hit_record rec;
	if (world->hit(r, 0.0, FLT_MAX, rec))
	{
		return 0.5f * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);//in [0,1]
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

	hittable* list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hittable* world = new hittable_list(list, 2);
	//camera 
	camera cam;
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
				col += color(r, world);
			}
			col /= float(ns);


			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			outputFile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outputFile.close();
	return 0;
}
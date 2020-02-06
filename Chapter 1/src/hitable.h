#pragma once
#include "ray.h"

class material;

struct hit_record
{
	float t;
	vec3 p; //pos
	vec3 normal;
	material* pMat;
};

class hittable {

public:
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const = 0;
};
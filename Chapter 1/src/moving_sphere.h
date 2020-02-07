#pragma once

#include "hitable.h"

class moving_sphere : public hittable
{
public:
	moving_sphere() = default;
	moving_sphere(vec3 cen0,vec3 cen1,float t0,float t1,float r, material* m)
		:center0(cen0),center1(cen1),time0(t0),time1(t1),radius(r),pMat(m)
	{}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec)const override;
	//virtual bool bounding_box(float t0, float t1, aabb& box) const override;
	vec3 center(float time)const;
private:
	vec3 center0, center1;
	float time0, time1;
	float radius;
	material* pMat;
};

vec3 moving_sphere::center(float time)const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}
bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec)const
{
	vec3 oc = r.Origin() - center(r.time());
	float a = dot(r.Direction(), r.Direction());
	float b = dot(oc, r.Direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.pMat = pMat;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.pMat = pMat;
			return true;
		}
	}

	return false;
}
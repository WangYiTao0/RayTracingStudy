#pragma once

#include "hitable.h"

class sphere : public hittable
{
public:
	sphere() = default;
	sphere(vec3 cen, float r,material*m)
		:center(cen),radius(r),pMat(m){}
	// Inherited via hittable
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
	virtual bool bounding_box(float t0, float t1, aabb& box) const override;
private:
	vec3 center;
	float radius;
	material* pMat;
};


/*
	point(x ,y,z)
	center (cx,cy,cz)
	dot((p-c),(p-c)) = (x-Cx)* (x-Cx)+ (y-Cy)* (y-Cy)+ (z-Cz)* (z-Cz)
	dot((p(t)-c),(p(t)-c)) = R*R

	dot((A-C+t*B),(A-C+t*B)) = R*R

	t2*dot(B,B) + 2t*dot(B,A-C) + dot(A-C,A-C)-R2 = 0;
*/
// sphere normal Normal(P-C)
bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{

	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;

	//positive (meaning two real solutions), 
	//negative (meaning no real solutions), 
	//or zero (meaning one real solution)
	float discriminant = b * b - a * c;
	//calculate hit_record
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.pMat = pMat;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.pMat = pMat;
			return true;
		}
	}
	return false;
}

bool sphere::bounding_box(float t0, float t1, aabb& box) const
{
	box = aabb(center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
}
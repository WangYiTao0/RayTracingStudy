#pragma once

#include "hitable.h"

class sphere : public hittable
{
public:
	sphere() = default;
	sphere(vec3 cen, float r)
		:m_center(cen),m_radius(r){}
	// Inherited via hittable
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
private:
	vec3 m_center;
	float m_radius;
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

	vec3 oc = r.Origin() - m_center;
	float a = dot(r.Direction(), r.Direction());
	float b = dot(oc, r.Direction());
	float c = dot(oc, oc) - m_radius * m_radius;

	//positive (meaning two real solutions), 
	//negative (meaning no real solutions), 
	//or zero (meaning one real solution)
	float discriminant = b * b - a * c;

	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - m_center) / m_radius;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - m_center) / m_radius;
			return true;
		}
	}
	return false;
}
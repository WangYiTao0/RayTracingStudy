#pragma once

#include "ray.h"
#include "hitable.h"
#include "random.h"

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0 * vec3(random_double(), random_double(), random_double()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

// ref_idx is the ratio of the refractive index
// cosine is the incident ray and the incident normal
float schlick(float cosine, float ref_idx) {
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
	vec3 uv = unit_vector(v);//ray in 
	float dt = dot(uv, n);// rad
	// ni_over_nt  = sin��' / sin��  =  n1 / n2 = ��
	float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0)
	{
		//Ncos�� perspective
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant); // cos��'
		return true;
	}
	else
		return false;
}

vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

class material
{
public:
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, vec3& attenuation,
		ray& scattered
	) const = 0;
};

class lambertian :public material
{
public:
	lambertian(const vec3& a) :albedo(a) {}
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, vec3& attenuation,
		ray& scattered) const override {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p, r_in.time());
		attenuation = albedo;
		return true;
	}
private:
	vec3 albedo;
};

class metal :public material
{
public:
	//add some fuzz
	metal(const vec3& a, float f) :albedo(a) {
		if (f < 1)fuzz = f; else fuzz = 1;
	}
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, vec3& attenuation,
		ray& scattered) const override {
		vec3 reflected = reflect(unit_vector(r_in.Direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
		attenuation = albedo;
		return (dot(scattered.Direction(), rec.normal) > 0);
	}
private:
	vec3 albedo;
	float fuzz;
};

class dielectric : public material
{
public:
	dielectric(float ri) : ref_idx(ri) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation,
		ray& scattered) const override {
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.Direction(), rec.normal);
		float ni_over_nt;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;

		float reflect_prob;
		float cosine;

		if (dot(r_in.Direction(), rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.Direction(), rec.normal)
				/ r_in.Direction().length();
		}
		else
		{
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.Direction(), rec.normal)
				/ r_in.Direction().length();
		}

		if (refract(r_in.Direction(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = schlick(cosine, ref_idx);
		}
		else
		{
			reflect_prob = 1.0;
		}

		if (random_double() < reflect_prob) {
			scattered = ray(rec.p, reflected,r_in.time());
		}
		else {
			scattered = ray(rec.p, refracted, r_in.time());
		}

		return true;
	}


private:
	float ref_idx;
};
#pragma once
#include "hitable.h"

class hittable_list : public hittable
{
public:
	hittable_list() = default;
	hittable_list(hittable** l, int n)
		:m_pplist(l),list_size(n){}
	// Inherited via hittable
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

private:
	hittable** m_pplist;
	int list_size;
};

bool hittable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (int i = 0; i < list_size; i++) {
		if (m_pplist[i]->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}
#pragma once
#include"Hitable.h"

class HitableList :public Hitable
{
public:
	HitableList(){}
	HitableList(Hitable **l,int n)
		:list(l),list_size(n)
	{}
	virtual bool hit(const Ray& r, float t_min,float t_max, hit_record& rec)const;
private:
	Hitable** list;
	int list_size;

};

bool HitableList::hit(const Ray& r, float t_min, float t_max, hit_record& rec)const
{
	hit_record tmp;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (int i = 0; i < list_size; i++)
	{
		if (list[i]->hit(r, t_min,closest_so_far,tmp))
		{
			hit_anything = true;
			closest_so_far = tmp.t;
			rec = tmp;
		}
	}
	return hit_anything;
 }
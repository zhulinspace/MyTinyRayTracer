#pragma once
#include"Hitable.h"
#include"utils\utility.h"
#include<vector>


class HitableList :public Hitable
{
public:
	HitableList(){}
	
	HitableList(shared_ptr<Hitable>object) { add(object); }
	void clear() { objects.clear(); }
	void add(shared_ptr<Hitable>object) { objects.push_back(object); }
	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec)const;
	virtual bool bounding_box(aabb& output_box)const;


	std::vector<shared_ptr<Hitable>>objects;

};

bool HitableList::hit(const Ray& r, double t_min, double t_max, hit_record& rec)const
{
	hit_record tmp;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (const auto& object:objects)
	{
		if (object->hit(r, t_min,closest_so_far,tmp))
		{
			hit_anything = true;
			closest_so_far = tmp.t;
			rec = tmp;
		}
	}
	return hit_anything;
 }

inline bool HitableList::bounding_box(aabb& output_box) const
{
	if (objects.empty())return false;

	aabb temp_box;
	bool first_box = true;

	
	for (const auto& object:objects)
	{
		if (!object->bounding_box(temp_box)) return false;
		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
		first_box = false;
		
	}
	return true;
}

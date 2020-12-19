#pragma once
#include"HitableList.h"


class bvh_node :public Hitable
{
public:
	bvh_node();
	
	bvh_node(const HitableList& list)
		:bvh_node(list.objects,0, list.objects.size())
	{}
	
	bvh_node(const std::vector<shared_ptr<Hitable>>& src_objects, size_t start, size_t end);
	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec)const;
	virtual bool bounding_box(aabb& b)const;

	shared_ptr<Hitable> left;
	shared_ptr<Hitable> right;
	aabb box;
};

bool bvh_node::bounding_box(aabb& output_box)const
{
	output_box = box;
	return true;
}



bool bvh_node::hit(const Ray& r, double t_min, double t_max, hit_record& rec)const
{
	
	if (!box.hit(r, t_min, t_max))
		return false;

	bool hit_left = left->hit(r, t_min, t_max, rec);
	bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

	return hit_left || hit_right;

}
inline bool box_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b, int axis) {
	aabb box_a;
	aabb box_b;

	if (!a->bounding_box(box_a) || !b->bounding_box(box_b))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	return box_a.Min().e[axis] < box_b.Min().e[axis];
}


bool box_x_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b) {
	return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b) {
	return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b) {
	return box_compare(a, b, 2);
}

bvh_node::bvh_node(const std::vector<shared_ptr<Hitable>>& src_objects, size_t start, size_t end)
{
	//选择最长的那个轴,如何选择最长的那个轴
	//如果是第一次则应该根据场景看的宽度和高度进行判定，
	//按最长的那个轴进行排序，排序后分成两个子节点进行构建
	//我得知道当前节点的bbox

	auto objects = src_objects;//create a modifiable array of the src scene objects

	int axis = random_int(0,2);

	auto comparator = (axis == 0) ? box_x_compare: (axis == 1) ? box_y_compare : box_z_compare;

	size_t n = end - start;

	if (n == 1)
		left = right = objects[start];
	else if (n == 2)
	{
		if (comparator(objects[start], objects[start + 1]))
		{
			left = objects[start];
			right = objects[start + 1];
		}
		else
		{
			left = objects[start+1];
			right = objects[start ];
		}
	}
	else
	{
		std::sort(objects.begin()+start, objects.begin()+end, comparator);

		auto mid = start + n / 2;
		left = make_shared<bvh_node>(objects, start, mid);
		right = make_shared<bvh_node>(objects, mid, end);

	}
	aabb box_left, box_right;
	if (!left->bounding_box(box_left) || !right->bounding_box(box_right))
		std::cerr << "No bounding box in bvh_node constructor.\n";
	

	box = surrounding_box(box_left, box_right);
}


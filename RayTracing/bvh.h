#pragma once
#include"Hitable.h"
#include<random>
//std::random_device rd;
std::mt19937 rng(rd());
std::uniform_real_distribution<double>nd(0, 1);
class bvh_node :public Hitable
{
public:
	bvh_node() {}
	bvh_node(Hitable** l, int n, float time0, float time1);
	
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& b)const;

	Hitable* left;
	Hitable* right;
	aabb box;
};

bool bvh_node::bounding_box(float t0, float t1, aabb& b)const
{
	b = box;
	return true;
}



bool bvh_node::hit(const Ray& r, float t_min, float t_max, hit_record& rec)const
{
	if (box.hit(r, t_min, t_max))
	{
		hit_record left_rec, right_rec;
		bool hit_left = left->hit(r, t_min, t_max, left_rec);
		bool hit_right = right->hit(r, t_min, t_max, right_rec);
		if (hit_left && hit_right)
		{
			if (left_rec.t < right_rec.t)
				rec = left_rec;
			else
				rec = right_rec;
			return true;
		}
		else if (hit_left)
		{
			rec = left_rec;
			return true;
		}
		else if (hit_right)
		{
			rec = right_rec;
			return true;
		}
	}
	else
		return false;

}
int box_x_compare(const void* a, const void* b)
{
	aabb box_left, box_right;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if(!ah->bounding_box(0,0,box_left)||!bh->bounding_box(0,0,box_right))
		printf("no bouding box in bvh_node constructer\n");
	if ( box_left.min().x - box_right.min().x< 0.0)
		return -1;
	else
		return 1;
}
int box_y_compare(const void* a, const void* b)
{
	aabb box_left, box_right;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		printf("no bouding box in bvh_node constructer\n");
	if (box_left.min().y - box_right.min().y < 0.0)
		return -1;
	else
		return 1;
}
int box_z_compare(const void* a, const void* b)
{
	aabb box_left, box_right;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right))
		printf("no bouding box in bvh_node constructer\n");
	if (box_left.min().z - box_right.min().z < 0.0)
		return -1;
	else
		return 1;
}
bvh_node::bvh_node(Hitable** l, int n, float time0, float time1)
{
	//1.选择最长的那个轴,如何选择最长的那个轴


	//如果是第一次则应该根据场景看的宽度和高度进行判定，

	//按最长的那个轴进行排序，排序后分成两个子节点进行构建

	//我得知道当前节点的bbox


	int axis = int(3 * nd(rng));
	//drand48产生什么样的随机数
	if (axis == 0)
		qsort(l, n, sizeof(Hitable*), box_x_compare);
	else if(axis==1)
		qsort(l, n, sizeof(Hitable*), box_y_compare);
	else
		qsort(l, n, sizeof(Hitable*), box_z_compare);

	if (n == 1)
		left = right = l[0];
	else if (n == 2)
	{
		left = l[0];
		right = l[1];
	}
	else
	{
		left = new bvh_node(l, n / 2, time0, time1);
		right = new bvh_node(l+n/2, n-n / 2, time0, time1);
	}
	aabb box_left, box_right;
	if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
		printf( "no bouding box in bvh_node constructer\n");
	//如何用异常来捕获这个输出，或者用messagebox来进行输出
	

	box = surrounding_box(box_left, box_right);
}


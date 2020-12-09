#pragma once
#include "Ray.h"
#include"BoundingBox.h"
class material;

struct hit_record
{
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	material* mat_ptr;
};

class Hitable
{
public:
	virtual bool hit(const Ray& r, float t_min,float t_max, hit_record& rec)const = 0;
	virtual bool bounding_box(float t0, float t1, aabb& box)const = 0;
};
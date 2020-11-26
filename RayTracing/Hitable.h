#pragma once
#include "Ray.h"

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
};
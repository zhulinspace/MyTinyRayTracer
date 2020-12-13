#pragma once

#include "BoundingBox.h"


class material;

struct hit_record
{
	double t;
	vec3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;
	bool front_face;

	inline void set_face_normal(const Ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hitable
{
public:
	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec)const = 0;
	//virtual bool bounding_box(float t0, float t1, aabb& box);
	virtual bool bounding_box(aabb& outputbox)const = 0;
};
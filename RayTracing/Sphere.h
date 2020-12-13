#pragma once
#include"Hitable.h"
#include"Material.h"
class Sphere :public Hitable
{
public:
	Sphere(){}
	Sphere(vec3 cen, float r, shared_ptr<material> mat)
		:center(cen),radius(r), mat_ptr(mat)
	{}
	
	virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec)const;
	//virtual bool bounding_box(float t0, float t1, aabb& box)const;
	virtual bool bounding_box(aabb& box)const;
	vec3 GetCenter()const { return center; }
	float GetRadius()const { return radius; }
private:
	vec3 center;
	float radius;
	shared_ptr<material> mat_ptr;
};

bool Sphere::hit(const Ray& r, double t_min, double t_max, hit_record& rec)const
{
	vec3 oc = r.origin() - center;
	auto a = r.direction().length_squared();
	auto half_b =  dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b -   a * c;

	if (discriminant < 0)return false;

	auto sqrtd = sqrt(discriminant);
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root)
	{
		root = (-half_b + sqrtd) / a;
		if (root<t_min || t_max<root)
			return false;
	}
	rec.t = root;
	rec.p = r.point_at_parameter(rec.t);
	vec3 outward_normal = (rec.p - center) / radius;
	rec.set_face_normal(r, outward_normal);
	
	rec.mat_ptr = mat_ptr;
	return true;
}

inline bool Sphere::bounding_box(aabb& box) const
{
	box = aabb(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
	return true;
}

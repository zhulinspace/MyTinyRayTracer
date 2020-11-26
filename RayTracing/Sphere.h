#pragma once
#include"Hitable.h"
#include"Material.h"
class Sphere :public Hitable
{
public:
	Sphere(){}
	Sphere(glm::vec3 cen, float r, material *mat)
		:center(cen),radius(r),m(mat)
	{}
	~Sphere()
	{
		delete m;
	}
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec)const;
	glm::vec3 GetCenter()const { return center; }
	float GetRadius()const { return radius; }
private:
	glm::vec3 center;
	float radius;
	material* m;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec)const
{
	glm::vec3 oc = r.origin() - center;
	float a = glm::dot(r.direction(), r.direction());
	float b = 2.0 * glm::dot(oc, r.direction());
	float c = glm::dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(discriminant)) / (2.0 * a);
		if (temp<t_max && temp>t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			//p is hit point!
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = m;
			return true;

		}
		temp = (-b + sqrt(discriminant)) / (2.0 * a);
		if (temp<t_max && temp>t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(temp);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = m;
			return true;

		}
	}
	return false;
}
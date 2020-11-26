#pragma once
#include"Ray.h"

#include<random>
std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float>dist(0.0, 1.0);

glm::vec3 random_in_unit_sphere()
{
	glm::vec3 p;
	do {
		p = 2.0f * glm::vec3(dist(mt), dist(mt), dist(mt)) - glm::vec3(1, 1, 1);

	} while (p.x * p.x + p.y * p.y + p.z * p.z >= 1.0);
	return p;
}
class material
{
public:
	virtual bool scatter(const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered)const = 0;
};

class lambertian :public material
{
public:
	lambertian(const glm::vec3&a):albedo(a){}
	virtual bool scatter(const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered)const
	{
		glm::vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = Ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}

private:
	glm::vec3 albedo;
};

glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n)
{
	return v - 2.0f * glm::dot(v, n) * n;
}
class metal :public material
{
public:
	metal(const glm::vec3& a, float f) :albedo(a) { if (f < 1)fuzz = f; else fuzz = 1; }
	virtual bool scatter(const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered)const
	{
		glm::vec3 reflected = reflect(glm::normalize(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected+fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (glm::dot(scattered.direction(), rec.normal) > 0);
	}
private:
	glm::vec3 albedo;
	float fuzz;
};
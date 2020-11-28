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

bool refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted)
{
	glm::vec3 uv = glm::normalize(v);
	float dt = glm::dot(uv, n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	
	return false;
}
float schlick(float cosine, float ref_idx)
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * powf((1 - cosine), 5);

}
class dielectric :public material
{
public:
	dielectric(float ri) :ref_idx(ri){}
	virtual bool scatter(const Ray& r_in, const hit_record& rec, glm::vec3& attenuation, Ray& scattered)const
	{
		glm::vec3 outward_normal;
		glm::vec3 reflectd = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = glm::vec3(1.0, 1.0,1.0);
		glm::vec3 refracted;
		float reflect_prob;
		float cosine;

		if (glm::dot(r_in.direction(), rec.normal) > 0)
		{
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * glm::dot(r_in.direction(), rec.normal) / r_in.direction().length();

		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0f / ref_idx;
			cosine = -glm::dot(r_in.direction(), rec.normal) / r_in.direction().length();

		}

		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			reflect_prob = schlick(cosine, ref_idx);
			
		}
		else
		{
			reflect_prob = 1.0f;
		}

		if (dist(mt) < reflect_prob)
		{
			scattered = Ray(rec.p, reflectd);
		}
		else
		{
			scattered = Ray(rec.p, refracted);
		}
		
		return true;
	}
	float ref_idx;
};


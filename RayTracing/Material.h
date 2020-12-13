#pragma once
#include"Ray.h"



inline vec3 random_in_unit_sphere()
{
	
	while (true)
	{
		auto p = vec3::random(-1, 1);
		if (p.length_squared() >= 1)continue;
		return p;
	}
}
inline vec3 random_unit_vector()
{
	return unit_vector(random_in_unit_sphere());
}

class material
{
public:
	virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered)const = 0;
};

class lambertian :public material
{
public:
	lambertian(const vec3&a):albedo(a){}
	virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered)const
	{
		auto scatter_direction = rec.normal + random_unit_vector();

		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = Ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

private:
	vec3 albedo;
};

vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2.0f * dot(v, n) * n;
}
class metal :public material
{
public:
	metal(const vec3& a, float f) :albedo(a) ,fuzz(f<1?f:1){ }
	virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered)const
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected+fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
private:
	vec3 albedo;
	double fuzz;
};

vec3 refract(const vec3& v, const vec3& n, double ni_over_nt)
{
	auto cos_theta = fmin(dot(-v, n), 1.0);
	vec3 r_out_perp = ni_over_nt * (v + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
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
	dielectric(double ri) :ref_idx(ri){}
	virtual bool scatter(const Ray& r_in, const hit_record& rec, vec3& attenuation, Ray& scattered)const
	{
		attenuation = vec3(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / ref_idx) : ref_idx;

		vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 direction;
		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, refraction_ratio);

		scattered = Ray(rec.p, direction);
		return true;
	}
private:
	static double reflectance(double cosine, double ref_idx) {
		// Use Schlick's approximation for reflectance.
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
	double ref_idx;
};


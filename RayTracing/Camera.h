#pragma once
#include"utils\utility.h"

class Camera
{
public:
	Camera(vec3 lookfrom,vec3 lookat,vec3 vup,double vfov,double aspect)
	{	
		vec3 u,v, w;
		auto theta = degrees_to_radians(vfov);
		auto half_height = tan(theta / 2);
		auto viewport_height = 2.0 * half_height;
		auto viewport_width = aspect * viewport_height;

		
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		origin = lookfrom;
		horizontal = viewport_width * u;
		vertical = viewport_height * v;
		
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
	
		
		//这里的成像平面定义如果定义为z=-1
		//
		
	}
	Ray GetRay(double u, double v)
	{
		return Ray(origin, lower_left_corner+ u*horizontal+v*vertical-origin);
	}

	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
};
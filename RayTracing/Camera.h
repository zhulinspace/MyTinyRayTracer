#pragma once
#include"Ray.h"
#define M_PI 3.14159265358979323846
class Camera
{
public:
	Camera(glm::vec3 lookfrom,glm::vec3 lookat,glm::vec3 vup,float vfov,float aspect)
	{	
		glm::vec3 u,v, w;
		float theta = vfov * M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = glm::normalize(lookfrom - lookat);
		u = glm::normalize(glm::cross(vup, w));
		v = glm::cross(w, u);
		lower_left_corner = origin - half_width * u - half_height * v - w;
		horizontal = half_width * u;
		vertical = half_height * v;
		/*lower_left_corner=glm::vec3 (-half_width, -half_height, -1.0);
		horizontal=glm::vec3 (2*half_width, 0.0, 0.0);
		vertical=glm::vec3 (0.0, 2*half_height, 0.0);*/
		
	}
	Ray GetRay(float u, float v)
	{
		return Ray(origin, lower_left_corner + u * horizontal + v * vertical-origin);
	}

	glm::vec3 lower_left_corner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 origin;
};
#pragma once
#include"Ray.h"

class Camera
{
public:
	Camera()
	{
		lower_left_corner=glm::vec3 (-2.0, -1.5, -1.0);
		horizontal=glm::vec3 (4.0, 0.0, 0.0);
		vertical=glm::vec3 (0.0, 3.0, 0.0);
		origin=glm::vec3 (0.0, 0.0, 0.0);
	}
	Ray GetRay(float u, float v)
	{
		return Ray(origin, lower_left_corner + u * horizontal + (1 - v) * vertical-origin);
	}

	glm::vec3 lower_left_corner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 origin;
};
#pragma once
#include"Ray.h"
#include"Hitable.h"
inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class aabb
{
public:
	aabb(){}
	aabb(const glm::vec3& a, const glm::vec3& b) { _min = a; _max = b; }
	glm::vec3 min() const { return _min; }
	glm::vec3 max() const { return _max; }

	bool hit(const Ray& r, float tmin, float tmax)const
	{
		for (int i = 0; i < 3; i++)
		{
			float t0 = ffmin((_min[i] - r.origin()[i]) / r.direction()[i],
				(_max[i] - r.origin()[i]) / r.direction()[i]);
			float t1 = ffmax((_min[i] - r.origin()[i]) / r.direction()[i],
				(_max[i] - r.origin()[i]) / r.direction()[i]);
			tmin = ffmax(t0, tmin);
			tmax = ffmin(t1, tmax);
			if (tmax <= tmin)
				return false;
		}
		return true;
	}


	glm::vec3 _min;
	glm::vec3 _max;
};
aabb surrounding_box(aabb box0, aabb box1)
{
	glm::vec3 samll(fmin(box0.min().x,box1.min().x),
		fmin(box0.min().y, box1.min().y),
		fmin(box0.min().z, box1.min().z));

	glm::vec3 big(fmax(box0.max().x, box1.max().x),
		fmax(box0.max().y, box1.max().y),
		fmax(box0.max().z, box1.max().z));
	return aabb(samll, big);


}

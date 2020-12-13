#pragma once

#include"Ray.h"



class aabb
{
public:
	vec3 _min;
	vec3 _max;
	aabb(){}
	aabb(const vec3& a, const vec3& b) { _min = a; _max = b; }

	vec3 Min() const { return _min; }

	vec3 Max() const { return _max; }


	inline bool hit(const Ray& r, double tmin, double tmax)const
	{
		for (int i = 0; i < 3; i++)
		{
			
			/*float t0 = ffmin((_min[i] - r.origin()[i]) / r.direction()[i],
				(_max[i] - r.origin()[i]) / r.direction()[i]);
			float t1 = ffmax((_min[i] - r.origin()[i]) / r.direction()[i],
				(_max[i] - r.origin()[i]) / r.direction()[i]);
			tmin = ffmax(t0, tmin);
			tmax = ffmin(t1, tmax);*/

			auto invD = 1.0f / r.direction()[i];
			auto t0 = (_min[i] - r.origin()[i]) * invD;
			auto t1 = (_max[i] - r.origin()[i]) * invD;
			if (invD < 0.0f)
				std::swap(t0, t1);
			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;
			if (tmax <= tmin)
				return false;
		}
		return true;
	}

};
inline aabb surrounding_box(aabb box0, aabb box1)
{
	vec3 samll(fmin(box0.Min().x(),box1.Min().x()),
		fmin(box0.Min().y(), box1.Min().y()),
		fmin(box0.Min().z(), box1.Min().z()));

	vec3 big(fmax(box0.Max().x(), box1.Max().x()),
		fmax(box0.Max().y(), box1.Max().y()),
		fmax(box0.Max().z(), box1.Max().z()));
	return aabb(samll, big);


}

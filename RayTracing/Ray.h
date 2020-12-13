#pragma once
#include"utils\vec3.h"


class Ray
{
public:
	Ray(){}
	Ray(const vec3&a,const vec3& b)
	:ori(a),dir(b)
	{}
	inline vec3 origin()const { return ori; }
	inline vec3 direction()const { return dir; }
	inline vec3 point_at_parameter(double t)const { return ori + dir*t; }
private:
	vec3 ori;
	vec3 dir;
};
#pragma once
#include"utils\vec3.h"
#include"utils\utility.h"
class texture
{
public:
	virtual vec3 value(double u, double v, const vec3& p)const = 0;
};

class constant_texture :public texture
{
public:
	constant_texture(){}
	constant_texture(vec3 c) :color(c) {}
	constant_texture(double red,double green,double blue):constant_texture(vec3(red,green,red)){}
	virtual vec3 value(double u, double v, const vec3& p)const
	{
		return color;
	}
	vec3 color;
};


class checker_texture :public texture
{
public:
	checker_texture() {}

	checker_texture(shared_ptr<texture>_even,shared_ptr<texture>_odd):even(_even),odd(_odd)
	{}

	checker_texture(vec3 c1, vec3 c2) :even(make_shared<constant_texture>(c1)), odd(make_shared<constant_texture>(c2)) {}

	virtual vec3 value(double u, double v, const vec3& p)const override
	{
		auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}
public:
	shared_ptr<texture>odd;
	shared_ptr<texture>even;
};
#pragma once
#include<math.h>
#include<stdlib.h>
#include<iostream>

template<typename T>
class vec3 {
public:
	vec3(){}
	vec3(T e0, T e1, T e2)
		:e[0](e0),e[1](e1),e[2](e2)
	{}

	inline T x() const { return e[0]; }
	inline T y()const { return e[1]; }
	inline T z()const { return e[2]; }
	inline const vec3& operator+() const { return *this; }
	T e[3];
};
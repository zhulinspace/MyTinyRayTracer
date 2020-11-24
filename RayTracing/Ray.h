#pragma once
#include"vendor\glm\glm.hpp"
class Ray
{
public:
	Ray(){}
	Ray(const glm::vec3&a,const glm::vec3& b)
	:ori(a),dir(b)
	{}
	inline glm::vec3 origin()const { return ori; }
	inline glm::vec3 direction()const { return dir; }
	inline glm::vec3 point_at_parameter(float t)const { return ori + t * dir; }
private:
	glm::vec3 ori;
	glm::vec3 dir;
};
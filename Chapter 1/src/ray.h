#pragma once

#include "vec3.h"
// P(t) = origin + t * direction(normalize) 
class ray
{
public:
	ray() = default;
	ray(const vec3& Origin,const vec3& Direction,float t = 0.0)
	:m_Origin(Origin),m_Direction(Direction),m_time(0.0){}
	vec3 Origin()const { return m_Origin; }
	vec3 Direction() const { return m_Direction; }
	float time()const { return m_time; }
	vec3 point_at_parameter(float t) const { return m_Origin + t * m_Direction; }
private:
	vec3 m_Origin;
	vec3 m_Direction;
	float m_time;
};
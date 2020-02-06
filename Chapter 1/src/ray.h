#pragma once

#include "vec3.h"
// P(t) = origin + t * direction(normalize) 
class ray
{
public:
	ray() = default;
	ray(const vec3& Origin,const vec3& Direction)
	:m_Origin(Origin),m_Direction(Direction){}
	vec3 Origin()const { return m_Origin; }
	vec3 Direction() const { return m_Direction; }
	vec3 point_at_parameter(float t) const { return m_Origin + t * m_Direction; }
private:
	vec3 m_Origin;
	vec3 m_Direction;
};
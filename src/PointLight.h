#pragma once
#include "ALight.h"

class PointLight : public ALight
{
public:
	PointLight(String name, Vector3 color, float intensity, float range);
	~PointLight() = default;

	float GetRange() const { return m_range; }

private:
	float m_range;

};


#pragma once
#include "AMeshObject.h"

class Cylinder :  public AMeshObject
{
public:
	Cylinder(String name);
	~Cylinder() = default;

	void Update(float deltaTime) override;

private:
	float m_radius = 0.5f;
	float m_height = 2.0f;
	int m_segments = 20;
};
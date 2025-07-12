#pragma once
#include "AMeshObject.h"

class Sphere : public AMeshObject
{
public:
    Sphere(String name);
    virtual ~Sphere() = default;

protected:
	void Update(float deltaTime) override;

private:
    int m_tessellation = 20;
    float m_radius = 0.5f;
};


#pragma once
#include "AMeshObject.h"

class Sphere : public AMeshObject
{
public:
    Sphere(String name);
    virtual ~Sphere() = default;

protected:
	void OnUpdate(float deltaTime) override;

private:
    static const uint32_t TESSELLATION = 16;
    inline static const float RADIUS = 0.5f;
};


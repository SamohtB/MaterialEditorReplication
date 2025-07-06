#pragma once
#include "AMeshObject.h"

class Plane : public AMeshObject
{
public:
	Plane(String name);
	~Plane() = default;

	// Inherited via AMeshObject
	void OnUpdate(float deltaTime) override;

private:
};


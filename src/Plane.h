#pragma once
#include "AMeshObject.h"

class Plane : public AMeshObject
{
public:
	Plane(String name);
	~Plane() = default;

	// Inherited via AMeshObject
	void Update(float deltaTime) override;

private:
};


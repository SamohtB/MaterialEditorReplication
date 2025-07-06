#pragma once
#include "AMeshObject.h"

class Cube : public AMeshObject
{
public:
	Cube(String name);
	~Cube() = default;

	// Inherited via AMeshObject
	void OnUpdate(float deltaTime) override;
private:
};


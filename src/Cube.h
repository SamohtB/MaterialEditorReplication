#pragma once
#include "AMeshObject.h"

class Cube : public AMeshObject
{
public:
	Cube(String name);
	~Cube() = default;

	// Inherited via AMeshObject
	void Update(float deltaTime) override;
private:
};


#pragma once
#include "AGameObject.h"
#include "LightTypes.h"

class ALight : public AGameObject
{
public:
	Vector3 GetColor() const { return m_color; }
	float GetIntensity() const { return m_intensity; }

	ALight(String name, Vector3 color, float intensity, String m_type);
	~ALight() = default;
	

private:
	String m_type;
	Vector3 m_color;
	float m_intensity;

	// Inherited via AGameObject
	void Update(float deltaTime) override;
	void Draw(DeviceContext* context, String shader) override;
};


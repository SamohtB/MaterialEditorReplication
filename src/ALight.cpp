#include "ALight.h"

ALight::ALight(String name, Vector3 color, float intensity, String type)
	: AGameObject(name), m_color(color), m_intensity(intensity), m_type(type)
{
}

void ALight::Update(float deltaTime)
{
}

void ALight::Draw(DeviceContext* context, String shader)
{
}

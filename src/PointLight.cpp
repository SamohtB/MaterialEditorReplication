#include "PointLight.h"

PointLight::PointLight(String name, Vector3 color, float intensity, float range)
	: ALight(name, color, intensity, LightTypes::POINT_LIGHT), m_range(range)
{

}

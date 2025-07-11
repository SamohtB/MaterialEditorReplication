#pragma once
#include <string>
class LightTypes
{
public:
	inline static const std::string POINT_LIGHT = "POINT_LIGHT";
	inline static const std::string DIRECTIONAL_LIGHT = "DIRECTIONAL_LIGHT";

	inline static constexpr int MAX_DIRECTIONAL_LIGHTS = 1;
	inline static constexpr int MAX_POINT_LIGHTS = 3;
};
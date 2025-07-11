#pragma once
#include "pch.h"
#include "Math.h"
#include "FrameConstants.h"
#include "DynamicConstantBufferPool.h"
#include "ALight.h"
#include "PointLight.h"

struct alignas(16) DirectionalLightData
{
    Vector3 direction;
    float padding;

    Vector3 color;
    float intensity;
};

struct alignas(16) PointLightData
{
    Vector3 position;
    float range;

    Vector3 color;
    float intensity;
};

inline static constexpr uint32_t MaxPointLights = 4;

struct alignas(16) LightConstants
{
    PointLightData pointLights[MaxPointLights];
    uint32_t pointLightCount;
};

class LightManager
{
public:
	using LightMap = std::unordered_map<String, ALight*>;

    LightManager(ID3D12Device* device);
    ~LightManager() = default;

	void CreateLight(const String& lightName, const Vector3& color, float intensity, float range);

	void BeginFrame(UINT currentFrameIndex);
    D3D12_GPU_VIRTUAL_ADDRESS GetLightBufferHandle(UINT currentFrameIndex) const;

private:
    std::unique_ptr<DynamicConstantBufferPool> m_lightBuffer;
    LightConstants m_lightConstants;
    LightMap m_lightMap;
};
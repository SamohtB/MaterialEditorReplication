#include "LightManager.h"
#include "GameObjectManager.h"

LightManager::LightManager(ID3D12Device* device)
{
    m_lightBuffer = std::make_unique<DynamicConstantBufferPool>(device, sizeof(LightConstants));
}

void LightManager::CreateLight(const String& lightName, const Vector3& color, float intensity, float range)
{
	Debug::Assert(m_lightConstants.pointLightCount < MaxPointLights, "Maximum number of point lights exceeded.");

	auto light = std::make_unique<PointLight>(lightName, color, intensity, range);

    uint32_t idx = m_lightConstants.pointLightCount++;
    m_lightConstants.pointLights[idx] = 
    {
        light->GetLocalPosition(),
        light->GetRange(),
        light->GetColor(),
        light->GetIntensity()
    };

    auto cpu = m_lightBuffer->GetMappedAddress();

    for (const auto& buffer : cpu)
    {
        memcpy(buffer, &m_lightConstants, sizeof(LightConstants));
    }

    this->m_lightMap[lightName] = light.get();
    GameObjectManager::GetInstance()->AddGameObject(std::move(light));
}

void LightManager::BeginFrame(UINT currentFrameIndex)
{
    m_lightBuffer->BeginFrame(currentFrameIndex);
}

D3D12_GPU_VIRTUAL_ADDRESS LightManager::GetLightBufferHandle(UINT currentFrameIndex) const
{
    return m_lightBuffer->GetCurrentBufferAddress(currentFrameIndex);
}

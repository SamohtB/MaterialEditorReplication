#include "LightManager.h"
#include "GameObjectManager.h"

LightManager::LightManager(ID3D12Device* device) : m_lightCount(0)
{
    m_lightBuffer = std::make_unique<DynamicConstantBufferPool>(device, sizeof(LightConstants));
}

void LightManager::CreateLight(const String& lightName, const Vector3& color, float intensity, float range)
{
    if (m_lightCount >= MaxPointLights)
    {
        Debug::LogWarning("Maximum number of point lights exceeded.");
        return;
    }
	
	auto light = std::make_unique<PointLight>(lightName, color, intensity, range);

    PointLightData data;
    data.position = light->GetLocalPosition();
    data.range = light->GetRange();
    data.color = light->GetColor();
    data.intensity = light->GetIntensity();

    m_pointLightData[m_lightCount] = data;
	m_lightCount++;

    this->m_lightMap[lightName] = light.get();
    GameObjectManager::GetInstance()->AddGameObject(std::move(light));
}

void LightManager::UploadLightConstants(UINT currentFrameIndex)
{
    m_lightBuffer->BeginFrame(currentFrameIndex);

    LightConstants constants = {};
    uint32_t i = 0;

    for (auto& [name, lightPtr] : m_lightMap)
    {
        if (i >= m_lightCount) break;

        PointLight* light = static_cast<PointLight*>(lightPtr);

        PointLightData data;
        data.position = light->GetLocalPosition();  
        data.range = light->GetRange();
        data.color = light->GetColor();
        data.intensity = light->GetIntensity();

        constants.pointLights[i] = data;
        i++;
    }

    constants.pointLightCount = i;

    auto cb = m_lightBuffer->Allocate(sizeof(LightConstants));
    memcpy(cb.cpuPtr, &constants, sizeof(LightConstants));
}

D3D12_GPU_VIRTUAL_ADDRESS LightManager::GetLightBuffer(UINT currentFrameIndex) const
{
    return m_lightBuffer->GetCurrentBufferAddress(currentFrameIndex);
}

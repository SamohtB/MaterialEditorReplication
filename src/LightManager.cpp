#include "LightManager.h"
#include "GameObjectManager.h"
#include "Debug.h"

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
	
	auto light = std::make_shared<PointLight>(lightName, color, intensity, range);

    PointLightData data;
    data.position = light->GetLocalPosition();
    data.range = light->GetRange();
    data.color = light->GetColor();
    data.intensity = light->GetIntensity();

    m_pointLightData[m_lightCount] = data;
	m_lightCount++;

    this->m_lightMap[lightName] = light;
    GameObjectManager::GetInstance()->AddGameObject(light, false);
}

void LightManager::UploadLightConstants(UINT currentFrameIndex)
{
    m_lightBuffer->BeginFrame(currentFrameIndex);

    LightConstants constants = {};
    uint32_t i = 0;

    for (auto it = m_lightMap.begin(); it != m_lightMap.end();)
    {
        /* Remove Deleted References */
        std::shared_ptr<ALight> baseLight = it->second.lock();
        if (!baseLight)
        {
            it = m_lightMap.erase(it);
            continue;
        }

        std::shared_ptr<PointLight> pointLight = std::dynamic_pointer_cast<PointLight>(baseLight);

        if (!pointLight)
        {
            it++;
            continue; 
        }

        if (i >= MaxPointLights) break;

        PointLightData data;
        data.position = pointLight->GetLocalPosition();
        data.range = pointLight->GetRange();
        data.color = pointLight->GetColor();
        data.intensity = pointLight->GetIntensity();

        constants.pointLights[i++] = data;
        it++;
    }

    constants.pointLightCount = i;

    auto cb = m_lightBuffer->Allocate(sizeof(LightConstants));
    memcpy(cb.cpuPtr, &constants, sizeof(LightConstants));
}

D3D12_GPU_VIRTUAL_ADDRESS LightManager::GetLightBuffer(UINT currentFrameIndex) const
{
    return m_lightBuffer->GetCurrentBufferAddress(currentFrameIndex);
}

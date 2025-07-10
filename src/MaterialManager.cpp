#include "MaterialManager.h"
#include "FrameConstants.h"
#include "GraphicsEngine.h"
#include "TextureManager.h"
#include "Debug.h"

MaterialManager::MaterialManager(ID3D12Device* device)
{
    this->m_materialBuffer = std::make_unique<DynamicConstantBufferPool>(device, POOL_SIZE_MEDIUM);
}

void MaterialManager::LoadInitialMaterials()
{
    CreateBlankMaterial(MaterialType::DEFAULT);

    /* MaterialDesc Sequence: Albedo, Normal, Metal, Rough, AO, Emmissive, Height, Tiling, Offset */
    CreateMaterial(MaterialType::ROCK,
        {
        TextureType::ROCK_COLOR,    Vector4(1, 1, 1, 1),
        TextureType::ROCK_NORMAL,   1.0f,
        TextureType::DEFAULT,       0.0f,
        TextureType::ROCK_ROUGH,    1.0f,
        TextureType::ROCK_AO,       1.0f,
        TextureType::DEFAULT,       0.0f,
        TextureType::ROCK_HEIGHT,   0.7f,
		Vector2(1.0f, 1.0f),
        Vector2(0.0f, 0.0f)
        });

    CreateMaterial(MaterialType::METAL_PLATE,
        {
        TextureType::METAL_COLOR,   Vector4(1, 1, 1, 1),
        TextureType::METAL_NORMAL,  1.0f,
        TextureType::METAL_METAL,   0.8f,
        TextureType::METAL_ROUGH,   1.0f,
        TextureType::DEFAULT,       0.0f,
        TextureType::DEFAULT,       0.0f,
        TextureType::DEFAULT,       0.0f,
        Vector2(1.0f, 1.0f),
        Vector2(0.0f, 0.0f)
        });

    CreateMaterial(MaterialType::BRICKS,
        {
        TextureType::BRICKS_COLOR,  Vector4(1, 1, 1, 1),
        TextureType::BRICKS_NORMAL, 1.0f,
        TextureType::DEFAULT,       0.0f,
        TextureType::BRICKS_ROUGH,  1.0f,
        TextureType::BRICKS_AO,     1.0f,
        TextureType::DEFAULT,       0.0f,
        TextureType::DEFAULT,       0.0f,
        Vector2(1.0f, 1.0f),
        Vector2(0.0f, 0.0f)
        });
}

void MaterialManager::CreateBlankMaterial(const String& materialName)
{
    CreateMaterial(materialName, {
        TextureType::DEFAULT, Vector4(1, 1, 1, 1),
        TextureType::DEFAULT, 0.0f,
        TextureType::DEFAULT, 0.0f,
        TextureType::DEFAULT, 0.0f,
        TextureType::DEFAULT, 0.0f,
        TextureType::DEFAULT, 0.0f,
        TextureType::DEFAULT, 0.0f,
        Vector2(1.0f, 1.0f),
        Vector2(0.0f, 0.0f)
        });
}

void MaterialManager::CreateMaterial(const String& materialName, MaterialDescription materialDesc)
{
    if (m_materialMap.count(materialName))
    {
        Debug::Log("Material '" + materialName + "' already exists.");
        return;
    }

    MaterialConstants constants = CreateMaterialConstants(materialDesc);
    std::vector<D3D12_GPU_VIRTUAL_ADDRESS> perFrameAddress(FRAME_COUNT);
    for (UINT i = 0; i < FRAME_COUNT; ++i)
    {
        m_materialBuffer->SetCurrentFrameIndex(i);
        auto cb = m_materialBuffer->Allocate(sizeof(MaterialConstants));
        memcpy(cb.cpuPtr, &constants, sizeof(MaterialConstants));
        perFrameAddress[i] = cb.gpuAddr;
    }
    m_materialMap[materialName] = std::make_unique<Material>(materialName, perFrameAddress, materialDesc);
    Debug::Log(materialName + " Material Created");
}

void MaterialManager::UpdateMaterial(const String& materialName, MaterialDescription materialDesc)
{
    if (!m_materialMap.count(materialName))
    {
        Debug::Log("Cannot update material '" + materialName + "': it does not exist.");
        return;
    }

    auto& material = m_materialMap[materialName];
    material->SetDescription(materialDesc);

    MaterialConstants constants = CreateMaterialConstants(materialDesc);
    for (UINT i = 0; i < FRAME_COUNT; ++i)
    {
        m_materialBuffer->SetCurrentFrameIndex(i);
        auto cb = m_materialBuffer->Allocate(sizeof(MaterialConstants));
        memcpy(cb.cpuPtr, &constants, sizeof(MaterialConstants));
        material->SetGPUAddress(i, cb.gpuAddr);
    }
}

void MaterialManager::BeginFrame(UINT currentFrameIndex)
{
    m_materialBuffer->BeginFrame(currentFrameIndex);
}

D3D12_GPU_VIRTUAL_ADDRESS MaterialManager::GetMaterialHandle(const String& materialName, UINT currentFrameIndex)
{
    return this->m_materialMap[materialName]->GetCBAddress(currentFrameIndex);
}

MaterialDescription MaterialManager::GetMaterialDescription(const String& materialName)
{
    return this->m_materialMap[materialName]->GetDescription();
}

MaterialConstants MaterialManager::CreateMaterialConstants(const MaterialDescription& desc)
{
    auto textureManager = GraphicsEngine::GetInstance()->GetTextureManager();
    MaterialConstants constants = {};

    constants.diffuseHandleIndex = textureManager->GetTextureSRVIndex(desc.albedoTex);
    constants.normalHandleIndex = textureManager->GetTextureSRVIndex(desc.normalTex);
    constants.metalHandleIndex = textureManager->GetTextureSRVIndex(desc.metalTex);
    constants.roughHandleIndex = textureManager->GetTextureSRVIndex(desc.roughTex);
    constants.aoHandleIndex = textureManager->GetTextureSRVIndex(desc.aoTex);
    constants.emmissiveHandleIndex = textureManager->GetTextureSRVIndex(desc.emissiveTex);
    constants.heightHandleIndex = textureManager->GetTextureSRVIndex(desc.heightTex);

    constants.baseColor = desc.albedoColor;

    constants.normalStr = desc.normalStrength;
    constants.metalStr = desc.metalStrength;
    constants.roughStr = desc.roughStrength;
    constants.aoStr = desc.aoStrength;
    constants.emmissiveStr = desc.emissiveStrength;
    constants.heightStr = desc.heightStrength;

    constants.tiling = desc.tiling;
    constants.offset = desc.offset;

    constants.materialFlags = 0;

    if (desc.normalTex != "default_normal") constants.materialFlags |= (1 << 0);
    if (desc.metalTex != "default_black")  constants.materialFlags |= (1 << 1);
    if (desc.roughTex != "default_white")  constants.materialFlags |= (1 << 2);
    if (desc.aoTex != "default_white")  constants.materialFlags |= (1 << 3);
    if (desc.emissiveTex != "default_black")  constants.materialFlags |= (1 << 4);
    if (desc.heightTex != "default_black")  constants.materialFlags |= (1 << 5);
    return constants;
}
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

    CreateMaterial(MaterialType::ROCK, 
        MaterialDescription{
        TextureType::ROCK_COLOR,    Vector4(1, 1, 1, 1),
        TextureType::ROCK_NORMAL,   1.0f,       // normal
        TextureType::DEFAULT,       0.0f,       // metal
        TextureType::ROCK_ROUGH,    1.0f,       // rough
        TextureType::ROCK_AO,       1.0f,       // ao
        TextureType::DEFAULT,       0.0f,       // emmissive
        TextureType::DEFAULT,       0.0f        // height
        });

    CreateMaterial(MaterialType::METAL_PLATE, 
        MaterialDescription{
        TextureType::METAL_COLOR,   Vector4(1, 1, 1, 1),
        TextureType::METAL_NORMAL,  1.0f,
        TextureType::METAL_METAL,   0.8f,
        TextureType::METAL_ROUGH,   1.0f,
        TextureType::DEFAULT,       0.0f,
        TextureType::DEFAULT,       0.0f,
        TextureType::DEFAULT,       0.0f
        });

    CreateMaterial(MaterialType::BRICKS,
        MaterialDescription{
        TextureType::BRICKS_COLOR,  Vector4(1, 1, 1, 1),
        TextureType::BRICKS_NORMAL, 1.0f,
        TextureType::DEFAULT,       0.0f,
        TextureType::BRICKS_ROUGH,  1.0f,
        TextureType::BRICKS_AO,     1.0f,
        TextureType::DEFAULT,       0.0f, 
        TextureType::DEFAULT,       0.0f  
        });
}

void MaterialManager::CreateBlankMaterial(const String& materialName)
{
    MaterialDescription defaultDesc
    {
        TextureType::DEFAULT, Vector4(1, 1, 1, 1),
        TextureType::DEFAULT, 0.0f,
        TextureType::DEFAULT, 0.0f,
        TextureType::DEFAULT, 0.0f,
        TextureType::DEFAULT, 0.0f,
        TextureType::DEFAULT, 0.0f,
        TextureType::DEFAULT, 0.0f
    };

    CreateMaterial(materialName, defaultDesc);
}

void MaterialManager::CreateMaterial(const String& materialName, MaterialDescription materialDesc)
{
    if (m_materialMap.find(materialName) != m_materialMap.end())
    {
        Debug::Log("Material '" + materialName + "' already exists.");
        return;
    }

    MaterialConstants constants = CreateMaterialConstants(materialDesc);

    std::vector<D3D12_GPU_VIRTUAL_ADDRESS> perFrameAddress;
    perFrameAddress.resize(FRAME_COUNT);

    for (UINT i = 0; i < FRAME_COUNT; ++i)
    {
        m_materialBuffer->SetCurrentFrameIndex(i);
        auto cb = m_materialBuffer->Allocate(sizeof(MaterialConstants));
        memcpy(cb.cpuPtr, &constants, sizeof(MaterialConstants));
        perFrameAddress[i] = cb.gpuAddr;
    }

    MaterialPtr newMaterial = std::make_unique<Material>(materialName, perFrameAddress, materialDesc);
    m_materialMap[materialName] = std::move(newMaterial);

    Debug::Log(materialName + " Material Created");
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

    constants.materialFlags = 0;

    if (desc.normalTex != "default_normal") constants.materialFlags |= (1 << 0);
    if (desc.metalTex != "default_black")  constants.materialFlags |= (1 << 1);
    if (desc.roughTex != "default_white")  constants.materialFlags |= (1 << 2);
    if (desc.aoTex != "default_white")  constants.materialFlags |= (1 << 3);
    if (desc.emissiveTex != "default_black")  constants.materialFlags |= (1 << 4);
    if (desc.heightTex != "default_black")  constants.materialFlags |= (1 << 5);

    return constants;
}

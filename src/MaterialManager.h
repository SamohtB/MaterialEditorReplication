#pragma once
#include "pch.h"
#include "FrameConstants.h"
#include "DynamicConstantBufferPool.h"
#include "MaterialTypes.h"
#include "Material.h"

class MaterialManager
{
public:
    MaterialManager(ID3D12Device* device);
    ~MaterialManager() = default;

    using MaterialPtr = std::unique_ptr<Material>;
    using MaterialMap = std::unordered_map<String, MaterialPtr>;

    void LoadInitialMaterials();
    void CreateBlankMaterial(const String& materialName);
    void CreateMaterial(const String& materialName, MaterialDescription materialDesc);
    void UpdateMaterial(const String& materialName, MaterialDescription materialDesc);

    void BeginFrame(UINT currentFrameIndex);
    D3D12_GPU_VIRTUAL_ADDRESS GetMaterialHandle(const String& materialName, UINT currentFrameIndex);
    MaterialDescription GetMaterialDescription(const String& materialName);

private:
    MaterialConstants CreateMaterialConstants(const MaterialDescription& desc);
    MaterialMap m_materialMap;
    std::unique_ptr<DynamicConstantBufferPool> m_materialBuffer;
};
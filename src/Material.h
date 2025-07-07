#pragma once
#include "pch.h"
#include "MaterialTypes.h"

class Material
{
public:
    Material(String name, const std::vector<D3D12_GPU_VIRTUAL_ADDRESS>& perFrameAddress, MaterialDescription description);
    ~Material() = default;

    D3D12_GPU_VIRTUAL_ADDRESS GetCBAddress(UINT currentFrameIndex);
    MaterialDescription GetDescription() const;

    void SetDescription(const MaterialDescription& description);
    void SetGPUAddress(UINT frameIndex, D3D12_GPU_VIRTUAL_ADDRESS address);

private:
    String m_name;
    std::vector<D3D12_GPU_VIRTUAL_ADDRESS> m_gpuAddresses;
    MaterialDescription m_data;
    UINT m_version;
};
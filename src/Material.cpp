#include "Material.h"
#include "FrameConstants.h"
#include "Debug.h"

Material::Material(String name, const std::vector<D3D12_GPU_VIRTUAL_ADDRESS>& perFrameAddress, MaterialDescription description)
    : m_name(name), m_gpuAddresses(perFrameAddress), m_data(description), m_version(0)
{
}

D3D12_GPU_VIRTUAL_ADDRESS Material::GetCBAddress(UINT currentFrameIndex)
{
    return this->m_gpuAddresses[currentFrameIndex];
}

MaterialDescription Material::GetDescription() const
{
    return this->m_data;
}

void Material::SetDescription(const MaterialDescription& description)
{
    this->m_data = description;
}

void Material::SetGPUAddress(UINT frameIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
{
	Debug::Assert(frameIndex < FRAME_COUNT, "Frame index out of bounds in Material::SetGPUAddress");
    this->m_gpuAddresses[frameIndex] = address;
}
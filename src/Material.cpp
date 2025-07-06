#include "Material.h"

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

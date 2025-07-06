#include "RenderDevice.h"
#include "Debug.h"

RenderDevice::RenderDevice()
{
	Debug::ThrowIfFailed(CreateDXGIFactory2(0, IID_PPV_ARGS(&m_dxgiFactory)), "DXGI factory creation failed!");
	this->m_deviceManager = std::make_unique<DeviceManager>(this->m_dxgiFactory.Get());

	auto d3dDevice = this->m_deviceManager->GetD3DDevice().Get();

	this->m_descriptorHeap = std::make_shared<DescriptorHeapManager>(d3dDevice);
	this->m_pipelineStateManager = std::make_unique<PipelineStateManager>(d3dDevice);
}

ID3D12Device* RenderDevice::GetD3DDevice() const
{
	return this->m_deviceManager->GetD3DDevice().Get();
}

ComPtr<ID3D12Device> RenderDevice::GetD3DDevicePtr() const
{
	return this->m_deviceManager->GetD3DDevice();
}

IDXGIFactory6* RenderDevice::GetFactory() const
{
	return this->m_dxgiFactory.Get();
}

PipelineStateManager* RenderDevice::GetPSOManager() const
{
	return this->m_pipelineStateManager.get();
}

DescriptorHeapManager* RenderDevice::GetDescriptorHeapManager() const
{
	return this->m_descriptorHeap.get();
}

std::shared_ptr<DescriptorHeapManager> RenderDevice::GetDescriptorHeapManagerPtr() const
{
	return this->m_descriptorHeap;
}

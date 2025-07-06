#pragma once
#include "pch.h"

#include "DeviceManager.h"
#include "DescriptorHeapManager.h"
#include "PipelineStateManager.h"

class RenderDevice
{
public:
	RenderDevice();
	~RenderDevice() = default;

	ID3D12Device* GetD3DDevice() const;
	ComPtr<ID3D12Device> GetD3DDevicePtr() const;
	IDXGIFactory6* GetFactory() const;

	PipelineStateManager* GetPSOManager() const;
	DescriptorHeapManager* GetDescriptorHeapManager() const;

	std::shared_ptr<DescriptorHeapManager> GetDescriptorHeapManagerPtr() const;

private:
	ComPtr<IDXGIFactory6> m_dxgiFactory;
	std::unique_ptr<DeviceManager> m_deviceManager;
	std::shared_ptr<DescriptorHeapManager> m_descriptorHeap;
	std::unique_ptr<PipelineStateManager> m_pipelineStateManager;
};




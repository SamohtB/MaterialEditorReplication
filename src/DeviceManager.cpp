#include "DeviceManager.h"
#include "Debug.h"

DeviceManager::DeviceManager(IDXGIFactory6* factory)
{
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		Debug::Log("Debug Layer Enabled!");
	}

	m_dxgiAdapter = FindHardwareAdapter(factory);
	CreateDeviceFromAdapter();
}

ComPtr<ID3D12Device> DeviceManager::GetD3DDevice() const
{
	return this->m_d3dDevice;
}

ComPtr<IDXGIAdapter1> DeviceManager::FindHardwareAdapter(IDXGIFactory6* factory)
{
	ComPtr<IDXGIAdapter1> adapter;

	for (UINT adapterIndex = 0;
		SUCCEEDED(factory->EnumAdapters1(adapterIndex, &adapter));
		++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		if (SUCCEEDED(D3D12CreateDevice(
			adapter.Get(),
			D3D_FEATURE_LEVEL_12_0,
			__uuidof(ID3D12Device), nullptr)))
		{
			return adapter;
		}
	}

	Debug::ThrowIfFailed(E_FAIL);
	return nullptr;
}

void DeviceManager::CreateDeviceFromAdapter()
{
	Debug::ThrowIfFailed(D3D12CreateDevice(
		m_dxgiAdapter.Get(),
		D3D_FEATURE_LEVEL_12_0,
		IID_PPV_ARGS(&m_d3dDevice)));
}
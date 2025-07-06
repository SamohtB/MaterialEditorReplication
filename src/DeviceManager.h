#pragma once
#include "pch.h"

class DeviceManager
{
public:
	DeviceManager(IDXGIFactory6* factory);
	~DeviceManager() = default;

	ComPtr<ID3D12Device> GetD3DDevice() const;

private:
	ComPtr<ID3D12Device> m_d3dDevice;
	ComPtr<IDXGIAdapter1> m_dxgiAdapter;
	ComPtr<ID3D12Debug> debugController;

	ComPtr<IDXGIAdapter1> FindHardwareAdapter(IDXGIFactory6* factory);
	void CreateDeviceFromAdapter();
};


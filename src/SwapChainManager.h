#pragma once
#include "pch.h"
#include "FrameConstants.h"

class SwapChainManager
{
public:
	SwapChainManager(IDXGIFactory6* factory, ID3D12CommandQueue* commandQueue, UINT width, UINT height, HWND hwnd);
	~SwapChainManager() = default;

	UINT GetCurrentFrameIndex() const;

	void PresentFrame();
	void UpdateFrameIndex();

	IDXGISwapChain3* GetSwapChain();
private:
	UINT m_frameIndex;
	ComPtr<IDXGISwapChain3> m_swapChain;
};

 
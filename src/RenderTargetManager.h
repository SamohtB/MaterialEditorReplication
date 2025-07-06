#pragma once
#include "pch.h"
#include "FrameConstants.h"

class RenderTargetManager 
{
public:
    RenderTargetManager(ID3D12Device* device, IDXGISwapChain3* swapChain, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandleStart, UINT descriptorSize);
    ~RenderTargetManager() = default;

    ID3D12Resource* GetRenderTarget(UINT index) const;

private:
    ComPtr<ID3D12Resource> m_renderTargets[FRAME_COUNT];

};
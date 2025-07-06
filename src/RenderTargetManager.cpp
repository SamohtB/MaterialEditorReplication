#include "RenderTargetManager.h"
#include "Debug.h"

RenderTargetManager::RenderTargetManager(ID3D12Device* device, IDXGISwapChain3* swapChain, D3D12_CPU_DESCRIPTOR_HANDLE rtvHandleStart, UINT descriptorSize)
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvHandleStart);

    for (UINT n = 0; n < FRAME_COUNT; n++)
    {
        Debug::ThrowIfFailed(swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
        device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
        rtvHandle.Offset(1, descriptorSize);
    }
}

ID3D12Resource* RenderTargetManager::GetRenderTarget(UINT index) const
{
	return m_renderTargets[index].Get();
}

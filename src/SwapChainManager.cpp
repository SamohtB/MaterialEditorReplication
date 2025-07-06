#include "SwapChainManager.h"
#include "Debug.h"

SwapChainManager::SwapChainManager(IDXGIFactory6* factory, ID3D12CommandQueue* commandQueue, UINT width, UINT height, HWND hwnd)
    : m_frameIndex(0)
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FRAME_COUNT; 
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;

    Debug::ThrowIfFailed(factory->CreateSwapChainForHwnd(commandQueue, hwnd, &swapChainDesc,
        nullptr, nullptr, &swapChain), "Swapchain creation failed!");

    Debug::ThrowIfFailed(factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER));

    Debug::ThrowIfFailed(swapChain.As(&m_swapChain));
    UpdateFrameIndex();
}

UINT SwapChainManager::GetCurrentFrameIndex() const
{
    return m_frameIndex;
}

void SwapChainManager::PresentFrame()
{
    Debug::ThrowIfFailed(this->m_swapChain->Present(1, 0), "Frame Present Fail!");
}

void SwapChainManager::UpdateFrameIndex()
{
    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
}

IDXGISwapChain3* SwapChainManager::GetSwapChain()
{
    return this->m_swapChain.Get();
}

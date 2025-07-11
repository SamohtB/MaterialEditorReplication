#pragma once
#include "pch.h"

inline UINT Align(UINT size)
{
    return (size + 255) & ~255;
}

class DynamicConstantBufferPool
{
public:
    struct Allocation 
    {
        void* cpuPtr;
        D3D12_GPU_VIRTUAL_ADDRESS gpuAddr;
    };

    DynamicConstantBufferPool(ID3D12Device* device, size_t bufferSizePerFrame);
    ~DynamicConstantBufferPool();

    void BeginFrame(UINT frameIndex);
    void SetCurrentFrameIndex(UINT frameIndex);
    Allocation Allocate(size_t size);

    /* For Static Data */
    D3D12_GPU_VIRTUAL_ADDRESS GetCurrentBufferAddress(UINT frameIndex) const;
    std::vector<void*> GetMappedAddress();

private:
    void CreateBuffers(ID3D12Device* device);

    UINT m_currentFrameIndex = 0;
    size_t m_bufferSizePerFrame;

    std::vector<ComPtr<ID3D12Resource>> m_buffers;
    std::vector<void*> m_mappedPtrs;
    std::vector<size_t> m_currentOffsets;
};


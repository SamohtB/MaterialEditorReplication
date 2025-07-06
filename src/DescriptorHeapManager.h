#pragma once
#include "pch.h"
#include "FrameConstants.h"
#include "GraphicsCommons.h"

class DescriptorHeapManager 
{
public:
    DescriptorHeapManager(ID3D12Device* device);
    ~DescriptorHeapManager() = default;

    const std::vector<ID3D12DescriptorHeap*>& GetActiveHeaps() const;

    // RTV access
    D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUHandleAt(UINT frameIndex) const;
    UINT GetRTVDescriptorSize() const;

    // SRV
    UINT AllocateSRVSlot();
    void FreeSRVSlot(UINT index);
    D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandleAt(UINT index) const;
    D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandleAt(UINT index) const;
    ID3D12DescriptorHeap* GetSRVHeap() const;
	UINT GetSRVDescriptorSize() const;

    // DSV
    D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUHandle() const;

    ID3D12DescriptorHeap* GetRTVHeap() const { return this->m_rtvHeap.Get(); }

private:
    std::vector<ID3D12DescriptorHeap*> m_activeHeaps;

    // RTV heap
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    UINT m_rtvDescriptorSize;
    UINT m_maxRTVCount;

    // SRV + CBV
    ComPtr<ID3D12DescriptorHeap> m_srvHeap;
    std::stack<UINT> m_freeSRVIndices;
    UINT m_srvDescriptorSize;
    UINT m_currentSRVOffset;

    // DSV 
    ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
};
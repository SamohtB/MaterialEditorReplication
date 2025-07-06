#include "DescriptorHeapManager.h"
#include <DirectXHelpers.h>
#include <BufferHelpers.h>
#include "Debug.h"

DescriptorHeapManager::DescriptorHeapManager(ID3D12Device* device)
    : m_maxRTVCount(FRAME_COUNT), m_currentSRVOffset(0)
{
    /* Render Target View */
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = this->m_maxRTVCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    Debug::ThrowIfFailed(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&this->m_rtvHeap)));
   
    this->m_rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    /* Shader Resource View */
    D3D12_DESCRIPTOR_HEAP_DESC  srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = MAX_SRV_COUNT;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    Debug::ThrowIfFailed(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&this->m_srvHeap)));

    this->m_srvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    /* Depth Stencil View */
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    Debug::ThrowIfFailed(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap)));

    /* add for binding */
    this->m_activeHeaps.push_back(m_srvHeap.Get());
}

const std::vector<ID3D12DescriptorHeap*>& DescriptorHeapManager::GetActiveHeaps() const
{
    return this->m_activeHeaps;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetRTVCPUHandleAt(UINT frameIndex) const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
        frameIndex,
        m_rtvDescriptorSize
    );
}

UINT DescriptorHeapManager::GetRTVDescriptorSize() const
{
    return this->m_rtvDescriptorSize; 
}

UINT DescriptorHeapManager::AllocateSRVSlot()
{
    if (!m_freeSRVIndices.empty())
    {
        UINT index = m_freeSRVIndices.top();
        m_freeSRVIndices.pop();
        return index;
    }

    Debug::Assert(this->m_currentSRVOffset < MAX_SRV_COUNT, "Exceeded SRV descriptor heap capacity");
    return this->m_currentSRVOffset++;
}

void DescriptorHeapManager::FreeSRVSlot(UINT index)
{
	Debug::Assert(index < MAX_SRV_COUNT, "Invalid SRV index to free");
	m_freeSRVIndices.push(index);
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetSRVCPUHandleAt(UINT index) const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(
        this->m_srvHeap->GetCPUDescriptorHandleForHeapStart(),
        index,
        this->m_srvDescriptorSize
    );
}

D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetSRVGPUHandleAt(UINT index) const
{
    return CD3DX12_GPU_DESCRIPTOR_HANDLE(
        this->m_srvHeap->GetGPUDescriptorHandleForHeapStart(),
        index,
        this->m_srvDescriptorSize
    );
}

ID3D12DescriptorHeap* DescriptorHeapManager::GetSRVHeap() const
{
    return this->m_srvHeap.Get();
}

UINT DescriptorHeapManager::GetSRVDescriptorSize() const
{
    return this->m_srvDescriptorSize;
}

D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapManager::GetDSVCPUHandle() const
{
    return this->m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
}


#include "DeviceContext.h"
#include "GraphicsCommons.h"
#include "Debug.h"

DeviceContext::DeviceContext(ID3D12Device* device) : m_nextFenceValue(1)
{
    /* Create Command Queue */
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    
    Debug::ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)), "Command Queue creation failed!");

    /* Create Command Allocators */
    for (int i = 0; i < FRAME_COUNT; i++)
    {
        Debug::ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[i])),
            "Command Allocator creation failed!");

        m_fenceValues[i] = 0;
    }

    /* Create and Close Command List */
    Debug::ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocators[0].Get(), 
        nullptr, IID_PPV_ARGS(&m_commandList)), "Command List creation failed!");
    Debug::ThrowIfFailed(this->m_commandList->Close());

    /* Create Fence */
    Debug::ThrowIfFailed(device->CreateFence(m_fenceValues[0], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)), "Fence creation failed!");
    m_fenceValues[0] = m_nextFenceValue;
    m_nextFenceValue++;

    /* Create Fence Event */
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_fenceEvent == nullptr)
    {
        Debug::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
}

DeviceContext::~DeviceContext()
{
    for (UINT frameIndex = 0; frameIndex < FRAME_COUNT; ++frameIndex)
    {
        this->SignalCurrentFrameGPU(frameIndex);
        this->WaitForFrameGPU(frameIndex);
    }

    CloseHandle(m_fenceEvent);
}

void DeviceContext::ExecuteCommandList()
{
    Debug::ThrowIfFailed(this->m_commandList->Close());
    ID3D12CommandList* commandLists[] = { m_commandList.Get() };
    this->m_commandQueue->ExecuteCommandLists(1, commandLists);
}

void DeviceContext::ResetCommands(UINT frameIndex, ID3D12PipelineState* pipelineState)
{
    auto allocator = m_commandAllocators[frameIndex].Get();

    // Command list allocators can only be reset when the associated 
    // command lists have finished execution on the GPU; apps should use 
    // fences to determine GPU execution progress.
    Debug::ThrowIfFailed(allocator->Reset());
    // However, when ExecuteCommandList() is called on a particular command 
    // list, that command list can then be reset at any time and must be before 
    // re-recording.
    Debug::ThrowIfFailed(m_commandList->Reset(allocator, pipelineState));
}

void DeviceContext::WaitForFrameGPU(UINT frameIndex)
{
    UINT64 fenceValue = m_fenceValues[frameIndex];
    auto val = m_fence->GetCompletedValue();

    // GPU has not reached this fence yet — must wait
    if (val < fenceValue)
    {
        Debug::ThrowIfFailed(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent));
        WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
    }
}

void DeviceContext::SignalCurrentFrameGPU(UINT frameIndex)
{
    UINT64 value = m_nextFenceValue++;
    m_fenceValues[frameIndex] = value;
    Debug::ThrowIfFailed(this->m_commandQueue->Signal(this->m_fence.Get(), value));
}

void DeviceContext::SetRootSignature(ID3D12RootSignature* rootSignature)
{
    this->m_commandList->SetGraphicsRootSignature(rootSignature);
}

void DeviceContext::SetPSO(ID3D12PipelineState* pipelineState)
{
    this->m_commandList->SetPipelineState(pipelineState);
}

void DeviceContext::SetDescriptorHeaps(const std::vector<ID3D12DescriptorHeap*>& heaps)
{
    this->m_commandList->SetDescriptorHeaps(static_cast<UINT>(heaps.size()), heaps.data());
}

void DeviceContext::SetObjectConstants(D3D12_GPU_VIRTUAL_ADDRESS address)
{
    this->m_commandList->SetGraphicsRootConstantBufferView(RootDescriptorIndex::OBJECT_CONSTANTS, address);
}

void DeviceContext::SetMaterialConstants(D3D12_GPU_VIRTUAL_ADDRESS address)
{
    this->m_commandList->SetGraphicsRootConstantBufferView(RootDescriptorIndex::MATERIAL_CONSTANTS, address);
}

void DeviceContext::SetFrameConstants(D3D12_GPU_VIRTUAL_ADDRESS address)
{
    this->m_commandList->SetGraphicsRootConstantBufferView(RootDescriptorIndex::FRAME_CONSTANTS, address);
}

void DeviceContext::SetViewport(CD3DX12_VIEWPORT* viewport)
{
    m_commandList->RSSetViewports(1, viewport);
}

void DeviceContext::SetScissorRect(CD3DX12_RECT* rect)
{
    this->m_commandList->RSSetScissorRects(1, rect);
}

/* Use before draw calls */
void DeviceContext::TransitionToRenderTarget(ID3D12Resource* resource)
{
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource,
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );

    m_commandList->ResourceBarrier(1, &barrier);
}

/* Use after draw calls */
void DeviceContext::TransitionToPresent(ID3D12Resource* resource)
{
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );

    m_commandList->ResourceBarrier(1, &barrier);
}

/* Use before draw calls */
void DeviceContext::TransitionToDepthWrite(ID3D12Resource* resource)
{
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource,
        D3D12_RESOURCE_STATE_DEPTH_READ,
        D3D12_RESOURCE_STATE_DEPTH_WRITE
    );

    m_commandList->ResourceBarrier(1, &barrier);
}

/* Use after draw calls */
void DeviceContext::TransitionToDepthRead(ID3D12Resource* resource)
{
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        resource,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        D3D12_RESOURCE_STATE_DEPTH_READ
    );

    m_commandList->ResourceBarrier(1, &barrier);
}

void DeviceContext::ClearRenderTargetColor(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, const std::vector<float>& color)
{
    D3D12_CLEAR_FLAGS clearFlags = D3D12_CLEAR_FLAG_DEPTH;
    this->m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
    this->m_commandList->ClearRenderTargetView(rtvHandle, color.data(), 0, nullptr);
    this->m_commandList->ClearDepthStencilView(dsvHandle, clearFlags, 1.0f, 0, 0, nullptr);
}

void DeviceContext::SetTexture(D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
    this->m_commandList->SetGraphicsRootDescriptorTable(RootDescriptorIndex::TEXTURES, handle);
}

void DeviceContext::SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW* vertexBufferView)
{
    this->m_commandList->IASetVertexBuffers(0, 1, vertexBufferView);
}

void DeviceContext::SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW* indexBufferView)
{
    this->m_commandList->IASetIndexBuffer(indexBufferView);
}

void DeviceContext::SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
    this->m_commandList->IASetPrimitiveTopology(topology);
}

void DeviceContext::DrawTriangleList(UINT vertexCount, UINT startVertexIndex)
{    
    this->m_commandList->DrawInstanced(vertexCount, 1, startVertexIndex, 0);
}

void DeviceContext::DrawIndexedTriangleStrip(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation)
{
    this->m_commandList->DrawIndexedInstanced(indexCount, 1, startIndexLocation, startVertexIndex, 0);
}

ID3D12CommandQueue* DeviceContext::GetCommandQueue() const
{
    return this->m_commandQueue.Get();
}

ID3D12GraphicsCommandList* DeviceContext::GetCommandList() const
{
    return this->m_commandList.Get();
}

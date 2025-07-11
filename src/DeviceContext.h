#pragma once
#include "pch.h"
#include "FrameConstants.h"

/* Reset -> Transition Barrier -> Set Params (Root/PSO/CBV/DescHeaps/Viewport/Rect) -> Draw Calls -> Transition Barrier */
class DeviceContext
{
public:
	DeviceContext(ID3D12Device* device);
	~DeviceContext();

	void ExecuteCommandList();
	void ResetCommands(UINT frameIndex, ID3D12PipelineState* pipelineState = nullptr);

	/* GPU Sync */
	void WaitForFrameGPU(UINT frameIndex);
	void SignalCurrentFrameGPU(UINT frameIndex);

	void SetRootSignature(ID3D12RootSignature* rootSignature);
	void SetPSO(ID3D12PipelineState* pipelineState);
	void SetDescriptorHeaps(const std::vector<ID3D12DescriptorHeap*>& heaps);

	void SetObjectConstants(D3D12_GPU_VIRTUAL_ADDRESS address);
	void SetMaterialConstants(D3D12_GPU_VIRTUAL_ADDRESS address);
	void SetFrameConstants(D3D12_GPU_VIRTUAL_ADDRESS address);
	void SetLightConstants(D3D12_GPU_VIRTUAL_ADDRESS address);

	void SetViewport(CD3DX12_VIEWPORT* viewport);
	void SetScissorRect(CD3DX12_RECT* rect);

	void TransitionToRenderTarget(ID3D12Resource* resource);
	void TransitionToPresent(ID3D12Resource* resource);

	void TransitionToDepthWrite(ID3D12Resource* resource);
	void TransitionToDepthRead(ID3D12Resource* resource);

	void ClearRenderTargetColor(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, const std::vector<float>& color);

	void SetTexture(D3D12_GPU_DESCRIPTOR_HANDLE handle);
	void SetVertexBuffer(D3D12_VERTEX_BUFFER_VIEW* vertexBufferView);
	void SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW* indexBufferView);
	void SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology);

	void DrawTriangleList(UINT vertexCount, UINT startVertexIndex);
	void DrawIndexedTriangleStrip(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation);

	ID3D12CommandQueue* GetCommandQueue() const;
	ID3D12GraphicsCommandList* GetCommandList() const;
private:
	ComPtr<ID3D12Fence> m_fence;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[FRAME_COUNT];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	HANDLE m_fenceEvent;
	UINT64 m_fenceValues[FRAME_COUNT];
	UINT64 m_nextFenceValue;
};


#pragma once
#include "pch.h"
#include <ResourceUploadBatch.h>
#include "VertexBuffer.h"

using namespace DirectX;

struct VertexBufferInfo 
{
	ComPtr<ID3D12Resource> buffer;
	D3D12_VERTEX_BUFFER_VIEW view;
};

struct IndexBufferInfo
{
	ComPtr<ID3D12Resource> buffer;
	D3D12_INDEX_BUFFER_VIEW view;
};

class BatchUploader
{
public:
	BatchUploader(ComPtr<ID3D12Device> device);
	~BatchUploader() = default;

	void StartUpload();
	void StopAndWaitUpload();

	VertexBufferInfo SchedVertexBuffer(const std::vector<Vertex>& vertices);
	IndexBufferInfo SchedIndexBuffer(const std::vector<unsigned int>& indices);
	ComPtr<ID3D12Resource> SchedTexture(const std::wstring& filePath, D3D12_CPU_DESCRIPTOR_HANDLE handle);
	ComPtr<ID3D12Resource> SchedWhitePixelTexture(D3D12_CPU_DESCRIPTOR_HANDLE handle);

private:
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	std::unique_ptr<ResourceUploadBatch> m_resourceUploader;
	
	bool m_uploadStarted = false;
};

#include "BatchUploader.h"
#include <DirectXHelpers.h>
#include <BufferHelpers.h>
#include <WICTextureLoader.h>

#include "Debug.h"

BatchUploader::BatchUploader(ComPtr<ID3D12Device> device) : m_device(device)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    Debug::ThrowIfFailed(this->m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)), 
        "Command Queue creation failed!");

    this->m_resourceUploader = std::make_unique<ResourceUploadBatch>(this->m_device.Get());
}

void BatchUploader::StartUpload()
{
    if (this->m_uploadStarted) 
    {
        Debug::LogWarning("Upload Already In Progress!");
        return;
    }
        
    this->m_uploadStarted = true;
    this->m_resourceUploader->Begin();
}

void BatchUploader::StopAndWaitUpload()
{
    auto future = this->m_resourceUploader->End(this->m_commandQueue.Get());
    future.wait();

    this->m_uploadStarted = false;
}

IndexBufferInfo BatchUploader::SchedIndexBuffer(const std::vector<unsigned int>& indices)
{
    Debug::Assert(this->m_uploadStarted, "Upload Not Yet Started! | Invalid Shedule!");

    size_t indexBufferSize = indices.size() * sizeof(UINT);
    IndexBufferInfo indexBufferInfo;

    Debug::ThrowIfFailed(DirectX::CreateStaticBuffer(
        this->m_device.Get(), 
        *this->m_resourceUploader, 
        indices,
        D3D12_RESOURCE_STATE_INDEX_BUFFER,
        indexBufferInfo.buffer.GetAddressOf()
    ));

    indexBufferInfo.view.BufferLocation = indexBufferInfo.buffer->GetGPUVirtualAddress();
    indexBufferInfo.view.SizeInBytes = static_cast<UINT>(indexBufferSize);
    indexBufferInfo.view.Format = DXGI_FORMAT_R32_UINT;

    return indexBufferInfo;
}

ComPtr<ID3D12Resource> BatchUploader::SchedTexture(const std::wstring& filePath, D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
    Debug::Assert(this->m_uploadStarted, "Upload Not Yet Started! | Invalid Shedule!");

    ComPtr<ID3D12Resource> textureBuffer;

    Debug::ThrowIfFailed(DirectX::CreateWICTextureFromFile(
        this->m_device.Get(),
        *this->m_resourceUploader,
        filePath.c_str(),
        &textureBuffer,
        false //mipmaps
    ));

    DirectX::CreateShaderResourceView(this->m_device.Get(), 
        textureBuffer.Get(), handle);

    return textureBuffer;
}

ComPtr<ID3D12Resource> BatchUploader::SchedWhitePixelTexture(D3D12_CPU_DESCRIPTOR_HANDLE handle)
{
    Debug::Assert(this->m_uploadStarted, "Upload Not Yet Started! | Invalid Schedule!");

    static const uint8_t whitePixel[4] = { 255, 255, 255, 255 };

    D3D12_SUBRESOURCE_DATA subresource = {};
    subresource.pData = whitePixel;
    subresource.RowPitch = 4;
    subresource.SlicePitch = 4;

    ComPtr<ID3D12Resource> textureBuffer;
    Debug::ThrowIfFailed(DirectX::CreateTextureFromMemory(
        this->m_device.Get(), 
        *this->m_resourceUploader,
        4, 
        DXGI_FORMAT_R8G8B8A8_UNORM, 
        subresource, 
        &textureBuffer
    ));

    DirectX::CreateShaderResourceView(this->m_device.Get(), textureBuffer.Get(), handle);

    return textureBuffer;
}

VertexBufferInfo BatchUploader::SchedVertexBuffer(const std::vector<Vertex>& vertices)
{
    Debug::Assert(this->m_uploadStarted, "Upload Not Yet Started! | Invalid Shedule!");

    size_t vertexBufferSize = vertices.size() * sizeof(Vertex);
    VertexBufferInfo vertexBufferInfo;

    Debug::ThrowIfFailed(DirectX::CreateStaticBuffer(
        this->m_device.Get(), 
        *this->m_resourceUploader, 
        vertices,
        D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, 
        vertexBufferInfo.buffer.GetAddressOf()
    ));

    vertexBufferInfo.view.BufferLocation = vertexBufferInfo.buffer->GetGPUVirtualAddress();
    vertexBufferInfo.view.StrideInBytes = sizeof(Vertex);
    vertexBufferInfo.view.SizeInBytes = static_cast<UINT>(vertexBufferSize);

    return vertexBufferInfo;
}

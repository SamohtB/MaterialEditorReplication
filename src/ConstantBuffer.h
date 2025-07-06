#pragma once
#include "pch.h"
#include "Math.h"
#include "Debug.h"

inline UINT Align256(UINT size) 
{
    return (size + 255) & ~255;
}

template<typename T>
class ConstantBuffer
{
public:
    ConstantBuffer(ID3D12Device* device, UINT count);
    virtual ~ConstantBuffer();

    D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress(UINT index) const;
    void Update(const T& data, UINT index);


protected:
    ComPtr<ID3D12Resource> m_resource;
    BYTE* m_mappedData;
    UINT m_alignedSize;
};

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer(ID3D12Device* device, UINT count)
{
    m_alignedSize = Align256(sizeof(T));
    UINT totalBufferSize = m_alignedSize * count;

    Debug::Assert(totalBufferSize <= 65536, "CB size too big for a single constant buffer (max 64KB)");

    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(totalBufferSize);

    Debug::ThrowIfFailed(device->CreateCommittedResource(&heapProps,D3D12_HEAP_FLAG_NONE,
        &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_resource)),
        "Constant Buffer creation failed!");

    CD3DX12_RANGE readRange(0, 0);
    Debug::ThrowIfFailed(m_resource->Map(0, &readRange, reinterpret_cast<void**>(&m_mappedData)));
}

template<typename T>
inline ConstantBuffer<T>::~ConstantBuffer()
{
    if (m_resource) {
        m_resource->Unmap(0, nullptr);
        m_mappedData = nullptr;
    }
}

template<typename T>
inline D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer<T>::GetGPUVirtualAddress(UINT index) const
{
    return m_resource->GetGPUVirtualAddress() + index * Align256(sizeof(T));
}

template<typename T>
inline void ConstantBuffer<T>::Update(const T& data, UINT index)
{
    memcpy(reinterpret_cast<uint8_t*>(m_mappedData) + index * m_alignedSize, &data, sizeof(T));
}

struct alignas(256) MaterialConstantsData
{
    uint32_t diffuseHandleIndex = 0;
    uint32_t normalHandleIndex = 0;
    float normalStr = 0;
    uint32_t metalHandleIndex = 0;
    float metalStr = 0;
    uint32_t roughHandleIndex = 0;
    float roughStr = 0;
    uint32_t ambientOcclussionHandleIndex = 0;
    float ambientOcclussionStr = 0;
    uint32_t materialFlags = 0;
};

class MaterialConstantsBuffer : public ConstantBuffer<MaterialConstantsData>
{
public:
    using ConstantBuffer::ConstantBuffer;
};

struct alignas(256) ObjectConstantsData
{
    Matrix modelMatrix;
    UINT objectId;
};

class ObjectConstantsBuffer : public ConstantBuffer<ObjectConstantsData>
{
public:
    using ConstantBuffer::ConstantBuffer;
};

struct alignas(256) FrameConstantsData
{
    Matrix viewMatrix;
    Matrix projMatrix;
	Vector3 cameraPosition;
};

class FrameConstantsBuffer : public ConstantBuffer<FrameConstantsData>
{
public:
    using ConstantBuffer::ConstantBuffer;
};
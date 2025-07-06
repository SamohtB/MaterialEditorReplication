#pragma once
#include "pch.h"

class IndexBuffer
{
public:
    IndexBuffer(const std::vector<unsigned int>& indices);
    ~IndexBuffer() = default;

    D3D12_INDEX_BUFFER_VIEW* GetIndexBufferViewPointer();

private:
    ComPtr<ID3D12Resource> m_indexBuffer;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
};


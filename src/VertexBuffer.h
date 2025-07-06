#pragma once
#include "pch.h"
#include "Math.h"

struct Vertex
{
    Vector3 position;
    Vector2 texcoord;
    Vector3 normal;
    Vector3 tangent;
    Vector3 bitangent;
};

class VertexBuffer
{
public:
    VertexBuffer() = default;
	~VertexBuffer() = default;

    VertexBuffer(const std::vector<Vertex>& vertices);

    D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferViewPointer();

private:
    ComPtr<ID3D12Resource> m_vertexBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
};


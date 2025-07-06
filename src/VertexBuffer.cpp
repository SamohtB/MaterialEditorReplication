#include "VertexBuffer.h"
#include "GraphicsEngine.h"
#include "BatchUploader.h"

VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices)
{
    VertexBufferInfo info = GraphicsEngine::GetInstance()->GetBatchUploader()->SchedVertexBuffer(vertices);

    this->m_vertexBuffer = info.buffer;
    this->m_vertexBufferView = info.view;
}

D3D12_VERTEX_BUFFER_VIEW* VertexBuffer::GetVertexBufferViewPointer()
{
    return &m_vertexBufferView;
}

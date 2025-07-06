#include "IndexBuffer.h"
#include "GraphicsEngine.h"
#include "BatchUploader.h"

IndexBuffer::IndexBuffer(const std::vector<unsigned int>& indices)
{
    IndexBufferInfo info = GraphicsEngine::GetInstance()->GetBatchUploader()->SchedIndexBuffer(indices);

    this->m_indexBuffer = info.buffer;
    this->m_indexBufferView = info.view;
}

D3D12_INDEX_BUFFER_VIEW* IndexBuffer::GetIndexBufferViewPointer()
{
	return &this->m_indexBufferView;
}

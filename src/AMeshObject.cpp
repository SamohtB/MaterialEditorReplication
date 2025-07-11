#include "AMeshObject.h"

#include "GameObjectManager.h"

#include "GraphicsEngine.h"

#include "DeviceContext.h"
#include "RenderSystem.h"
#include "RenderDevice.h"

#include "TextureManager.h"
#include "MaterialManager.h"
#include "LightManager.h"

#include "Debug.h"

AMeshObject::AMeshObject(String name, String shader, String material) : AGameObject(name), m_material(material) 
{
	m_gpuAddresses.resize(FRAME_COUNT);
}

void AMeshObject::Update(float deltaTime)
{
}

void AMeshObject::Draw(DeviceContext* context, String shader)
{
    auto renderSystem = GraphicsEngine::GetInstance()->GetRenderSystem();
    auto frameIndex = renderSystem->GetCurrentFrameIndex();

    context->SetPSO(renderSystem->GetPipelineState(shader));
    context->SetObjectConstants(this->m_gpuAddresses[frameIndex]);
    context->SetFrameConstants(renderSystem->GetFrameConstantsAddress());
    context->SetTexture(GraphicsEngine::GetInstance()->GetTextureManager()->GetSRVStart());
    context->SetMaterialConstants(GraphicsEngine::GetInstance()->GetMaterialManager()->GetMaterialHandle(this->m_material, frameIndex));
	context->SetLightConstants(GraphicsEngine::GetInstance()->GetLightManager()->GetLightBuffer(frameIndex));

    context->SetVertexBuffer(this->m_vertexBuffer->GetVertexBufferViewPointer());
    context->SetIndexBuffer(this->m_indexBuffer->GetIndexBufferViewPointer());

	context->SetTopology(this->m_topology);
    context->DrawIndexedTriangleStrip(m_indicesSize, 0, 0);
}

void AMeshObject::SetMaterial(String material)
{
	if (material != this->m_material)
	{
		this->m_material = material;
	}
}

String AMeshObject::GetMaterial() const
{
    return this->m_material;
}

void AMeshObject::SetGPUAddress(UINT frameIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
{
    Debug::Assert(frameIndex < FRAME_COUNT, "Frame index out of bounds in AMeshObject::SetGPUAddress");
    this->m_gpuAddresses[frameIndex] = address;
}

void AMeshObject::SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
	this->m_topology = topology;
}

void AMeshObject::SetGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    m_vertexBuffer = std::make_unique<VertexBuffer>(vertices);
    m_indexBuffer = std::make_unique<IndexBuffer>(indices);
    m_indicesSize = static_cast<UINT>(indices.size());
}
#pragma once
#include "pch.h"
#include "FrameConstants.h"

#include "TextureTypes.h"
#include "ShaderTypes.h"
#include "MaterialTypes.h"

#include "AGameObject.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Colors.h"

class DeviceContext;

class AMeshObject : public AGameObject
{
public:
	AMeshObject(String name, String material = MaterialType::DEFAULT);
	virtual ~AMeshObject() = default;

	virtual void Update(float deltaTime) override;
	virtual void Draw(DeviceContext* context);

	void SetMaterial(String material);
	String GetMaterial() const;

	void SetGPUAddress(UINT frameIndex, D3D12_GPU_VIRTUAL_ADDRESS address);

protected:
	void SetGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology);

private:
	/* To DO: Move Buffers to a Mesh Manager and Create a primitve factory */
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	std::unique_ptr<IndexBuffer> m_indexBuffer;
	std::vector<D3D12_GPU_VIRTUAL_ADDRESS> m_gpuAddresses;

	UINT m_indicesSize;
	String m_material;
	D3D12_PRIMITIVE_TOPOLOGY m_topology;
};


#pragma once
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
	AMeshObject(String name, String shader = ShaderTypes::UNLIT, String material = MaterialType::DEFAULT);
	virtual ~AMeshObject() = default;

	virtual void Update(float deltaTime) override final;
	virtual void Draw(DeviceContext* context, String shader) override;

	void SetMaterial(String material);
	String GetMaterial() const;

protected:
	void SetGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void SetTopology(D3D12_PRIMITIVE_TOPOLOGY topology);
	virtual void OnUpdate(float deltaTime) = 0;

private:
	/* To DO: Move Buffers to a Mesh Manager and Create a primitve factory */
	std::unique_ptr<VertexBuffer> m_vertexBuffer;
	std::unique_ptr<IndexBuffer> m_indexBuffer;

	UINT m_indicesSize;
	String m_material;
	D3D12_PRIMITIVE_TOPOLOGY m_topology;
};


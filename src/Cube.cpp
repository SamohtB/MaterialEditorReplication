#include "Cube.h"
#include "DeviceContext.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "Random.h"

Cube::Cube(String name) : AMeshObject(name)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    static const Vector3 faceNormals[] =
    {
        {  0,  0,  1 }, // Front
        {  0,  0, -1 }, // Back
        {  0,  1,  0 }, // Top
        {  0, -1,  0 }, // Bottom
        { -1,  0,  0 }, // Left
        {  1,  0,  0 }, // Right
    };

    static const Vector3 faceTangents[] =
    {
        { 1,  0,  0 }, // Front
        { -1, 0,  0 }, // Back
        { 1,  0,  0 }, // Top
        { 1,  0,  0 }, // Bottom
        { 0,  0, -1 }, // Left
        { 0,  0,  1 }, // Right
    };

    static const Vector2 texCoords[] =
    {
        { 0, 1 }, { 1, 1 }, { 0, 0 }, { 1, 0 }
    };

    // Cube face data
    const Vector3 positions[] =
    {
        // Front face
        {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f},
        // Back face
        { 0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
        // Top face
        {-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
        // Bottom face
        {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f},
        // Left face
        {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f,  0.5f},
        // Right face
        { 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f},
    };

    for (int face = 0; face < 6; ++face)
    {
        for (int i = 0; i < 4; ++i)
        {
            Vertex v;
            v.position = positions[face * 4 + i];
            v.normal = faceNormals[face];
            v.texcoord = texCoords[i];
            v.tangent = Vector3(0, 0, 0);
            vertices.push_back(v);
        }
    }

    for (int i = 0; i < 6; ++i)
    {
        int offset = i * 4;
        indices.push_back(offset + 0);
        indices.push_back(offset + 2);
        indices.push_back(offset + 1);

        indices.push_back(offset + 2);
        indices.push_back(offset + 3);
        indices.push_back(offset + 1);
    }

    // Calculate tangents
    std::vector<Vector3> positionsOnly(vertices.size());
    std::vector<Vector3> normals(vertices.size());
    std::vector<Vector2> texcoords(vertices.size());
    std::vector<Vector3> tangents(vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        positionsOnly[i] = vertices[i].position;
        normals[i] = vertices[i].normal;
        texcoords[i] = vertices[i].texcoord;
    }

    GeoMath::CalculateTangentFrame(indices, positionsOnly.data(), normals.data(), texcoords.data(), vertices.size(), tangents.data());

    for (size_t i = 0; i < vertices.size(); ++i)
        vertices[i].tangent = tangents[i];

    SetGeometry(vertices, indices);
    SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	SetMaterial(MaterialType::DEFAULT);
}

void Cube::Update(float deltaTime)
{

}

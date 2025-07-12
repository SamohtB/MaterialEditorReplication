#include "Cylinder.h"
#include "DeviceContext.h"
#include "RenderSystem.h"
#include "RenderDevice.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "Random.h"

Cylinder::Cylinder(String name) : AMeshObject(name)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const float halfHeight = m_height * 0.5f;
    const float angleStep = 2.0f * M_PI / m_segments;

    // --- SIDE WALL ---
    for (int i = 0; i <= m_segments; ++i)
    {
        float theta = i * angleStep;
        float x = m_radius * cosf(theta);
        float z = m_radius * sinf(theta);
        float u = static_cast<float>(i) / m_segments;

        Vector3 normal = Vector3(x, 0, z);
        normal.Normalize();

        Vertex vTop{
            { x, +halfHeight, z },
            { u, 0 },
            normal,
            { 0, 0, 0 }
        };

        Vertex vBottom{
            { x, -halfHeight, z },
            { u, 1 },
            normal,
            { 0, 0, 0 }
        };

        vertices.push_back(vTop);
        vertices.push_back(vBottom);
    }

    for (int i = 0; i < m_segments; ++i)
    {
        int top0 = i * 2;
        int bottom0 = i * 2 + 1;
        int top1 = (i + 1) * 2;
        int bottom1 = (i + 1) * 2 + 1;

        indices.push_back(top0);
        indices.push_back(bottom0);
        indices.push_back(top1);

        indices.push_back(top1);
        indices.push_back(bottom0);
        indices.push_back(bottom1);
    }

    // --- TOP CAP ---
    int topCenterIndex = static_cast<int>(vertices.size());
    vertices.push_back(Vertex{
        { 0, +halfHeight, 0 },
        { 0.5f, 0.5f },
        { 0, 1, 0 },
        { 0, 0, 0 }
        });

    for (int i = 0; i <= m_segments; ++i)
    {
        float theta = i * angleStep;
        float x = m_radius * cosf(theta);
        float z = m_radius * sinf(theta);

        float u = 0.5f + 0.5f * cosf(theta);
        float v = 0.5f - 0.5f * sinf(theta);

        vertices.push_back(Vertex{
            { x, +halfHeight, z },
            { u, v },
            { 0, 1, 0 },
            { 0, 0, 0 }
            });

        if (i < m_segments)
        {
            indices.push_back(topCenterIndex);
            indices.push_back(topCenterIndex + 1 + i);
            indices.push_back(topCenterIndex + 2 + i);
        }
    }

    // --- BOTTOM CAP ---
    int bottomCenterIndex = static_cast<int>(vertices.size());
    vertices.push_back(Vertex{
        { 0, -halfHeight, 0 },
        { 0.5f, 0.5f },
        { 0, -1, 0 },
        { 0, 0, 0 }
        });

    for (int i = 0; i <= m_segments; ++i)
    {
        float theta = i * angleStep;
        float x = m_radius * cosf(theta);
        float z = m_radius * sinf(theta);

        float u = 0.5f + 0.5f * cosf(theta);
        float v = 0.5f + 0.5f * sinf(theta);

        vertices.push_back(Vertex{
            { x, -halfHeight, z },
            { u, v },
            { 0, -1, 0 },
            { 0, 0, 0 }
            });

        if (i < m_segments)
        {
            indices.push_back(bottomCenterIndex);
            indices.push_back(bottomCenterIndex + 2 + i);
            indices.push_back(bottomCenterIndex + 1 + i);
        }
    }

    // --- TANGENT FRAME COMPUTATION ---
    std::vector<Vector3> positions(vertices.size());
    std::vector<Vector3> normals(vertices.size());
    std::vector<Vector2> texcoords(vertices.size());
    std::vector<Vector3> tangents(vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        positions[i] = vertices[i].position;
        normals[i] = vertices[i].normal;
        texcoords[i] = vertices[i].texcoord;
    }

    GeoMath::CalculateTangentFrame(indices, positions.data(), normals.data(), texcoords.data(), vertices.size(), tangents.data());

    for (size_t i = 0; i < vertices.size(); ++i)
        vertices[i].tangent = tangents[i];

    // --- FINALIZE ---
    SetGeometry(vertices, indices);
    SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    SetMaterial(MaterialType::DEFAULT);
}


void Cylinder::Update(float deltaTime)
{
}

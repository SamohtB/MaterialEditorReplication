#include "Plane.h"

Plane::Plane(String name) : AMeshObject(name)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Vector3 positions[] =
    {
        { -5.0f, 0.0f, -5.0f }, // 0
        { -5.0f, 0.0f,  5.0f }, // 1
        {  5.0f, 0.0f, -5.0f }, // 2
        {  5.0f, 0.0f,  5.0f }, // 3
    };

    Vector2 texcoords[] =
    {
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f }
    };

    Vector3 normal = { 0.0f, 1.0f, 0.0f };

    for (int i = 0; i < 4; ++i)
    {
        Vertex v;
        v.position = positions[i];
        v.texcoord = texcoords[i];
        v.normal = normal;
        v.tangent = { 0.0f, 0.0f, 0.0f };
        vertices.push_back(v);
    }

    indices =
    {
        0, 1, 2,
        2, 1, 3
    };

    std::vector<Vector3> tangents(vertices.size());

    GeoMath::CalculateTangentFrame(indices, &vertices[0].position, &vertices[0].normal, &vertices[0].texcoord, vertices.size(), tangents.data());

    for (size_t i = 0; i < vertices.size(); ++i)
        vertices[i].tangent = tangents[i];

    SetGeometry(vertices, indices);
    SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    SetMaterial(MaterialType::DEFAULT);
}

void Plane::OnUpdate(float deltaTime)
{
}

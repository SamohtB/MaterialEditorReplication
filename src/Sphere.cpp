#include "Sphere.h"

Sphere::Sphere(String name) : AMeshObject(name)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const uint32_t verticalSegments = TESSELLATION;
    const uint32_t horizontalSegments = TESSELLATION * 2;

    // Create rings of vertices at progressively higher latitudes.
    for (uint32_t i = 0; i <= verticalSegments; i++)
    {
        const float v = 1 - float(i) / float(verticalSegments);

        const float latitude = (float(i) * M_PI / float(verticalSegments)) - M_PI / 2.0f;

        float dy, dxz;
        DirectX::XMScalarSinCos(&dy, &dxz, latitude);

        // Create a single ring of vertices at this latitude.
        for (uint32_t j = 0; j <= horizontalSegments; j++)
        {
            const float u = float(j) / float(horizontalSegments);
            const float longitude = float(j) * (M_PI * 2.0f) / float(horizontalSegments);

            float dx, dz;
            DirectX::XMScalarSinCos(&dx, &dz, longitude);

            dx *= dxz;
            dz *= dxz;

            Vector3 normal(dx, dy, dz);
            Vector2 texcoord(u, v);
            Vector3 position = normal * RADIUS;

            vertices.push_back(Vertex{
                position,
                texcoord,
                normal,
                Vector3(0, 0, 0) // Placeholder tangent
                });
        }
    }

    // Fill the index buffer with triangles joining each pair of latitude rings.
    const uint32_t stride = horizontalSegments + 1;

    for (uint32_t i = 0; i < verticalSegments; i++)
    {
        for (uint32_t j = 0; j <= horizontalSegments; j++)
        {
            const uint32_t nextI = i + 1;
            const uint32_t nextJ = (j + 1) % stride;

            indices.push_back(i * stride + j);         
            indices.push_back(i * stride + nextJ);  
            indices.push_back(nextI * stride + j);   

            indices.push_back(i * stride + nextJ);    
            indices.push_back(nextI * stride + nextJ); 
            indices.push_back(nextI * stride + j);
        }
    }

    std::vector<Vector3> tangents(vertices.size());
    std::vector<Vector3> positions(vertices.size());
    std::vector<Vector3> normals(vertices.size());
    std::vector<Vector2> texcoords(vertices.size());

    for (size_t i = 0; i < vertices.size(); ++i)
    {
        positions[i] = vertices[i].position;
        normals[i] = vertices[i].normal;
        texcoords[i] = vertices[i].texcoord;
    }

    GeoMath::CalculateTangentFrame(indices, positions.data(), normals.data(), texcoords.data(), vertices.size(), tangents.data());

    for (size_t i = 0; i < vertices.size(); ++i)
        vertices[i].tangent = tangents[i];

    SetGeometry(vertices, indices);
    SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    SetMaterial(MaterialType::DEFAULT);
}

void Sphere::Update(float deltaTime)
{
	
}

#pragma once
#include "pch.h"
#include <SimpleMath.h>

using Vector2 = DirectX::SimpleMath::Vector2;
using Vector3 = DirectX::SimpleMath::Vector3;
using Vector4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;

#define M_PI 3.14159265358979323846f

template<typename T>
inline T Clamp(const T& value, const T& min, const T& max)
{
    return (value < min) ? min : (value > max) ? max : value;
}

namespace GeoMath
{
    static void CalculateTangentFrame(const std::vector<unsigned int>& indices, const Vector3* positions, const Vector3* normals,
        const Vector2* texcoords, size_t vertexCount, Vector3* tangents)
    {
        for (int i = 0; i < vertexCount; ++i)
        {
            tangents[i] = Vector3(0.0f, 0.0f, 0.0f);
        }

        for (int i = 0; i < indices.size(); i += 3)
        {
            unsigned int i0 = indices[i];
            unsigned int i1 = indices[i + 1];
            unsigned int i2 = indices[i + 2];

            const Vector3& p0 = positions[i0];
            const Vector3& p1 = positions[i1];
            const Vector3& p2 = positions[i2];

            const Vector2& uv0 = texcoords[i0];
            const Vector2& uv1 = texcoords[i1];
            const Vector2& uv2 = texcoords[i2];

            Vector3 edge1 = p1 - p0;
            Vector3 edge2 = p2 - p0;

            Vector2 deltaUV1 = uv1 - uv0;
            Vector2 deltaUV2 = uv2 - uv0;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            Vector3 tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);

            tangents[i0] += tangent;
            tangents[i1] += tangent;
            tangents[i2] += tangent;
        }

        for (int i = 0; i < vertexCount; ++i)
        {
            tangents[i].Normalize();
        }

    };
};
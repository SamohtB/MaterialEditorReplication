#pragma once
#include <string>
#include "TextureTypes.h"
#include "Math.h"

class MaterialType
{
public:
    inline static constexpr const char* DEFAULT = "DEFAULT";
    inline static constexpr const char* ROCK = "ROCK";
    inline static constexpr const char* METAL_PLATE = "METAL_PLATE";
    inline static constexpr const char* BRICKS = "BRICKS";
};

enum MaterialMapType : int
{
    ALBEDO = 0,
    NORMAL,
    METAL,
    ROUGH,
    AO,
    HEIGHT,
    EMMISSIVE
};

constexpr size_t MATERIAL_MAP_COUNT = 7;

enum MaterialFlags : uint32_t
{
    HasAlbedoMap = 1 << 0,
    HasNormalMap = 1 << 1,
    HasMetallicMap = 1 << 2,
    HasRoughnessMap = 1 << 3,
    HasAOMap = 1 << 4,
    HasEmissiveMap = 1 << 5,
    HasHeightMap = 1 << 6
};

struct alignas(16) MaterialConstants
{
    uint32_t diffuseHandleIndex;
    uint32_t normalHandleIndex;
    uint32_t metalHandleIndex;
    uint32_t roughHandleIndex;

    uint32_t aoHandleIndex;
    uint32_t emmissiveHandleIndex;
    uint32_t heightHandleIndex;
    uint32_t materialFlags = 0;

    Vector4 baseColor;

    float normalStr;
    float metalStr;
    float roughStr;
    float aoStr;

    float emmissiveStr;
    float heightStr;
    Vector2 pad;
};

struct MaterialDescription
{
    std::string albedoTex;
    std::string normalTex;
    std::string metalTex;
    std::string roughTex;
    std::string aoTex;
    Vector4 albedoColor;
    std::string emissiveTex;
    std::string heightTex;

    float normalStrength;
    float metalStrength;
    float roughStrength;
    float aoStrength;
    float emissiveStrength;
    float heightStrength;

    MaterialDescription(
        const std::string& albedoTex = "",
        const Vector4& albedoColor = Vector4(1, 1, 1, 1),
        const std::string& normalTex = "", float normalStrength = 1.0f,
        const std::string& metalTex = "", float metalStrength = 0.0f,
        const std::string& roughTex = "", float roughStrength = 1.0f,
        const std::string& aoTex = "", float aoStrength = 1.0f,
        const std::string& emissiveTex = "", float emissiveStrength = 0.0f,
        const std::string& heightTex = "", float heightStrength = 0.0f)
        : albedoTex(albedoTex), albedoColor(albedoColor),
        normalTex(normalTex), normalStrength(normalStrength),
        metalTex(metalTex), metalStrength(metalStrength),
        roughTex(roughTex), roughStrength(roughStrength),
        aoTex(aoTex), aoStrength(aoStrength),
        emissiveTex(emissiveTex), emissiveStrength(emissiveStrength),
        heightTex(heightTex), heightStrength(heightStrength)
    {
    }
};


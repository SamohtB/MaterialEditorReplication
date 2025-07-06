#pragma once

class TextureType
{
public:
    inline static constexpr const char* DEFAULT = "DEFAULT_TEXTURE";

    inline static constexpr const char* ROCK_COLOR = "ROCK_COLOR";
    inline static constexpr const char* ROCK_NORMAL = "ROCK_NORMAL";
    inline static constexpr const char* ROCK_ROUGH = "ROCK_ROUGH";
    inline static constexpr const char* ROCK_AO = "ROCK_AO";

    inline static constexpr const char* METAL_COLOR = "METAL_COLOR";
    inline static constexpr const char* METAL_NORMAL = "METAL_NORMAL";
    inline static constexpr const char* METAL_ROUGH = "METAL_ROUGH";
    inline static constexpr const char* METAL_METAL = "METAL_METAL";

    inline static constexpr const char* BRICKS_COLOR = "BRICKS_COLOR";
    inline static constexpr const char* BRICKS_NORMAL = "BRICKS_NORMAL";
    inline static constexpr const char* BRICKS_ROUGH = "BRICKS_ROUGH";
    inline static constexpr const char* BRICKS_AO = "BRICKS_AO";
};
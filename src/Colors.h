#pragma once
#include <array>
#include "Math.h"

class ColorUtils 
{
public:
    static std::array<float, 4> ToFloatArray(const Vector3& vec) 
    {
        return { vec.x, vec.y, vec.z, 1.0f };
    }
};

class ColorPalette 
{
public:
    static const Vector3 White; // Default color, not included in palette
    static const Vector3 Red;
    static const Vector3 Orange;
    static const Vector3 Yellow;
    static const Vector3 Green;
    static const Vector3 Violet;
    static const Vector3 Magenta;
    static const Vector3 Blue;
    static const Vector3 Teal;

    // Get color palette vector (excluding white)
    static const std::vector<Vector3>& GetPalette() {
        static const std::vector<Vector3> palette = {
            Red,
            Orange,
            Yellow,
            Green,
            Violet,
            Magenta,
            Blue,
            Teal
        };
        return palette;
    }
};

#pragma once
#include <random>

class Random
{
public:
    static float Range(float min, float max)
    {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(min, max);
        return dist(rng);
    }
};
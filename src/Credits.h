#pragma once
#include "AUIScreen.h"

class Credits :  public AUIScreen
{
public:
    Credits();
    ~Credits() = default;

    void DrawUI() override;
};
#pragma once
#include "AUIScreen.h"
class ColorPicker :  public AUIScreen
{
public:
    ColorPicker();
    ~ColorPicker() = default;

    void DrawUI() override;
};


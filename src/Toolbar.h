#pragma once
#include "AUIScreen.h"

class Toolbar : public AUIScreen
{
public:
    Toolbar();
    ~Toolbar() = default;

    // Inherited via AUIScreen
    void DrawUI() override;

    void Lighting();

private:
    void FileMenu();
    void GameObjects();
    void DisplayFPS();
    void Windows();
    void ShaderSelector();
};


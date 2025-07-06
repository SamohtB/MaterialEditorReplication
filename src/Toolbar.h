#pragma once
#include "AUIScreen.h"

class Toolbar : public AUIScreen
{
public:
    Toolbar();
    ~Toolbar() = default;

    // Inherited via AUIScreen
    void DrawUI() override;

private:
    void FileMenu();
    void GameObjects();
    void DisplayFPS();
    void Windows();
    void ShaderSelector();
};


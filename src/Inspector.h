#pragma once
#include "AUIScreen.h"

class AGameObject;

class Inspector : public AUIScreen
{
public:
    Inspector();
    ~Inspector() = default;

    void DrawUI() override;

private:
    void DrawObjectInfo(AGameObject* object);
    void DrawTransformTab(AGameObject* object);
    void DrawMaterialTab(AGameObject* object);
    bool DrawTextureField(const char* label, std::string& textureName);
};
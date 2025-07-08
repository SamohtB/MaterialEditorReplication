#include "Inspector.h"
#include "GameObjectManager.h"
#include "AGameObject.h"
#include "AMeshObject.h"
#include "MaterialTypes.h"
#include "GraphicsEngine.h"
#include "MaterialManager.h"
#include "TextureManager.h"

Inspector::Inspector() : AUIScreen("Inspector") {}

void Inspector::DrawUI()
{
    if (!this->IsVisible()) //
    {
        return;
    }

    bool is_visible = this->IsVisible(); //
    ImGui::Begin("Inspector", &is_visible);

    AGameObject* object = GameObjectManager::GetInstance()->GetSelectedObject();
    if (object != nullptr)
    {
        DrawObjectInfo(object);
        ImGui::Spacing();

        if (ImGui::BeginTabBar("InspectorTabs"))
        {
            if (ImGui::BeginTabItem("Properties"))
            {
                DrawTransformTab(object);
                ImGui::EndTabItem();
            }

            if (dynamic_cast<AMeshObject*>(object) != nullptr)
            {
                if (ImGui::BeginTabItem("Material"))
                {
                    DrawMaterialTab(object);
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
    }
    else
    {
        ImGui::Text("No object selected.");
    }

    ImGui::End();

    this->SetVisible(is_visible); //
}

void Inspector::DrawObjectInfo(AGameObject* object)
{
    ImGui::Text("Name: %s", object->GetName().c_str());
}

void Inspector::DrawTransformTab(AGameObject* object)
{
    Vector3 position = object->GetLocalPosition();
    Vector3 rotation = object->GetLocalRotation();
    Vector3 scale = object->GetLocalScale();

    ImGui::SetNextItemOpen(true, ImGuiCond_Once);
    if (ImGui::TreeNode("Transform"))
    {
        if (ImGui::DragFloat3("Position", &position.x, 0.1f)) { object->SetPosition(position); }
        if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f)) { object->SetRotation(rotation); }
        if (ImGui::DragFloat3("Scale", &scale.x, 0.1f)) { object->SetScale(scale); }
        ImGui::TreePop();
    }
}

void Inspector::DrawMaterialTab(AGameObject* object)
{
    auto meshObject = dynamic_cast<AMeshObject*>(object);
    if (!meshObject) return;

    static const char* materialOptions[] = {
        MaterialType::DEFAULT, MaterialType::ROCK, MaterialType::METAL_PLATE, MaterialType::BRICKS
    };

    std::string currentMat = meshObject->GetMaterial();
    int currentIndex = 0;
    for (int i = 0; i < IM_ARRAYSIZE(materialOptions); ++i)
    {
        if (currentMat == materialOptions[i])
        {
            currentIndex = i;
            break;
        }
    }

    if (ImGui::Combo("Material", &currentIndex, materialOptions, IM_ARRAYSIZE(materialOptions)))
    {
        meshObject->SetMaterial(materialOptions[currentIndex]);
        return;
    }

    MaterialDescription matDesc = GraphicsEngine::GetInstance()->GetMaterialManager()->GetMaterialDescription(currentMat);
    bool propertyChanged = false;

    ImGui::SeparatorText("Main Maps");
    if (DrawTextureField("Albedo", matDesc.albedoTex)) { propertyChanged = true; }
    ImGui::SameLine();
    if (ImGui::ColorEdit4("Albedo Color", &matDesc.albedoColor.x)) { propertyChanged = true; }

    if (DrawTextureField("Metallic", matDesc.metalTex)) { propertyChanged = true; }
    ImGui::SameLine();
    if (ImGui::SliderFloat("Metal Strength", &matDesc.metalStrength, 0.0f, 1.0f)) { propertyChanged = true; }

    if (DrawTextureField("Rough", matDesc.roughTex)) { propertyChanged = true; }
    ImGui::SameLine();
    if (ImGui::SliderFloat("Rough Strength", &matDesc.roughStrength, 0.0f, 1.0f)) { propertyChanged = true; }

    if (DrawTextureField("Normal", matDesc.normalTex)) { propertyChanged = true; }
    ImGui::SameLine();
    if (ImGui::SliderFloat("Strength", &matDesc.normalStrength, 0.0f, 2.0f)) { propertyChanged = true; }

    if (DrawTextureField("Height Map", matDesc.heightTex)) { propertyChanged = true; }
    ImGui::SameLine();
    if (ImGui::SliderFloat("Height Scale", &matDesc.heightStrength, 0.0f, 1.0f)) { propertyChanged = true; }

    if (DrawTextureField("Occlusion", matDesc.aoTex)) { propertyChanged = true; }
    ImGui::SameLine();
    if (ImGui::SliderFloat("AO Strength", &matDesc.aoStrength, 0.0f, 1.0f)) { propertyChanged = true; }

    if (propertyChanged)
    {
        GraphicsEngine::GetInstance()->GetMaterialManager()->UpdateMaterial(currentMat, matDesc);
    }
}

bool Inspector::DrawTextureField(const char* label, std::string& textureName)
{
    bool changed = false;
    ImGui::Text("%s: %s", label, textureName.c_str());

    ImTextureID texID = GraphicsEngine::GetInstance()->GetTextureManager()->GetThumbnail(textureName);

    if (ImGui::ImageButton(label, (void*)texID, ImVec2(16, 16)))
    {
        ImGui::OpenPopup(label);
    }

    if (ImGui::BeginPopup(label))
    {
        const std::vector<const char*>& textureList = TextureType::GetAllTextures();

        for (const char* texName : textureList)
        {
            if (ImGui::Selectable(texName))
            {
                textureName = texName;
                changed = true;
                ImGui::CloseCurrentPopup();
                break;
            }
        }

        ImGui::EndPopup();
    }
    return changed;
}
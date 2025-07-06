#include "Inspector.h"
#include "GameObjectManager.h"
#include "AGameObject.h"
#include "AMeshObject.h"
#include "MaterialTypes.h"
#include "GraphicsEngine.h"
#include "MaterialManager.h"
#include "TextureManager.h"

Inspector::Inspector() : AUIScreen("Inspector")
{
}

void Inspector::DrawUI()
{
    ImGui::Begin("Inspector");

    AGameObject* object = GameObjectManager::GetInstance()->GetSelectedObject();

    if (object != nullptr)
    {
		DrawObjectInfo(object);
        DrawTransformTab(object);
		DrawMaterialTab(object);
    }

    else
    {
        ImGui::Text("No object selected.");
    }

    ImGui::End();
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
        if (ImGui::DragFloat3("Position", &position.x, 0.1f))
        {
            object->SetPosition(position);
        }
        if (ImGui::DragFloat3("Rotation", &rotation.x, 0.1f))
        {
            object->SetRotation(rotation);
        }
        if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
        {
            object->SetScale(scale);
        }

        ImGui::TreePop();
    }
}

void Inspector::DrawMaterialTab(AGameObject* object)
{
    ImGui::SetNextItemOpen(true, ImGuiCond_Once);

    if (ImGui::TreeNode("Material"))
    {
        static const char* materialOptions[] = 
        {
            MaterialType::DEFAULT,
            MaterialType::ROCK,
            MaterialType::METAL_PLATE,
            MaterialType::BRICKS
        };

		auto meshObject = dynamic_cast<AMeshObject*>(object);
        auto currentMat = meshObject->GetMaterial();
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
        }

        auto matDesc = GraphicsEngine::GetInstance()->GetMaterialManager()->GetMaterialDescription(currentMat);

        ImGui::SeparatorText("Albedo");
        DrawTextureField("Albedo Texture", matDesc.albedoTex);
        ImGui::ColorEdit4("Albedo Color", &matDesc.albedoColor.x);

        ImGui::SeparatorText("Normal Map");
        DrawTextureField("Normal Texture", matDesc.normalTex);
        ImGui::SliderFloat("Normal Strength", &matDesc.normalStrength, 0.0f, 2.0f);

        ImGui::SeparatorText("Metallic");
        DrawTextureField("Metal Texture", matDesc.metalTex);
        ImGui::SliderFloat("Metal Strength", &matDesc.metalStrength, 0.0f, 1.0f);

        ImGui::SeparatorText("Roughness");
        DrawTextureField("Rough Texture", matDesc.roughTex);
        ImGui::SliderFloat("Rough Strength", &matDesc.roughStrength, 0.0f, 1.0f);

        ImGui::SeparatorText("Ambient Occlusion");
        DrawTextureField("AO Texture", matDesc.aoTex);
        ImGui::SliderFloat("AO Strength", &matDesc.aoStrength, 0.0f, 1.0f);

        ImGui::SeparatorText("Emissive");
        DrawTextureField("Emissive Texture", matDesc.emissiveTex);
        ImGui::SliderFloat("Emissive Strength", &matDesc.emissiveStrength, 0.0f, 10.0f);

        ImGui::SeparatorText("Height");
        DrawTextureField("Height Texture", matDesc.heightTex);
        ImGui::SliderFloat("Height Strength", &matDesc.heightStrength, 0.0f, 1.0f);

        ImGui::TreePop();
    }
}

void Inspector::DrawTextureField(const char* label, std::string& textureName)
{
    ImGui::Text("%s", label);

    ImTextureID texID = GraphicsEngine::GetInstance()->GetTextureManager()->GetThumbnail(textureName);

    if (texID)  
    {
        ImGui::Image(texID, ImVec2(64, 64));
    }
}
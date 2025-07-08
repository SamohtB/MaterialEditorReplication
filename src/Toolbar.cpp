#include "Toolbar.h"
#include "GameObjectSpawner.h"
#include "Win32App.h"
#include "EngineTime.h"
#include "EngineGUIManager.h"
#include "GraphicsEngine.h"
#include "RenderSystem.h"

Toolbar::Toolbar() : AUIScreen("TOOLBAR")
{

}

void Toolbar::DrawUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        FileMenu();
        GameObjects();
        Windows();
        ShaderSelector();
        DisplayFPS();
    }

    ImGui::EndMainMenuBar();
}

void Toolbar::GameObjects()
{
    /* Primitive Spawners */
    if (ImGui::BeginMenu("Primitives"))
    {
        if (ImGui::MenuItem("Cube")) { GameObjectSpawner::CreatePrimitive(ObjectType::PRIMITVE_CUBE); }
        if (ImGui::MenuItem("Plane")) { GameObjectSpawner::CreatePrimitive(ObjectType::PRIMITVE_PLANE); }
        if (ImGui::MenuItem("Sphere")) { GameObjectSpawner::CreatePrimitive(ObjectType::PRIMITVE_SPHERE); }
        ImGui::EndMenu();
    }
}

void Toolbar::FileMenu()
{


    /* File Menus*/
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Clear Scene"))
        {
            GameObjectManager::GetInstance()->ClearAllObjects();
        }

        if (ImGui::MenuItem("Close"))
        {
            Win32App::Quit();
        }

        if (ImGui::MenuItem("About"))
        {
            auto ui = EngineGUIManager::GetInstance()->GetUI("CREDITS");
            bool visibility = ui->IsVisible();
            ui->SetVisible(!visibility);
        }

        ImGui::EndMenu();
    }
}

void Toolbar::DisplayFPS()
{
    int fps = EngineTime::GetFPS();
    std::string fpsText = "FPS: " + std::to_string(fps);

    float windowWidth = ImGui::GetWindowWidth();
    float textWidth = ImGui::CalcTextSize(fpsText.c_str()).x;
    ImGui::SameLine(windowWidth - textWidth - 20.0f);

    ImGui::Text("%s", fpsText.c_str());
}

void Toolbar::Windows()
{
    if (ImGui::BeginMenu("Windows"))
    {
        auto uiList = EngineGUIManager::GetInstance()->GetAllScreens();
 
        for (const auto& screen : uiList)
        {
            if (screen->GetName() == "TOOLBAR") continue; //exclude self

            bool visible = screen->IsVisible();

            if (ImGui::MenuItem(screen->GetName().c_str(), nullptr, visible))
            {
                screen->SetVisible(!visible);
            }
        }

        ImGui::EndMenu();
    }
}

void Toolbar::ShaderSelector()
{
    static const char* shaderOptions[] = 
    {
        ShaderTypes::UNLIT,
        ShaderTypes::LIT
    };

    static int currentShaderIndex = 0;

    std::string label = shaderOptions[currentShaderIndex];
    float labelWidth = ImGui::CalcTextSize(label.c_str()).x + 40.0f;
    float windowWidth = ImGui::GetWindowWidth();

    // Reserve space: FPS is ~90px, shader selector sits to its left with padding
    float comboPos = windowWidth - 90.0f - labelWidth - 20.0f;

    ImGui::SameLine();
    ImGui::SetCursorPosX(comboPos);
    ImGui::SetNextItemWidth(labelWidth);

    if (ImGui::BeginCombo("##ShaderSelector", shaderOptions[currentShaderIndex], ImGuiComboFlags_NoArrowButton))
    {
        for (int i = 0; i < IM_ARRAYSIZE(shaderOptions); ++i)
        {
            bool isSelected = (i == currentShaderIndex);
            if (ImGui::Selectable(shaderOptions[i], isSelected))
            {
                currentShaderIndex = i;
                GraphicsEngine::GetInstance()->GetRenderSystem()->SetActiveShader(shaderOptions[i]);
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

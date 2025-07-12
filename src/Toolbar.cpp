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
        Lighting();
        Windows();
        DisplayFPS();
    }

    ImGui::EndMainMenuBar();
}

void Toolbar::Lighting()
{
    if (ImGui::BeginMenu("Lighting"))
    {
        if (ImGui::MenuItem("Point Light")) { GameObjectSpawner::CreateLight(); }
        ImGui::EndMenu();
    }
}

void Toolbar::GameObjects()
{
    /* Primitive Spawners */
    if (ImGui::BeginMenu("Primitives"))
    {
        if (ImGui::MenuItem("Cube")) { GameObjectSpawner::CreatePrimitive(ObjectType::PRIMITIVE_CUBE); }
        if (ImGui::MenuItem("Plane")) { GameObjectSpawner::CreatePrimitive(ObjectType::PRIMITIVE_PLANE); }
        if (ImGui::MenuItem("Sphere")) { GameObjectSpawner::CreatePrimitive(ObjectType::PRIMITIVE_SPHERE); }
        if (ImGui::MenuItem("Cylinder")) { GameObjectSpawner::CreatePrimitive(ObjectType::PRIMITIVE_CYLINDER); }
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

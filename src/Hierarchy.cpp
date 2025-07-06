#include "Hierarchy.h"
#include "GameObjectManager.h"
#include "AGameObject.h"

Hierarchy::Hierarchy() : AUIScreen("Hierarchy")
{

}

void Hierarchy::DrawUI()
{
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.9f));

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

    if (ImGui::Begin("Hierarchy", nullptr, window_flags))
    {

        auto objectList = GameObjectManager::GetInstance()->GetAllObjects();

        for (AGameObject* gameObject : objectList)
        {
            DrawGameObjectNode(gameObject);
        }
    }

    ImGui::End();

    ImGui::PopStyleColor();
}

void Hierarchy::DrawGameObjectNode(AGameObject* gameObject)
{
    bool isSelected = GameObjectManager::GetInstance()->GetSelectedObject() == gameObject;

    if (isSelected)
    {
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.3f, 0.6f, 0.3f, 1.0f)); // Green for selected
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.5f, 0.8f, 0.5f, 1.0f)); // Hover effect
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Default color
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f)); // Hover effect
    }

    if (ImGui::Selectable(gameObject->GetName().c_str(), isSelected))
    {
        if (isSelected)
        {
            GameObjectManager::GetInstance()->SetSelectedObject(nullptr);
        }
        else
        {
            GameObjectManager::GetInstance()->SetSelectedObject(gameObject);
        }
    }

    if (isSelected)
    {
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Delete").x - ImGui::GetStyle().ItemSpacing.x * 2);
        ImGui::SetCursorPosY(cursorPos.y + ImGui::GetStyle().ItemSpacing.y / 2);

        if (ImGui::Button("Delete"))
        {
            GameObjectManager::GetInstance()->DeleteObject(gameObject);
            GameObjectManager::GetInstance()->SetSelectedObject(nullptr);
        }
    }

    ImGui::PopStyleColor(2);
}
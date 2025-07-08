#include "Credits.h"

Credits::Credits() : AUIScreen("Credits")
{
	this->SetVisible(false);
}

void Credits::DrawUI()
{
    if (ImGui::Begin("About", &this->m_visible, ImGuiWindowFlags_None))
    {
        ImGui::Text("Scene Editor v0.068");
        ImGui::Text("Developed by: Thomas Banatin");
        ImGui::Separator();
        ImGui::Text("Made Possible by: ");
        ImGui::Text("DirectX Samples @ https://github.com/microsoft/DirectX-Graphics-Samples");
        ImGui::Text("DirectX Shader Compiler");
        ImGui::Text("Dear Imgui");
    }

    ImGui::End();
}

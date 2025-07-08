#include "Credits.h"
#include "GraphicsEngine.h"
#include "TextureManager.h"
#include "TextureTypes.h"

Credits::Credits() : AUIScreen("Credits")
{
	this->SetVisible(false);
}

void Credits::DrawUI()
{
    ImGui::SetNextWindowSize(ImVec2(400, 300));

    if (ImGui::Begin("About", &this->m_visible, ImGuiWindowFlags_NoResize))
    {
        ImTextureID texID = GraphicsEngine::GetInstance()->GetTextureManager()->GetThumbnail(TextureType::LOGO);

        ImGui::Image((void*)texID, ImVec2(128, 128));
        ImGui::SeparatorText("Scene Editor v0.068");
        ImGui::Text("Developed by: Thomas Banatin");
        ImGui::Separator();
        ImGui::Text("Made Possible by: ");
        ImGui::Text("DirectX Samples @ \n https://github.com/microsoft/DirectX-Graphics-Samples");
        ImGui::Text("DirectX Shader Compiler");
        ImGui::Text("Dear Imgui");
    }

    ImGui::End();
}

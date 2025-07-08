#include "ColorPicker.h"

ColorPicker::ColorPicker() : AUIScreen("Color Picker")
{
	this->SetVisible(false);
}

void ColorPicker::DrawUI()
{
	float col[4];

	if (ImGui::Begin("Color Picker", &this->m_visible))
	{
		ImGui::ColorPicker4("Color", col);
	}

	ImGui::End();
}

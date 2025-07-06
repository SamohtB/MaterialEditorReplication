#include "AUIScreen.h"

AUIScreen::AUIScreen(String name) : m_name(name), m_visible(true)
{
}

std::string AUIScreen::GetName()
{
    return this->m_name;
}

bool AUIScreen::IsVisible() const
{
    return this->m_visible;
}

void AUIScreen::SetVisible(bool visible)
{
    this->m_visible = visible;
}

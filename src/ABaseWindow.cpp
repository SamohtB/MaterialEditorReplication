#include "ABaseWindow.h"

ABaseWindow::ABaseWindow(UINT width, UINT height) : m_width(width), m_height(height)
{
	m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

unsigned int ABaseWindow::GetWidth() const
{
	return this->m_width;
}

unsigned int ABaseWindow::GetHeight() const
{
	return this->m_height;
}
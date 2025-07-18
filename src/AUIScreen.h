#pragma once
#include "pch.h"

class AUIScreen
{
public:
    using String = std::string;

    AUIScreen(String name);
    ~AUIScreen() = default;

    virtual void DrawUI() = 0;

    String GetName();
    bool IsVisible() const;
    void SetVisible(bool visible);

protected:
    bool m_visible;

private:
    String m_name;
    
};


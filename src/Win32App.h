#pragma once
#include "pch.h"

class ABaseWindow;

/* Win32 platform-specific window application host. */
class Win32App
{
public:
    Win32App(ABaseWindow* window, std::wstring windowName);
    HWND GetHwnd();

    bool IsRun();
    void Broadcast();
    static void Quit();

protected:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    ABaseWindow* m_window = nullptr;

    static HWND m_hwnd;
    bool m_isRun = true;
};

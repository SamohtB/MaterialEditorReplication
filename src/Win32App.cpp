#include "Win32App.h"
#include "EngineTime.h"
#include "InputSystem.h"
#include "ABaseWindow.h"
#include "Debug.h"

HWND Win32App::m_hwnd = nullptr;

Win32App::Win32App(ABaseWindow* window, std::wstring windowName) : m_window(window), m_isRun(true)
{
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = &WindowProc;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpszClassName = L"MyWindowClass";

	if (!RegisterClassEx(&wc)) 
	{
		throw std::runtime_error("Failed to register window class.");
	}

	const UINT width = window->GetWidth();
	const UINT height = window->GetHeight();

	RECT rc = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };

	if (!AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE))
	{
		throw std::runtime_error("Window adjust rect failed.");
	}

	m_hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
		L"MyWindowClass",		//Window
		windowName.c_str(),				//Window Name
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, //Style
		CW_USEDEFAULT, CW_USEDEFAULT, // Window Position
		rc.right - rc.left, rc.bottom - rc.top, //Window Size
		NULL, NULL, wc.hInstance, this);

	if (!m_hwnd)
	{
		throw std::runtime_error("CreateWindowEx failed");
	}

	window->OnCreate(m_hwnd);

	::ShowWindow(m_hwnd, SW_SHOW);
}

HWND Win32App::GetHwnd()
{
	return m_hwnd;
}

bool Win32App::IsRun()
{
	return m_isRun;
}

LRESULT Win32App::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	if (msg == WM_CREATE)
	{
		auto* createStruct = reinterpret_cast<LPCREATESTRUCT>(lparam);
		auto* app = reinterpret_cast<Win32App*>(createStruct->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
		return 0;
	}

	auto* app = reinterpret_cast<Win32App*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (!app) return DefWindowProc(hwnd, msg, wparam, lparam);

	switch (msg)
	{

	case WM_MOUSEWHEEL:
	{
		float scroll = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam)) / 120.0f;
		InputSystem::GetInstance()->GetMouseScrollDelta(scroll);
		break;
	}

	case WM_DESTROY:
		if (app->m_window)
		{
			app->m_isRun = false;
			app->m_window->OnDestroy();
		}
		::PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Win32App::Broadcast()
{
	EngineTime::LogFrameStart();

	MSG msg = {};

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (!m_isRun || !m_window) return;

	this->m_window->OnUpdate();
	this->m_window->OnRender();

	EngineTime::LogFrameEnd();
	EngineTime::UpdateFPSCounter();
}

void Win32App::Quit()
{
	if (m_hwnd)
	{
		::PostMessage(m_hwnd, WM_CLOSE, 0, 0);
		Debug::Log("Closing application...");
	}
		
}



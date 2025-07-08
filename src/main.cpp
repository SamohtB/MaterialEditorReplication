#include "Win32App.h"
#include "GameWindow.h"
#include "Debug.h"

int main() 
{
	/* COM INIT for DirectXTex */
#if (_WIN32_WINNT >= 0x0A00 /*_WIN32_WINNT_WIN10*/)
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	Debug::ThrowIfFailed(initialize);
#else
	Debug::ThrowIFFailed(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED));
#endif
	try
	{
		GameWindow game = GameWindow(1024, 768);
		Win32App app = Win32App(&game, L"DirectX Application");

		while (app.IsRun()) 
		{
			app.Broadcast();
		}
	}
	catch (...)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
#include "System.h"

int WINAPI wWinMain( HINSTANCE hInstance_, HINSTANCE hPrevInstance_, LPWSTR lpCmdLine_, int nCmdShow_ )
{
	AllocConsole();
	HWND hConsole = GetConsoleWindow();
	HWND hDesktop = GetDesktopWindow();
	RECT desktop;
	GetWindowRect(hDesktop,&desktop);
	SetWindowPos(hConsole, 0, 960, 0, desktop.right - 960, desktop.bottom, SWP_NOSENDCHANGING );

	freopen("CONIN$", "r", stdin );
	freopen("CONOUT$", "w", stdout );
	freopen("CONOUT$", "w", stderr );

	System* sys = new System;
	sys->Initialize( hInstance_, nCmdShow_ );
	int local_return = sys->Run();
	sys->Release();
	return local_return;
}
#include "System.h"

// Temporary
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
//

int WINAPI wWinMain( HINSTANCE hInstance_, HINSTANCE hPrevInstance_, LPWSTR lpCmdLine_, int nCmdShow_ )
{
	// Temporary
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );  
	//_CrtSetBreakAlloc(267);
	//

	AllocConsole();
	HWND hConsole = GetConsoleWindow();
	HWND hDesktop = GetDesktopWindow();
	RECT desktop;
	GetWindowRect(hDesktop,&desktop);
	SetWindowPos(hConsole, 0, 1000, 0, desktop.right - 1100, desktop.bottom - 100, SWP_DRAWFRAME );
	
	freopen("CONIN$", "r", stdin );
	freopen("CONOUT$", "w", stdout );
	freopen("CONOUT$", "w", stderr );

	System* sys = new System;
	sys->Initialize( hInstance_, nCmdShow_ );
	int local_return = sys->Run();

	sys->Release();
	delete sys;
	
	return local_return;
}
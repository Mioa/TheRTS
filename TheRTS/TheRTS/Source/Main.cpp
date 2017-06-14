#include "System.h"

int WINAPI wWinMain( HINSTANCE hInstance_, HINSTANCE hPrevInstance_, LPWSTR lpCmdLine_, int nCmdShow_ )
{
	System* sys = new System;
	sys->Initialize( hInstance_, nCmdShow_ );
	int local_return = sys->Run();
	sys->Release();
	return local_return;
}
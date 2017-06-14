#ifndef SYSTEM_H
#define SYSTEM_H

#include "Game.h"
#include <Windows.h>

class System
{
	//Variables
	public:
	private:
		HWND	windowHandle;
		LONG	windowWidth;
		LONG	windowHeight;

	//Functions
	public:
		HRESULT	Initialize( HINSTANCE hInstance_, int nCmdShow_ );
		void	Release();

				System();
				~System();
	public:
		int		Run();

	private:
		static LRESULT CALLBACK WndProc( HWND hWnd_, UINT uMsg_, WPARAM wParam_, LPARAM lParam_ );
};
#endif
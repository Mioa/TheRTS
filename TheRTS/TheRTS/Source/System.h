#ifndef SYSTEM_H
#define SYSTEM_H

#include "Game.h"
#include <Windows.h>
#include <iostream>

class System
{
	//Variables
	public:
	private:
		struct Timer
		{
			private:
				LARGE_INTEGER start, stop, elapsed, freq;
			public:
			Timer()			{ QueryPerformanceFrequency( &freq ); }
			void Start()	{ QueryPerformanceCounter( &start ); }
			void Stop()		{ QueryPerformanceCounter( &stop ); }
			double GetTime()
			{
				elapsed.QuadPart = stop.QuadPart - start.QuadPart;
				return (double)elapsed.QuadPart / freq.QuadPart;
			}
		};

		HWND	windowHandle;
		LONG	windowWidth;
		LONG	windowHeight;
		Game	game;
		Timer	timer;

		float	fpsValue;

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
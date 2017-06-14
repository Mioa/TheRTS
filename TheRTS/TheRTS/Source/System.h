#include "Game.h"
#include <Windows.h>

class System
{
	//Variables
	private:
		HWND	windowHandle;
		LONG	windowWidth;
		LONG	windowHeight;

	//Functions
	public:
		int		Run();

		HRESULT	Initialize( HINSTANCE hInstance_, int nCmdShow_ );
		void	Release();

				System();
				~System();
	private:
		static LRESULT CALLBACK WndProc( HWND hWnd_, UINT uMsg_, WPARAM wParam_, LPARAM lParam_ );
};
#include "System.h"
#include "Input.h"

HRESULT System::Initialize( HINSTANCE hInstance_, int nCmdShow_ )
{
	windowWidth		= 960;
	windowHeight	= 600;

	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( wc ) );
	wc.cbSize			= sizeof( wc );
	wc.hInstance		= hInstance_;
	wc.lpfnWndProc		= WndProc;
	wc.lpszClassName	= L"TheRTS";
	wc.style			= CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx( &wc );
	RECT rc = { 0, 0, windowWidth, windowHeight };

	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	windowHandle = CreateWindow( 
		L"TheRTS",
		L"TheRTS",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance_,
		NULL );

	if( windowHandle )
		ShowWindow( windowHandle, nCmdShow_ );

	game.Initialize( windowHandle, windowWidth, windowHeight );

	Input::GetInstance()->Initialize();

	return S_OK;
}

void System::Release()
{
	game.Release();
}

System::System()
{

}

System::~System()
{

}

int	System::Run()
{
	MSG msg = { 0 };

	while( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			Input::GetInstance()->Update();
			Input::GetInstance()->Clear();

			game.Update( 1.0f );
			game.Render();
		}
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK System::WndProc( HWND hWnd_, UINT uMsg_, WPARAM wParam_, LPARAM lParam_ )
{
	switch (uMsg_) 
	{
		case WM_KEYDOWN:
			switch( wParam_ )
			{
				case 'A'		: Input::GetInstance()->pressFrame[I_KEY::A]			= true; break;
				case 'D'		: Input::GetInstance()->pressFrame[I_KEY::D]			= true; break;
				case 'S'		: Input::GetInstance()->pressFrame[I_KEY::S]			= true; break;
				case 'W'		: Input::GetInstance()->pressFrame[I_KEY::W]			= true; break;
				case VK_LEFT	: Input::GetInstance()->pressFrame[I_KEY::ARROW_LEFT]	= true; break;
				case VK_RIGHT	: Input::GetInstance()->pressFrame[I_KEY::ARROW_RIGHT]	= true; break;
				case VK_DOWN	: Input::GetInstance()->pressFrame[I_KEY::ARROW_DOWN]	= true; break;
				case VK_UP		: Input::GetInstance()->pressFrame[I_KEY::ARROW_UP]		= true; break;
				case VK_ESCAPE:
					PostQuitMessage( 0 );
					break;
			}
			break;

		case WM_KEYUP:
			switch( wParam_ )
			{
				case 'A'		: Input::GetInstance()->releaseFrame[I_KEY::A]				= true; break;
				case 'D'		: Input::GetInstance()->releaseFrame[I_KEY::D]				= true; break;
				case 'S'		: Input::GetInstance()->releaseFrame[I_KEY::S]				= true; break;
				case 'W'		: Input::GetInstance()->releaseFrame[I_KEY::W]				= true; break;
				case VK_LEFT	: Input::GetInstance()->releaseFrame[I_KEY::ARROW_LEFT]		= true; break;
				case VK_RIGHT	: Input::GetInstance()->releaseFrame[I_KEY::ARROW_RIGHT]	= true; break;
				case VK_DOWN	: Input::GetInstance()->releaseFrame[I_KEY::ARROW_DOWN]		= true; break;
				case VK_UP		: Input::GetInstance()->releaseFrame[I_KEY::ARROW_UP]		= true; break;
			}
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;		
	}

	return DefWindowProc( hWnd_, uMsg_, wParam_, lParam_ );
}
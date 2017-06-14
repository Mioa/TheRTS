#include "System.h"
#include "Input.h"

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
		}
	}
	return (int)msg.wParam;
}

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

	return S_OK;
}

void System::Release()
{

}

System::System()
{

}

System::~System()
{

}

LRESULT CALLBACK System::WndProc( HWND hWnd_, UINT uMsg_, WPARAM wParam_, LPARAM lParam_ )
{
	switch (uMsg_) 
	{
		case WM_KEYDOWN:
			if( wParam_ == VK_ESCAPE )
				PostQuitMessage( 0 );
			break;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;		
	}

	return DefWindowProc( hWnd_, uMsg_, wParam_, lParam_ );
}
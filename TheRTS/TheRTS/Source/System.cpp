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

	Input::GetInstance()->Initialize();

	game.Initialize( windowHandle, windowWidth, windowHeight );

	return S_OK;
}

void System::Release()
{
	game.Release();

	RenderQueue::GetInstance()->Release();
	delete RenderQueue::GetInstance();

	Input::GetInstance()->Release();
	delete Input::GetInstance();

	Lockstep::GetInstance()->Release();
	delete Lockstep::GetInstance();
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

	fpsValue = 0.0f;
	timer.Start();
	while( WM_QUIT != msg.message )
	{
		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			timer.Stop();
			float deltaTime = (float)timer.GetTime(); 
			timer.Start();

			fpsValue = fpsValue * 0.95f + 0.5f/deltaTime;

			std::string title = "TheRTS - FPS: " + std::to_string( fpsValue );
			SetWindowTextA( windowHandle, title.c_str() );

			game.Update( deltaTime );
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
				case 'H'		: Input::GetInstance()->pressFrame[I_KEY::H]			= true; break;
				case 'J'		: Input::GetInstance()->pressFrame[I_KEY::J]			= true; break;
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
				case 'H'		: Input::GetInstance()->releaseFrame[I_KEY::H]				= true; break;
				case 'J'		: Input::GetInstance()->releaseFrame[I_KEY::J]				= true; break;
				case 'S'		: Input::GetInstance()->releaseFrame[I_KEY::S]				= true; break;
				case 'W'		: Input::GetInstance()->releaseFrame[I_KEY::W]				= true; break;
				case VK_LEFT	: Input::GetInstance()->releaseFrame[I_KEY::ARROW_LEFT]		= true; break;
				case VK_RIGHT	: Input::GetInstance()->releaseFrame[I_KEY::ARROW_RIGHT]	= true; break;
				case VK_DOWN	: Input::GetInstance()->releaseFrame[I_KEY::ARROW_DOWN]		= true; break;
				case VK_UP		: Input::GetInstance()->releaseFrame[I_KEY::ARROW_UP]		= true; break;
			}
			break;
		case WM_LBUTTONDOWN:
			{
				Input::GetInstance()->pressFrame[I_KEY::MOUSE_LEFT] = true; break;
			}
		case WM_LBUTTONUP:
			{
				Input::GetInstance()->releaseFrame[I_KEY::MOUSE_LEFT] = true; break;
			}
		case WM_RBUTTONDOWN:
			{
				Input::GetInstance()->pressFrame[I_KEY::MOUSE_RIGHT] = true; break;
			}
		case WM_RBUTTONUP:
			{
				Input::GetInstance()->releaseFrame[I_KEY::MOUSE_RIGHT] = true; break;
			}
		case WM_MOUSEMOVE: 
			{
				Input::GetInstance()->currentMousePos[0] = GET_X_LPARAM(lParam_);
				Input::GetInstance()->currentMousePos[1] = GET_Y_LPARAM(lParam_);
				break;
			}
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;		
	}

	return DefWindowProc( hWnd_, uMsg_, wParam_, lParam_ );
}
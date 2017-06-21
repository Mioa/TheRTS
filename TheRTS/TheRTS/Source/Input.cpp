#include "Input.h"

HRESULT Input::Initialize()
{
	ZeroMemory( pressFrame, sizeof( bool ) * I_KEY::COUNT );
	ZeroMemory( releaseFrame, sizeof( bool ) * I_KEY::COUNT );
	ZeroMemory( currentFrame, sizeof( bool ) * I_KEY::COUNT );
	ZeroMemory( previousFrame, sizeof( bool ) * I_KEY::COUNT );

	mousePos[0]	= 0;
	mousePos[1]	= 0;

	cameraPos = DirectX::XMFLOAT4( 0.0f, 10.0f, -2.0f, 1.0f );
	DirectX::XMStoreFloat4( &cameraDir, DirectX::XMVector4Normalize( DirectX::XMVectorSet( 0.0f, -10.0f, 2.0f, 0.0f ) ) );

	cameraSpeed = 0.005f;

	return S_OK;
}

void Input::Release()
{

}

void Input::Update()
{
	memcpy( previousFrame, currentFrame, sizeof( bool ) * I_KEY::COUNT );
	for( int i = 0; i < I_KEY::COUNT; i++ )
	{
		if( pressFrame[i] )
			currentFrame[i] = true;
		else if( releaseFrame[i] )
			currentFrame[i] = false;
	}

	DirectX::XMFLOAT4 camMove(
		(Input_KeyDown( I_KEY::ARROW_RIGHT ) ? cameraSpeed : 0.0f ) - ( Input_KeyDown( I_KEY::ARROW_LEFT ) ? cameraSpeed : 0.0f ),
		0.0f,
		( Input_KeyDown( I_KEY::ARROW_UP ) ? cameraSpeed : 0.0f ) - ( Input_KeyDown( I_KEY::ARROW_DOWN ) ? cameraSpeed : 0.0f ),
		0.0f
		);

	cameraPos = AddXMF4( cameraPos, camMove );
}

void Input::Clear()
{
	ZeroMemory( pressFrame, sizeof( bool ) * I_KEY::COUNT );
	ZeroMemory( releaseFrame, sizeof( bool ) * I_KEY::COUNT );
}

bool Input::KeyDown( I_KEY::Keys key )
{
	return currentFrame[key];
}

bool Input::KeyPressed( I_KEY::Keys key )
{
	return !previousFrame[key] && currentFrame[key];
}

bool Input::KeyReleased( I_KEY::Keys key )
{
	return previousFrame[key] && !currentFrame[key];
}
#include "Input.h"

HRESULT Input::Initialize()
{
	ZeroMemory( pressFrame, sizeof( bool ) * I_KEY::COUNT );
	ZeroMemory( releaseFrame, sizeof( bool ) * I_KEY::COUNT );
	ZeroMemory( currentFrame, sizeof( bool ) * I_KEY::COUNT );
	ZeroMemory( previousFrame, sizeof( bool ) * I_KEY::COUNT );
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
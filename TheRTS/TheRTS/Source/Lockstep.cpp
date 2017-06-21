#include "Lockstep.h"

HRESULT Lockstep::Initialize()
{
	currentFrame = 0;
	ZeroMemory( &playerHasSubmitted, sizeof( playerHasSubmitted ) );
	ZeroMemory( &playerInputData, sizeof( playerInputData ) );

	return S_OK;
}

void Lockstep::Release()
{

}

void Lockstep::Increment()
{
	for( int i = 0; i < GENERAL_MAX_PLAYERS; i ++ )
		playerHasSubmitted[i][currentFrame] = false;
	currentFrame = ( currentFrame + 1 ) % GA_SYNC_WINDOW_SIZE;
}

bool Lockstep::PlayerSubmittedFrame( UINT playerIndex )
{
	return playerHasSubmitted[playerIndex][currentFrame];
}

bool Lockstep::KeyDown( UINT player, I_KEY::Keys key )
{
	return playerInputData[player][currentFrame].keyState[key];
}

bool Lockstep::KeyPressed( UINT player, I_KEY::Keys key )
{
	return false;
}

bool Lockstep::KeyReleased( UINT player, I_KEY::Keys key )
{
	return false;
}
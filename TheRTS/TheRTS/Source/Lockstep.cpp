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

bool Lockstep::PlayerSubmittedFrame( UINT playerIndex_ )
{
	return playerHasSubmitted[playerIndex_][currentFrame];
}

bool Lockstep::KeyDown( UINT player_, I_KEY::Keys key_ )
{
	return playerInputData[player_][currentFrame].keyState[key_];
}

bool Lockstep::KeyPressed( UINT player_, I_KEY::Keys key_ )
{
	return	playerInputData[player_][currentFrame].keyState[key_] && 
			!playerInputData[player_][( currentFrame + GA_SYNC_WINDOW_SIZE - 1 ) % GA_SYNC_WINDOW_SIZE].keyState[key_];
}

bool Lockstep::KeyReleased( UINT player_, I_KEY::Keys key_ )
{
	return	!playerInputData[player_][currentFrame].keyState[key_] && 
			playerInputData[player_][( currentFrame + GA_SYNC_WINDOW_SIZE - 1 ) % GA_SYNC_WINDOW_SIZE].keyState[key_];
}
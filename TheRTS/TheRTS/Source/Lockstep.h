#ifndef LOCKSTEP_H
#define LOCKSTEP_H

#include "Definitions.h"
#include "Input.h"

#define Lockstep_KeyDown( player, key )		Lockstep::GetInstance()->KeyDown( player, key )
#define Lockstep_KeyPressed( player, key )	Lockstep::GetInstance()->KeyPressed( player, key )
#define Lockstep_KeyReleased( player, key ) Lockstep::GetInstance()->KeyReleased( player, key )

struct InputData
{
	bool keyState[I_KEY::COUNT];
};

class Lockstep
{
	//Variables
	public:
		UINT		currentFrame;
		bool		playerHasSubmitted[GENERAL_MAX_PLAYERS][GA_SYNC_WINDOW_SIZE];
		InputData	playerInputData[GENERAL_MAX_PLAYERS][GA_SYNC_WINDOW_SIZE];

	private:

	//Functions
	public:
		HRESULT	Initialize();
		void	Release();

		static Lockstep* GetInstance()
		{
			static Lockstep* instance = new Lockstep();
			return instance;
		}

				Lockstep( Lockstep const& )		= delete;
		void	operator=( Lockstep const& )	= delete;

	private:
		Lockstep() {}

	public:
		void	Increment();
		bool	PlayerSubmittedFrame( UINT playerIndex );
		bool	KeyDown( UINT player, I_KEY::Keys key );
		bool	KeyPressed( UINT player, I_KEY::Keys key );
		bool	KeyReleased( UINT player, I_KEY::Keys key );
	private:
};
#endif
#ifndef GAME_H
#define GAME_H

#include "Network.h"
#include "EntityManager.h"
#include "Graphics.h"
#include "Input.h"
#include "Lockstep.h"

#define TEMP_MY_PLAYER_ID 0

class Game
{
	//Variables
	public:
	private:
		HWND windowHandle;
		LONG windowWidth;
		LONG windowHeight;

		EntityManager*	entityManager;
		Graphics*		graphicsManager;
		Network*		network;

		float gameTimeToProcess;

		UINT gameState;
		UINT nextGameState;

	//Functions
	public:
		HRESULT Initialize( HWND windowHandle, LONG windowWidth, LONG windowHeight );
		void	Release();

				Game();
				~Game();
	public:
		void	Update( float deltaTime );
		void	Render();

	private:
		void LoadAssets();
		void CreateResources();
		void CreateEntities();
};
#endif
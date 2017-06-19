#ifndef GAME_H
#define GAME_H

#include "Network.h"
#include "EntityManager.h"
#include "Graphics.h"
#include "Input.h"

#define GA_SYNC_WINDOW_SIZE 15
#define GA_NET_SYNC_TIME	0.2f

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
		bool			isGameHost			= false;
		bool			isSyncingNetwork	= false;
		std::thread		hostThread;
		std::thread		receiveThread;
		std::thread		sendThread;

		float			accumulatedTime			= 0.0f;
		float			accumulatedNetworkTime	= 0.0f;
		UINT			currentSyncedFrame		= 0;

		bool			playerFrameReceived[NE_MAX_SOCKETS][GA_SYNC_WINDOW_SIZE];
		bool			playerInputData[NE_MAX_SOCKETS][I_KEY::COUNT][GA_SYNC_WINDOW_SIZE];


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
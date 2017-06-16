#ifndef GAME_H
#define GAME_H

#include "Network.h"
#include "EntityManager.h"
#include "Graphics.h"
#include "Input.h"

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
		bool			isGameHost = false;
		std::thread		hostThread;
		std::thread		receiveThread;
		std::thread		sendThread;

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
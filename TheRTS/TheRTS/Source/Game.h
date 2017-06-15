#ifndef GAME_H
#define GAME_H

#include "EntityManager.h"
#include "Graphics.h"

class Game
{
	//Variables
	public:
	private:
		HWND windowHandle;
		LONG windowWidth;
		LONG windowHeight;

		EntityManager*	mainManager;
		Graphics*		graphicsManager;

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
};
#endif
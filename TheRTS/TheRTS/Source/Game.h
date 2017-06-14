#ifndef GAME_H
#define GAME_H

#include "EntityManager.h"
#include "Graphics.h"

class Game
{
	//Variables
	public:
	private:
		EntityManager* mainManager;

	//Functions
	public:
		HRESULT Initialize();
		void	Release();

				Game();
				~Game();
	public:
		void	Update( float deltaTime );
		void	Render();
	private:
};
#endif
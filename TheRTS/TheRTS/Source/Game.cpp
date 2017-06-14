#include "Game.h"	

HRESULT Game::Initialize()
{
	mainManager = new EntityManager;
	mainManager->Initialize();
}

void Game::Release()
{
	mainManager->Release();
	delete mainManager;
}

Game::Game()
{

}

Game::~Game()
{

}

void Game::Update( float deltaTime )
{
	mainManager->Update( deltaTime );
}

void Game::Render()
{
	mainManager->Render();
}
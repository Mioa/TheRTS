#include "Game.h"	

HRESULT Game::Initialize( HWND windowHandle_, LONG windowWidth_, LONG windowHeight_ )
{
	windowHandle = windowHandle_;
	windowWidth  = windowWidth_;
	windowHeight = windowHeight_;

	mainManager = new EntityManager;
	mainManager->Initialize();

	graphicsManager = new Graphics;
	graphicsManager->Initialize( windowHandle, windowWidth, windowHeight );

	return S_OK;
}

void Game::Release()
{
	mainManager->Release();
	delete mainManager;

	graphicsManager->Release();
	delete graphicsManager;
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
	graphicsManager->BeginScene();
	graphicsManager->EndScene();
}
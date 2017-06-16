#include "Game.h"	

#define macResourceManager graphicsManager->GetResourceManager()

HRESULT Game::Initialize( HWND windowHandle_, LONG windowWidth_, LONG windowHeight_ )
{
	windowHandle = windowHandle_;
	windowWidth  = windowWidth_;
	windowHeight = windowHeight_;

	entityManager = new EntityManager;
	entityManager->Initialize();

	graphicsManager = new Graphics;
	graphicsManager->Initialize( windowHandle, windowWidth, windowHeight );

	cameraSpeed = 0.001f;

	LoadAssets();
	CreateResources();
	CreateEntities();

	return S_OK;
}

void Game::Release()
{
	entityManager->Release();
	delete entityManager;

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
	// Temporary
	entityManager->keyStates.keyDown[0][I_KEY::W] = Input_KeyDown( I_KEY::W );
	entityManager->keyStates.keyDown[0][I_KEY::A] = Input_KeyDown( I_KEY::A );
	entityManager->keyStates.keyDown[0][I_KEY::S] = Input_KeyDown( I_KEY::S );
	entityManager->keyStates.keyDown[0][I_KEY::D] = Input_KeyDown( I_KEY::D );

	graphicsManager->UpdateCamera( DirectX::XMFLOAT4(
		( Input_KeyDown( I_KEY::ARROW_RIGHT ) ? cameraSpeed : 0.0f ) - ( Input_KeyDown( I_KEY::ARROW_LEFT ) ? cameraSpeed : 0.0f ),
		0.0f,
		( Input_KeyDown( I_KEY::ARROW_UP ) ? cameraSpeed : 0.0f ) - ( Input_KeyDown( I_KEY::ARROW_DOWN ) ? cameraSpeed : 0.0f ),
		0.0f
		) );
	//

	entityManager->Update( deltaTime );
}

void Game::Render()
{
	graphicsManager->BeginScene();
	entityManager->Render();
	graphicsManager->EndScene();
}

void Game::LoadAssets()
{
	macResourceManager->LoadStaticMesh( ASSET_CUBE, "cube.rtsa" );
	macResourceManager->LoadStaticMesh( ASSET_FLOOR, "floor.rtsa" );
}

void Game::CreateResources()
{
	macResourceManager->CreateStaticMesh( RES_SM_CUBE, ASSET_CUBE, 0 );
	macResourceManager->CreateStaticMesh( RES_SM_FLOOR, ASSET_FLOOR, 0 );
}

void Game::CreateEntities()
{
	UINT triangle = entityManager->AddEntity();
	entityManager->AddComponent( triangle, CI_Position{ DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) } );
	entityManager->AddComponent( triangle, CI_Mesh{ RES_SM_CUBE } );
	entityManager->AddComponent( triangle, CI_PlayerInput{ 0 } );

	UINT floor = entityManager->AddEntity();
	entityManager->AddComponent( floor, CI_Position{ DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) } );
	entityManager->AddComponent( floor, CI_Mesh{ RES_SM_FLOOR } );
}
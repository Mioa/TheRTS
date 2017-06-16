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

	LoadAssets();
	CreateResources();
	CreateEntities();

	network = new Network;
	network->Initialize();

	return S_OK;
}

void Game::Release()
{
	entityManager->Release();
	delete entityManager;

	graphicsManager->Release();
	delete graphicsManager;

	network->Release();
	delete network;
}

Game::Game()
{

}

Game::~Game()
{

}

void Game::Update( float deltaTime )
{
	entityManager->Update( deltaTime );

	if( Input_KeyPressed( I_KEY::H ) && !isGameHost )
	{
		hostThread = std::thread( &Network::StartHostSession, network );
		isGameHost = true;
	}

	if( Input_KeyPressed( I_KEY::J ) )
		network->AddConnection();


	if( receiveThread.joinable() )
		receiveThread.join();
	else
		receiveThread	= std::thread( &Network::ReceiveData, network );

	if( sendThread.joinable() )
		sendThread.join();
	else
		sendThread		= std::thread( &Network::SendData, network );
}

void Game::Render()
{
	graphicsManager->BeginScene();
	entityManager->Render();
	graphicsManager->EndScene();
}

void Game::LoadAssets()
{
	macResourceManager->LoadStaticMesh( ASSET_TRIANGLE, "this/is/a/filepath" );
}

void Game::CreateResources()
{
	macResourceManager->CreateStaticMesh( RES_SM_TRIANGLE, ASSET_TRIANGLE, 0 );
}

void Game::CreateEntities()
{
	UINT triangle = entityManager->AddEntity();
	entityManager->AddComponent( triangle, CI_Position{ DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ) } );
	entityManager->AddComponent( triangle, CI_Mesh{ RES_SM_TRIANGLE } );
}
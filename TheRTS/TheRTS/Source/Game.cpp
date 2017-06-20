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

	gameState		= STATE_GAME;
	nextGameState	= STATE_GAME;

	cameraSpeed = 0.005f;

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
	Input::GetInstance()->Update();
	Input::GetInstance()->Clear();

	// Temporary
	entityManager->keyStates.keyDown[0][I_KEY::W] = Input_KeyDown(I_KEY::W);
	entityManager->keyStates.keyDown[0][I_KEY::A] = Input_KeyDown(I_KEY::A);
	entityManager->keyStates.keyDown[0][I_KEY::S] = Input_KeyDown(I_KEY::S);
	entityManager->keyStates.keyDown[0][I_KEY::D] = Input_KeyDown(I_KEY::D);

	graphicsManager->UpdateCamera( DirectX::XMFLOAT4(
		( Input_KeyDown( I_KEY::ARROW_RIGHT ) ? cameraSpeed : 0.0f ) - ( Input_KeyDown( I_KEY::ARROW_LEFT ) ? cameraSpeed : 0.0f ),
		0.0f,
		( Input_KeyDown( I_KEY::ARROW_UP ) ? cameraSpeed : 0.0f ) - ( Input_KeyDown( I_KEY::ARROW_DOWN ) ? cameraSpeed : 0.0f ),
		0.0f
		) );
	//

	if( gameState != nextGameState )
	{
		gameState = nextGameState;
		entityManager->EntityStateChange( gameState );
	}

	// Input::GetInstance()->Update();
	// networkManager->Update();
	// networkManager->UpdateLockstep();
	 entityManager->Update( gameState );
	// entityManager->Render( gameState ); DONE IN Game::Render()
}

void Game::Render()
{
	graphicsManager->BeginScene();
	entityManager->Render( gameState );
	graphicsManager->EndScene();
}

void Game::LoadAssets()
{
	macResourceManager->LoadStaticMesh( ASSET_MESH_CUBE, "cube.rtsa" );
	macResourceManager->LoadStaticMesh( ASSET_MESH_SPHERE, "sphere.rtsa" );
	macResourceManager->LoadStaticMesh( ASSET_MESH_FLOOR, "floor.rtsa" );
	macResourceManager->LoadTexture( ASSET_TEXTURE_SPHERE, "cubeTexture.dds" );
	macResourceManager->LoadTexture( ASSET_TEXTURE_GUI, "gui.dds" );
}

void Game::CreateResources()
{
	macResourceManager->CreateStaticMesh( RES_SM_CUBE, ASSET_MESH_DEFAULT, ASSET_TEXTURE_GUI );
	macResourceManager->CreateStaticMesh( RES_SM_FLOOR, ASSET_MESH_FLOOR, ASSET_TEXTURE_SPHERE );
	macResourceManager->CreateStaticMesh( RES_SM_SPHERE, ASSET_MESH_SPHERE, ASSET_TEXTURE_DEFAULT );
	macResourceManager->CreateSprite( RES_SP_DEFAULT, ASSET_TEXTURE_DEFAULT );
	macResourceManager->CreateSprite( RES_SP_FLOWER, ASSET_TEXTURE_GUI );
}

void Game::CreateEntities()
{
	UINT player0 = entityManager->AddEntity();
	entityManager->AddComponent( player0, CI_Transform{ DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) } );
	entityManager->AddComponent( player0, CI_Mesh{ RES_SM_SPHERE } );
	entityManager->AddComponent( player0, CI_PlayerInput{ 0 } );

	UINT player1 = entityManager->AddEntity();
	entityManager->AddComponent( player1, CI_Transform{ DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) } );
	entityManager->AddComponent( player1, CI_Mesh{ RES_SM_SPHERE } );
	entityManager->AddComponent( player1, CI_PlayerInput{ 1 } );

	UINT floor = entityManager->AddEntity();
	entityManager->AddComponent( floor, CI_Transform{ DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) } );
	entityManager->AddComponent( floor, CI_Mesh{ RES_SM_FLOOR } );

	for( UINT x = 0; x < 20; x++ )
	{
		for( UINT z = 0; z < 20; z++ )
		{
			UINT cube = entityManager->AddEntity();
			entityManager->AddComponent( 
				cube, 
				CI_Transform{ 
					DirectX::XMFLOAT4( (float)x, 0.0f, (float)z, 1.0f ), 
					DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ),
					DirectX::XMFLOAT4( 0.7f, 0.7f, 0.7f, 1.0f )
					}
				);
			entityManager->AddComponent( cube, CI_Mesh{ RES_SM_CUBE } );
		}
	}

	for( UINT i = 0; i < 201; i++)
	{
		UINT sprite = entityManager->AddEntity();
		entityManager->AddComponent( sprite, CI_Position{ DirectX::XMFLOAT4( i * 4.8f, 550.0f, 3.0f, 50.0f ) } );
		entityManager->AddComponent( sprite, CI_Texture{ RES_SP_DEFAULT } );
	}

	UINT sprite2 = entityManager->AddEntity();
	entityManager->AddComponent( sprite2, CI_Position{ DirectX::XMFLOAT4( 0.0f, 0.0f, 200.0f, 100.0f ) } );
	entityManager->AddComponent( sprite2, CI_Texture{ RES_SP_FLOWER } );
}
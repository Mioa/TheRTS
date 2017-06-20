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

	entityManager->Update();
}

void Game::Render()
{
	graphicsManager->BeginScene();
	entityManager->Render();
	graphicsManager->EndScene();
}

void Game::LoadAssets()
{
	macResourceManager->LoadStaticMesh( ASSET_MESH_CUBE, "cube.rtsa" );
	macResourceManager->LoadStaticMesh( ASSET_MESH_SPHERE, "sphere.rtsa" );
	macResourceManager->LoadStaticMesh( ASSET_MESH_FLOOR, "floor.rtsa" );
	macResourceManager->LoadTexture( ASSET_TEXTURE_SPHERE, "cubeTexture.dds" );
}

void Game::CreateResources()
{
	macResourceManager->CreateStaticMesh( RES_SM_CUBE, ASSET_MESH_DEFAULT, ASSET_TEXTURE_DEFAULT );
	macResourceManager->CreateStaticMesh( RES_SM_FLOOR, ASSET_MESH_FLOOR, ASSET_TEXTURE_SPHERE );
	macResourceManager->CreateStaticMesh( RES_SM_SPHERE, ASSET_MESH_SPHERE, ASSET_TEXTURE_DEFAULT );
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
}
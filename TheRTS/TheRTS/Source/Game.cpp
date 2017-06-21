#include "Game.h"	

#define macResourceManager graphicsManager->GetResourceManager()

HRESULT Game::Initialize( HWND windowHandle_, LONG windowWidth_, LONG windowHeight_ )
{
	windowHandle = windowHandle_;
	windowWidth  = windowWidth_;
	windowHeight = windowHeight_;

	entityManager = new EntityManager;
	entityManager->Initialize();
	entityManager->UpdateWindowSize( windowWidth, windowHeight );

	graphicsManager = new Graphics;
	graphicsManager->Initialize( windowHandle, windowWidth, windowHeight );

	gameTimeToProcess = 0.0f;

	gameState		= STATE_MAIN_MENU;
	nextGameState	= STATE_MAIN_MENU;

	LoadAssets();
	CreateResources();
	CreateEntities();

	Lockstep::GetInstance()->Initialize();

	network = new Network;
	network->Initialize();

	entityManager->EntityStateChange( gameState );

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

	if( (gameState == STATE_GAME || gameState == STATE_GAME_MENU) && Input_KeyPressed( I_KEY::ESC ) )
		nextGameState = ( gameState == STATE_GAME ) ? STATE_GAME_MENU : STATE_GAME;

	if( gameState != nextGameState )
	{
		gameState = nextGameState;
		entityManager->EntityStateChange( gameState );
	}

	if( !Lockstep::GetInstance()->PlayerSubmittedFrame( TEMP_MY_PLAYER_ID ) )
	{
		for( int i = 0; i < I_KEY::COUNT; i++ )
			Lockstep::GetInstance()->playerInputData[TEMP_MY_PLAYER_ID][Lockstep::GetInstance()->currentFrame].keyState[i] = Input::GetInstance()->currentFrame[i];
		Lockstep::GetInstance()->playerHasSubmitted[TEMP_MY_PLAYER_ID][Lockstep::GetInstance()->currentFrame] = true;
	}

	gameTimeToProcess += deltaTime;

	if( gameTimeToProcess > EM_TIME_STEP )
	{
		gameTimeToProcess -= EM_TIME_STEP;

		bool frameIsReady = true;
		for( int i = 0; i < 1; i++ )
			if( !Lockstep::GetInstance()->PlayerSubmittedFrame( i ) )
			{
				frameIsReady = false;
				break;
			}

		if( frameIsReady )
		{
			// Input::GetInstance()->Update();
			// networkManager->Update();
			// networkManager->UpdateLockstep();
			entityManager->UpdateLockstep( gameState );
			// entityManager->Render( gameState ); DONE IN Game::Render()

			//std::cout << "Frame: " << Lockstep::GetInstance()->currentFrame << '\n';
			Lockstep::GetInstance()->Increment();
		}
	}

	entityManager->UpdateUnlocked( gameState );
	graphicsManager->UpdateCamera();

	if( nextGameState == STATE_EXIT)
		PostQuitMessage( 0 );
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

	macResourceManager->LoadTexture( ASSET_TEXTURE_NEWGAME_CLICKED, "NewGame_Clicked.dds" );
	macResourceManager->LoadTexture( ASSET_TEXTURE_NEWGAME_HOVER, "NewGame_Hover.dds" );
	macResourceManager->LoadTexture( ASSET_TEXTURE_NEWGAME_DEFAULT, "NewGame_Default.dds" );

	macResourceManager->LoadTexture( ASSET_TEXTURE_QUIT_CLICKED, "Quit_Clicked.dds" );
	macResourceManager->LoadTexture( ASSET_TEXTURE_QUIT_HOVER, "Quit_Hover.dds" );
	macResourceManager->LoadTexture( ASSET_TEXTURE_QUIT_DEFAULT, "Quit_Default.dds" );
}

void Game::CreateResources()
{
	macResourceManager->CreateStaticMesh( RES_SM_CUBE, ASSET_MESH_DEFAULT, ASSET_TEXTURE_GUI );
	macResourceManager->CreateStaticMesh( RES_SM_FLOOR, ASSET_MESH_FLOOR, ASSET_TEXTURE_SPHERE );
	macResourceManager->CreateStaticMesh( RES_SM_SPHERE, ASSET_MESH_SPHERE, ASSET_TEXTURE_DEFAULT );
	macResourceManager->CreateSprite( RES_SP_DEFAULT, ASSET_TEXTURE_DEFAULT );
	macResourceManager->CreateSprite( RES_SP_FLOWER, ASSET_TEXTURE_GUI );

	macResourceManager->CreateSprite( RES_SP_BUTTON_NEWGAME_CLICKED,	ASSET_TEXTURE_NEWGAME_CLICKED );
	macResourceManager->CreateSprite( RES_SP_BUTTON_NEWGAME_HOVER,		ASSET_TEXTURE_NEWGAME_HOVER );
	macResourceManager->CreateSprite( RES_SP_BUTTON_NEWGAME_DEFAULT,	ASSET_TEXTURE_NEWGAME_DEFAULT );

	macResourceManager->CreateSprite( RES_SP_BUTTON_QUIT_CLICKED,	ASSET_TEXTURE_QUIT_CLICKED );
	macResourceManager->CreateSprite( RES_SP_BUTTON_QUIT_HOVER,		ASSET_TEXTURE_QUIT_HOVER );
	macResourceManager->CreateSprite( RES_SP_BUTTON_QUIT_DEFAULT,	ASSET_TEXTURE_QUIT_DEFAULT );
}

void Game::CreateEntities()
{
	//########################################################################################
	//##									MAIN MENU										##
	//########################################################################################


	UINT newGameButton = entityManager->AddEntity( STATE_MAIN_MENU );
	entityManager->AddComponent( newGameButton, CI_Position{ DirectX::XMFLOAT4( 330.0f, 240.0f, 300.0f, 60.0f ) } );
	entityManager->AddComponent( newGameButton, CI_Texture{ RES_SP_BUTTON_NEWGAME_DEFAULT } );
	entityManager->AddComponent( newGameButton, CI_Button{ MAIN_MENU_ACTION_NEWGAME, RES_SP_BUTTON_NEWGAME_CLICKED, RES_SP_BUTTON_NEWGAME_HOVER, RES_SP_BUTTON_NEWGAME_DEFAULT } );
	entityManager->AddComponent( newGameButton, CI_StateTransition{ &nextGameState } );

	UINT quitAppButton = entityManager->AddEntity( STATE_MAIN_MENU );
	entityManager->AddComponent( quitAppButton, CI_Position{ DirectX::XMFLOAT4( 330.0f, 300.0f, 300.0f, 60.0f ) } );
	entityManager->AddComponent( quitAppButton, CI_Texture{ RES_SP_BUTTON_QUIT_DEFAULT } );
	entityManager->AddComponent( quitAppButton, CI_Button{ MAIN_MENU_ACTION_QUIT, RES_SP_BUTTON_QUIT_CLICKED, RES_SP_BUTTON_QUIT_HOVER, RES_SP_BUTTON_QUIT_DEFAULT } );
	entityManager->AddComponent( quitAppButton, CI_StateTransition{ &nextGameState } );


	//########################################################################################
	//##									GAME MENU										##
	//########################################################################################


	UINT quitGameButton = entityManager->AddEntity( STATE_GAME_MENU );
	entityManager->AddComponent( quitGameButton, CI_Position{ DirectX::XMFLOAT4( 330.0f, 270.0f, 300.0f, 60.0f ) } );
	entityManager->AddComponent( quitGameButton, CI_Texture{ RES_SP_BUTTON_QUIT_DEFAULT } );
	entityManager->AddComponent( quitGameButton, CI_Button{ GAME_MENU_ACTION_QUIT, RES_SP_BUTTON_QUIT_CLICKED, RES_SP_BUTTON_QUIT_HOVER, RES_SP_BUTTON_QUIT_DEFAULT } );
	entityManager->AddComponent( quitGameButton, CI_StateTransition{ &nextGameState } );


	//########################################################################################
	//##									   GAME		  								    ##
	//########################################################################################


	UINT player0 = entityManager->AddEntity( STATE_GAME | STATE_GAME_MENU );
	entityManager->AddComponent( player0, CI_Transform{ DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) } );
	entityManager->AddComponent( player0, CI_Mesh{ RES_SM_SPHERE } );
	entityManager->AddComponent( player0, CI_PlayerInput{ 0 } );
	entityManager->AddComponent( player0, CI_UnitMovement{ 0.1f, DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) } );

	UINT player1 = entityManager->AddEntity( STATE_GAME | STATE_GAME_MENU );
	entityManager->AddComponent( player1, CI_Transform{ DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) } );
	entityManager->AddComponent( player1, CI_Mesh{ RES_SM_SPHERE } );
	entityManager->AddComponent( player1, CI_PlayerInput{ 1 } );

	UINT floor = entityManager->AddEntity( STATE_GAME | STATE_GAME_MENU );
	entityManager->AddComponent( floor, CI_Transform{ DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) } );
	entityManager->AddComponent( floor, CI_Mesh{ RES_SM_FLOOR } );

	for( UINT x = 0; x < 20; x++ )
	{
		for( UINT z = 0; z < 20; z++ )
		{
			UINT cube = entityManager->AddEntity( STATE_GAME | STATE_GAME_MENU );
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
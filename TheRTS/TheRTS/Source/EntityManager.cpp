#include "EntityManager.h"
#include "SigFunctionsLockstep.h"

HRESULT EntityManager::Initialize()
{
	entity			= new Entity[EM_MAX_ENTITIES];
	ZeroMemory( entity, sizeof( Entity ) * EM_MAX_ENTITIES );

	button			= new C_Button[EM_MAX_ENTITIES];
	mesh			= new C_Mesh[EM_MAX_ENTITIES];
	playerInput		= new C_PlayerInput[EM_MAX_ENTITIES];
	position		= new C_Position[EM_MAX_ENTITIES];
	spriteTexture	= new C_SpriteTexture[EM_MAX_ENTITIES];
	stateTransition	= new C_StateTransition[EM_MAX_ENTITIES];
	transform		= new C_Transform[EM_MAX_ENTITIES];
	unitMovement	= new C_UnitMovement[EM_MAX_ENTITIES];

	ZeroMemory( button, sizeof( C_Button ) * EM_MAX_ENTITIES );
	ZeroMemory( mesh, sizeof( C_Mesh ) * EM_MAX_ENTITIES );
	ZeroMemory( playerInput, sizeof( C_PlayerInput ) * EM_MAX_ENTITIES );
	ZeroMemory( position, sizeof( C_Position ) * EM_MAX_ENTITIES );
	ZeroMemory( spriteTexture, sizeof( C_SpriteTexture ) * EM_MAX_ENTITIES );
	ZeroMemory( stateTransition, sizeof( C_StateTransition ) * EM_MAX_ENTITIES );
	ZeroMemory( transform, sizeof( C_Transform ) * EM_MAX_ENTITIES );
	ZeroMemory( unitMovement, sizeof( C_UnitMovement ) * EM_MAX_ENTITIES );

	//ZeroMemory( &keyStates, sizeof( PlayerKeystates ) );

	lockstepSignatures.push_back( new SL_MovePlayer( this ) );
	lockstepSignatures.push_back( new SL_UnitMovePosition( this ) );
	lockstepSignatures.push_back( new SL_UnitTargetPosition( this ) );

	renderSignatures.push_back( new SR_RenderMesh( this ) );
	renderSignatures.push_back( new SR_RenderSprite( this ) );

	updateSignatures.push_back( new SU_MainMenuButton( this ) );
	updateSignatures.push_back( new SU_GameMenuButton( this ) );

	return S_OK;
}

void EntityManager::Release()
{
	delete[] entity;

	delete[] button;
	delete[] mesh;
	delete[] playerInput;
	delete[] position;
	delete[] spriteTexture;
	delete[] stateTransition;
	delete[] transform;
	delete[] unitMovement;

	for( UINT i = 0; i < lockstepSignatures.size(); i++ )
		if( lockstepSignatures[i] ) delete lockstepSignatures[i];

	for( UINT i = 0; i < renderSignatures.size(); i++ )
		if( renderSignatures[i] ) delete renderSignatures[i];

	for( UINT i = 0; i < updateSignatures.size(); i++ )
		if( updateSignatures[i] ) delete updateSignatures[i];
}

EntityManager::EntityManager() {}

EntityManager::~EntityManager() {}

void EntityManager::EntityStateChange( UINT gameState_ )
{
	for( UINT i = 0; i < EM_MAX_ENTITIES; i++ )
	{
		entity[i].resting = (( entity[i].states & gameState_ ) == gameState_) ? false : true;
	}
}

void EntityManager::UpdateLockstep( UINT gameState_ )
{
	// Update
	for( int i = 0; i < lockstepSignatures.size(); i++ )
		if( ( lockstepSignatures[i]->states & gameState_ ) == gameState_ )
			lockstepSignatures[i]->Function();
}

void EntityManager::Render( UINT gameState_ )
{
	for( int i = 0; i < renderSignatures.size(); i++ )
		if( ( renderSignatures[i]->states & gameState_ ) == gameState_ )
			renderSignatures[i]->Function();
}

void EntityManager::UpdateUnlocked( UINT gameState_ )
{
	// MouseRay
	Input* input = Input::GetInstance();
	
	float mouseNDC[2] = {
			( ( (float)input->currentMousePos[0] / (float)windowWidth ) * 2.0f ) - 1.0f,
			( ( (float)-input->currentMousePos[1] / (float)windowHeight ) * 2.0f ) + 1.0f
		};
	
	XMVECTOR mouseNear = XMVectorSet( mouseNDC[0], mouseNDC[1], 0.0f, 1.0f );
	XMVECTOR mouseFar  = XMVectorSet( mouseNDC[0], mouseNDC[1], 1.0f, 1.0f );
	
	XMMATRIX combinedMtx = XMMatrixTranspose( XMLoadFloat4x4( &input->camera->projection ) * XMLoadFloat4x4( &input->camera->view ) );
	XMMATRIX combinedInv = XMMatrixInverse( 0, combinedMtx );
	
	XMVECTOR transNear = XMVector3TransformCoord(
		mouseNear, 
		combinedInv
		);

	XMVECTOR transFar = XMVector3TransformCoord(
		mouseFar, 
		combinedInv
		);

	ToXMF4( mouseRay.origin, transNear );
	ToXMF4( mouseRay.direction, XMVector3Normalize( transFar - transNear ) );

	// Update
	for( int i = 0; i < updateSignatures.size(); i++ )
		if( ( updateSignatures[i]->states & gameState_ ) == gameState_ )
			updateSignatures[i]->Function();
}

void EntityManager::UpdateWindowSize( LONG width_, LONG height )
{
	windowWidth  = width_;
	windowHeight = height;
}

int EntityManager::AddEntity( UINT gameState_)
{
	int localRet = -1;
	if( firstAvailableEntityIndex >= 0 )
	{

		entity[firstAvailableEntityIndex].active = true;
		entity[firstAvailableEntityIndex].states = gameState_;
		numActiveEntities++;
		localRet = firstAvailableEntityIndex;

		for( int i = firstAvailableEntityIndex; i < EM_MAX_ENTITIES; i++ )
		{
			if( !entity[i].active )
			{
				firstAvailableEntityIndex = i;
				break;
			}
		}
		if( firstAvailableEntityIndex == localRet )
			firstAvailableEntityIndex =  -1;
	}

	return localRet;
}

HRESULT EntityManager::AddComponent( UINT entityIndex_, CI_Button info_ )
{
	entity[entityIndex_].active					= true;
	entity[entityIndex_].resting				= false;
	entity[entityIndex_].signature[C_BUTTON]	= true;
	button[entityIndex_].action					= info_.action;
	button[entityIndex_].resourceClicked		= info_.resourceClicked;
	button[entityIndex_].resourceHover			= info_.resourceHover;
	button[entityIndex_].resourceDefault		= info_.resourceDefault;

	return S_OK;
}

HRESULT EntityManager::AddComponent( UINT entityIndex_, CI_Mesh info_ )
{
	entity[entityIndex_].active					= true;
	entity[entityIndex_].resting				= false;
	entity[entityIndex_].signature[C_MESH]		= true;
	mesh[entityIndex_].resource					= info_.resource;

	return S_OK;
}

HRESULT EntityManager::AddComponent( UINT entityIndex_, CI_PlayerInput info_ )
{
	entity[entityIndex_].active						= true;
	entity[entityIndex_].resting					= false;
	entity[entityIndex_].signature[C_PLAYERINPUT]	= true;
	playerInput[entityIndex_].playerIndex			= info_.playerIndex;

	return S_OK;
}

HRESULT EntityManager::AddComponent( UINT entityIndex_, CI_Position info_ )
{
	entity[entityIndex_].active					= true;
	entity[entityIndex_].resting				= false;
	entity[entityIndex_].signature[C_POSITION]	= true;
	position[entityIndex_].position				= info_.position;

	return S_OK;
}

HRESULT EntityManager::AddComponent( UINT entityIndex_, CI_SpriteTexture info_ )
{
	entity[entityIndex_].active							= true;
	entity[entityIndex_].resting						= false;
	entity[entityIndex_].signature[C_SPRITE_TEXTURE]	= true;
	spriteTexture[entityIndex_].resource				= info_.resource;
	spriteTexture[entityIndex_].depth					= info_.depth;

	return S_OK;
}

HRESULT EntityManager::AddComponent( UINT entityIndex_, CI_StateTransition info_ )
{
	entity[entityIndex_].active							= true;
	entity[entityIndex_].resting						= false;
	entity[entityIndex_].signature[C_STATE_TRANSITION]	= true;
	stateTransition[entityIndex_].stateReference		= info_.stateReference;

	return S_OK;
}

HRESULT EntityManager::AddComponent( UINT entityIndex_, CI_Transform info_ )
{
	entity[entityIndex_].active					= true;
	entity[entityIndex_].resting				= false;
	entity[entityIndex_].signature[C_TRANSFORM]	= true;
	transform[entityIndex_].position			= info_.position;
	transform[entityIndex_].rotation			= info_.rotation;
	transform[entityIndex_].scale				= info_.scale;

	return S_OK;
}

HRESULT EntityManager::AddComponent( UINT entityIndex_, CI_UnitMovement info_ )
{
	entity[entityIndex_].active						= true;
	entity[entityIndex_].resting					= false;
	entity[entityIndex_].signature[C_UNITMOVEMENT]	= true;
	unitMovement[entityIndex_].speed				= info_.speed;
	unitMovement[entityIndex_].position				= info_.position;

	return S_OK;
}
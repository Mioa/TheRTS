#ifndef SIGNATURE_FUNCTIONS_H
#define SIGNATURE_FUNCTIONS_H

#include "Definitions.h"
#include "EntityManager.h"
#include "RenderQueue.h"
#include "Lockstep.h"

#define macRenderQueue RenderQueue::GetInstance()

struct SignatureFunction
{
	EntityManager*			manager;
	std::bitset<C_COUNT>	signature;
	
	UINT					numActive;
	UINT					currentActive[EM_MAX_ENTITIES];

	virtual void Function() = 0;
};

struct SR_RenderMesh : public SignatureFunction
{
	SR_RenderMesh( EntityManager* manager_ )
	{
		manager					= manager_;
		signature[C_TRANSFORM]	= true;
		signature[C_MESH]		= true;
	}
	void Function()
	{
		numActive = 0;

		for( UINT entID = 0; entID < EM_MAX_ENTITIES; entID++ )
			if( manager->entity[entID].active && ( signature & manager->entity[entID].signature ) == signature )
				currentActive[numActive++] = entID;

		for( UINT i = 0; i < numActive; i++ )
		{
			UINT entID = currentActive[i];

			macRenderQueue->RenderStaticMesh( 
				manager->mesh[entID].resource, 
				manager->transform[entID].position,
				manager->transform[entID].rotation,
				manager->transform[entID].scale
				);
		}
	}
};

struct SR_RenderSprite : public SignatureFunction
{
	SR_RenderSprite( EntityManager* manager_ )
	{
		manager					= manager_;
		signature[C_POSITION]	= true;
		signature[C_TEXTURE]	= true;
	}
	void Function()
	{
		numActive = 0;

		for( UINT entID = 0; entID < EM_MAX_ENTITIES; entID++ )
			if( manager->entity[entID].active && ( signature & manager->entity[entID].signature ) == signature )
				currentActive[numActive++] = entID;

		for( UINT i = 0; i < numActive; i++ )
		{
			UINT entID = currentActive[i];

			macRenderQueue->RenderSprite( 
				manager->texture[entID].resource, 
				manager->position[entID].position
				);
		}
	}
};

struct SU_MovePlayer : public SignatureFunction
{
	SU_MovePlayer( EntityManager* manager_ )
	{
		manager						= manager_;
		signature[C_TRANSFORM]		= true;
		signature[C_MESH]			= true;
		signature[C_PLAYERINPUT]	= true;
	}
	void Function()
	{
		numActive = 0;

		for( UINT entID = 0; entID < EM_MAX_ENTITIES; entID++ )
			if( manager->entity[entID].active && ( signature & manager->entity[entID].signature ) == signature )
				currentActive[numActive++] = entID;

		C_PlayerInput*	pIn			= manager->playerInput;
		C_Transform*	transform	= manager->transform;

		for( UINT i = 0; i < numActive; i++ )
		{
			UINT entID = currentActive[i];

			// Temporary
			if( Lockstep_KeyDown( pIn[entID].playerIndex, I_KEY::W ) )
				transform[entID].position.z = transform[entID].position.z + 0.005f;
																			 
			if( Lockstep_KeyDown( pIn[entID].playerIndex, I_KEY::A ) ) 
				transform[entID].position.x = transform[entID].position.x - 0.005f;
																								 
			if( Lockstep_KeyDown( pIn[entID].playerIndex, I_KEY::S ) )	 
				transform[entID].position.z = transform[entID].position.z - 0.005f;
																								 
			if( Lockstep_KeyDown( pIn[entID].playerIndex, I_KEY::D ) )	 
				transform[entID].position.x = transform[entID].position.x + 0.005f;
			//
		}
	}
};

#endif
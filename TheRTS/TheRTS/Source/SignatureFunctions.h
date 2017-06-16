#ifndef SIGNATURE_FUNCTIONS_H
#define SIGNATURE_FUNCTIONS_H

#include "Definitions.h"
#include "EntityManager.h"
#include "RenderQueue.h"

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
		signature[C_POSITION]	= true;
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
		signature[C_POSITION]		= true;
		signature[C_MESH]			= true;
		signature[C_PLAYERINPUT]	= true;
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

			// Temporary
			if( manager->keyStates.keyDown[manager->playerInput[entID].playerIndex][I_KEY::W] )
				manager->position[entID].position.y = min(1.0f, manager->position[entID].position.y + 0.001f);

			if( manager->keyStates.keyDown[manager->playerInput[entID].playerIndex][I_KEY::A] )
				manager->position[entID].position.x = max(-1.0f, manager->position[entID].position.x - 0.001f);

			if( manager->keyStates.keyDown[manager->playerInput[entID].playerIndex][I_KEY::S] )
				manager->position[entID].position.y = max(-1.0f, manager->position[entID].position.y - 0.001f);

			if( manager->keyStates.keyDown[manager->playerInput[entID].playerIndex][I_KEY::D] )
				manager->position[entID].position.x = min(1.0f, manager->position[entID].position.x + 0.001f);
			//
		}
	}
};

#endif
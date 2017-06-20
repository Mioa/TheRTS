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
	UINT					states;
	
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
		states					= STATE_GAME;
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
		states					= STATE_GAME;
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

struct SU_HUDClicked : public SignatureFunction
{
	SU_HUDClicked( EntityManager* manager_ )
	{
		manager					= manager_;
		signature[C_POSITION]	= true;
		signature[C_TEXTURE]	= true;
		states					= STATE_GAME;
	}
	void Function()
	{
		if( Input_KeyDown(I_KEY::MOUSE_LEFT) )
		{
			numActive = 0;

			for( UINT entID = 0; entID < EM_MAX_ENTITIES; entID++ )
				if( manager->entity[entID].active && ( signature & manager->entity[entID].signature ) == signature )
					currentActive[numActive++] = entID;

			int mousePos[2] = { Input::GetInstance()->mousePos[0], Input::GetInstance()->mousePos[1] };

			for( UINT i = 0; i < numActive; i++ )
			{
				UINT entID = currentActive[i];
				DirectX::XMFLOAT4 pos = manager->position[entID].position;

				if ( mousePos[0] > pos.x && mousePos[0] < pos.x + pos.z && mousePos[1] > pos.y && mousePos[1] < pos.y + pos.w )
					manager->texture[entID].resource = RES_SP_FLOWER;
			}
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
		states						= STATE_GAME;
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
				manager->transform[entID].position.z = manager->transform[entID].position.z + 0.005f;
																			 
			if( manager->keyStates.keyDown[manager->playerInput[entID].playerIndex][I_KEY::A] )	 
				manager->transform[entID].position.x = manager->transform[entID].position.x - 0.005f;
																								 
			if( manager->keyStates.keyDown[manager->playerInput[entID].playerIndex][I_KEY::S] )	 
				manager->transform[entID].position.z = manager->transform[entID].position.z - 0.005f;
																								 
			if( manager->keyStates.keyDown[manager->playerInput[entID].playerIndex][I_KEY::D] )	 
				manager->transform[entID].position.x = manager->transform[entID].position.x + 0.005f;
			//
		}
	}
};

#endif
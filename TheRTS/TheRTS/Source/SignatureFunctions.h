#ifndef SIGNATURE_FUNCTIONS_H
#define SIGNATURE_FUNCTIONS_H

#include "Definitions.h"
#include "EntityManager.h"
#include "RenderQueue.h"

#define macRenderQueue RenderQueue::GetInstance()

using namespace DirectX;

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
		if( Input_KeyPressed(I_KEY::MOUSE_LEFT) )
		{
			numActive = 0;

			for( UINT entID = 0; entID < EM_MAX_ENTITIES; entID++ )
				if( manager->entity[entID].active && ( signature & manager->entity[entID].signature ) == signature )
					currentActive[numActive++] = entID;

			int mousePos[2] = { Input::GetInstance()->mousePos[0], Input::GetInstance()->mousePos[1] };

			for( UINT i = 0; i < numActive; i++ )
			{
				UINT entID = currentActive[i];
				XMFLOAT4 pos = manager->position[entID].position;

				if ( mousePos[0] >= pos.x && mousePos[0] <= pos.x + pos.z && mousePos[1] >= pos.y && mousePos[1] <= pos.y + pos.w )
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

struct SU_UnitTargetPosition : public SignatureFunction
{
	SU_UnitTargetPosition( EntityManager* manager_ )
	{
		manager								= manager_;
		signature[C_TRANSFORM]				= true;
		signature[C_PLAYERINPUT]			= true;
		signature[C_UNITMOVEMENT]	= true;
		states								= STATE_GAME;
	}
	void Function()
	{
		if( Input_KeyPressed(I_KEY::MOUSE_RIGHT) )
		{
			numActive = 0;

			for( UINT entID = 0; entID < EM_MAX_ENTITIES; entID++ )
				if( manager->entity[entID].active && ( signature & manager->entity[entID].signature ) == signature )
					currentActive[numActive++] = entID;

			for( UINT i = 0; i < numActive; i++ )
			{
				UINT entID = currentActive[i];

				float denom = XMVectorGetX( XMVector4Dot( XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ), ToXMV4( manager->mouseRay.direction ) ) );
				if (abs(denom) > 0.0001f)
				{
					float t = XMVectorGetX( XMVector4Dot( XMVectorNegate( ToXMV4( manager->mouseRay.origin ) ), XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) ) ) / denom;
					if (t >= 0)
					{
						manager->unitMovement[entID].moving   = true;
						XMStoreFloat4( &manager->unitMovement[entID].position, ToXMV4( manager->mouseRay.origin ) + ( ToXMV4( manager->mouseRay.direction ) * t ) );
					}
				}
			}
		}
	}
};

struct SU_UnitMovePosition : public SignatureFunction
{
	SU_UnitMovePosition( EntityManager* manager_ )
	{
		manager								= manager_;
		signature[C_TRANSFORM]				= true;
		signature[C_PLAYERINPUT]			= true;
		signature[C_UNITMOVEMENT]			= true;
		states								= STATE_GAME;
	}
	void Function()
	{
		numActive = 0;

		for( UINT entID = 0; entID < EM_MAX_ENTITIES; entID++ )
			if( manager->entity[entID].active && ( signature & manager->entity[entID].signature ) == signature )
				currentActive[numActive++] = entID;

		C_UnitMovement* mov = manager->unitMovement;
		C_Transform*	transform = manager->transform;

		for( UINT i = 0; i < numActive; i++ )
		{
			UINT entID = currentActive[i];

			if( mov[entID].moving == true )
			{
				XMVECTOR diffVec = XMLoadFloat4( &mov[entID].position ) - XMLoadFloat4( &transform[entID].position );

				if( XMVectorGetX( XMVector3LengthSq( diffVec ) ) < 0.0002f )
				{
					mov[entID].moving = false;
				}
				else
				{
					XMVECTOR movement = XMVector3Normalize( diffVec ) * 0.01f;
					XMStoreFloat4( &transform[entID].position, XMLoadFloat4( &transform[entID].position ) + movement );
				}
			}
		}
	}
};

#endif
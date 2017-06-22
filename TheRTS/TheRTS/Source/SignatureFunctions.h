#ifndef SIGNATURE_FUNCTIONS_H
#define SIGNATURE_FUNCTIONS_H

#include "Definitions.h"
#include "EntityManager.h"
#include "RenderQueue.h"
#include "Lockstep.h"

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

struct SL_MovePlayer : public SignatureFunction
{
	SL_MovePlayer( EntityManager* manager_ )
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
			if( manager->entity[entID].active && !manager->entity[entID].resting && ( signature & manager->entity[entID].signature ) == signature )
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

struct SL_UnitTargetPosition : public SignatureFunction
{
	SL_UnitTargetPosition( EntityManager* manager_ )
	{
		manager						= manager_;
		signature[C_TRANSFORM]		= true;
		signature[C_PLAYERINPUT]	= true;
		signature[C_UNITMOVEMENT]	= true;
		states						= STATE_GAME;
	}
	void Function()
	{
		if( Lockstep_KeyPressed( manager->playerInput->playerIndex, I_KEY::MOUSE_RIGHT) )
		{
			numActive = 0;

			for( UINT entID = 0; entID < EM_MAX_ENTITIES; entID++ )
				if( manager->entity[entID].active && !manager->entity[entID].resting && ( signature & manager->entity[entID].signature ) == signature )
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

struct SL_UnitMovePosition : public SignatureFunction
{
	SL_UnitMovePosition( EntityManager* manager_ )
	{
		manager								= manager_;
		signature[C_TRANSFORM]				= true;
		signature[C_PLAYERINPUT]			= true;
		signature[C_UNITMOVEMENT]			= true;
		states								= STATE_GAME | STATE_GAME_MENU;
	}
	void Function()
	{
		numActive = 0;

		for( UINT entID = 0; entID < EM_MAX_ENTITIES; entID++ )
			if( manager->entity[entID].active && !manager->entity[entID].resting && ( signature & manager->entity[entID].signature ) == signature )
				currentActive[numActive++] = entID;

		C_UnitMovement* mov = manager->unitMovement;
		C_Transform*	transform = manager->transform;

		for( UINT i = 0; i < numActive; i++ )
		{
			UINT entID = currentActive[i];

			if( mov[entID].moving == true )
			{
				XMVECTOR diffVec = XMLoadFloat4( &mov[entID].position ) - XMLoadFloat4( &transform[entID].position );

				if( XMVectorGetX( XMVector3LengthSq( diffVec ) ) < manager->unitMovement[entID].speed * manager->unitMovement[entID].speed )
				{
					mov[entID].moving = false;
				}
				else
				{
					XMVECTOR movement = XMVector3Normalize( diffVec ) * manager->unitMovement[entID].speed;
					XMStoreFloat4( &transform[entID].position, XMLoadFloat4( &transform[entID].position ) + movement );
				}
			}
		}
	}
};


struct SR_RenderMesh : public SignatureFunction
{
	SR_RenderMesh( EntityManager* manager_ )
	{
		manager					= manager_;
		signature[C_TRANSFORM]	= true;
		signature[C_MESH]		= true;
		states					= STATE_GAME | STATE_GAME_MENU | STATE_PAUSE;
	}
	void Function()
	{
		numActive = 0;

		for( UINT entID = 0; entID < EM_MAX_ENTITIES; entID++ )
			if( manager->entity[entID].active && !manager->entity[entID].resting && ( signature & manager->entity[entID].signature ) == signature )
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
		signature[C_SPRITE_TEXTURE]	= true;
		states					= STATE_MAIN_MENU | STATE_GAME | STATE_GAME_MENU | STATE_PAUSE;
	}
	void Function()
	{
		numActive = 0;

		for( UINT entID = 0; entID < EM_MAX_ENTITIES; entID++ )
			if( manager->entity[entID].active && !manager->entity[entID].resting && ( signature & manager->entity[entID].signature ) == signature )
				currentActive[numActive++] = entID;

		for( UINT i = 0; i < numActive; i++ )
		{
			UINT entID = currentActive[i];

			macRenderQueue->RenderSprite( 
				manager->spriteTexture[entID].resource, 
				manager->spriteTexture[entID].depth,
				manager->position[entID].position
				);
		}
	}
};

struct SU_MenuButton : public SignatureFunction
{
	SU_MenuButton( EntityManager* manager_ )
	{
		manager					= manager_;
		signature[C_POSITION]	= true;
		signature[C_SPRITE_TEXTURE]	= true;
		signature[C_BUTTON]		= true;
		states					= STATE_MAIN_MENU | STATE_GAME_MENU;
	}
	void Function()
	{
		numActive = 0;

		for( UINT entID = 0; entID < EM_MAX_ENTITIES; entID++ )
			if( manager->entity[entID].active && !manager->entity[entID].resting && ( signature & manager->entity[entID].signature ) == signature )
				currentActive[numActive++] = entID;

		bool lmbDown = Input_KeyDown(I_KEY::MOUSE_LEFT);
		bool lmbReleased = Input_KeyReleased(I_KEY::MOUSE_LEFT);
		int mousePos[2] = { Input::GetInstance()->currentMousePos[0], Input::GetInstance()->currentMousePos[1] };

		for( UINT i = 0; i < numActive; i++ )
		{
			UINT entID							= currentActive[i];
			XMFLOAT4 pos						= manager->position[entID].position;

			C_Button* button					= &manager->button[entID];
			C_SpriteTexture* spriteTexture		= &manager->spriteTexture[entID];
			C_StateTransition* stateTransition	= &manager->stateTransition[entID];

			if ( mousePos[0] >= pos.x && mousePos[0] <= pos.x + pos.z && mousePos[1] >= pos.y && mousePos[1] <= pos.y + pos.w )
			{
				if( lmbReleased )
				{				
					spriteTexture->resource = button->resourceDefault;
					if( ( manager->entity[entID].states & STATE_MAIN_MENU ) == STATE_MAIN_MENU )
					{
						switch( button->action )
						{
							case MAIN_MENU_ACTION_NEWGAME:
							{
								*stateTransition->stateReference = STATE_GAME;
								break;
							}
							case MAIN_MENU_ACTION_QUIT:
							{
								*stateTransition->stateReference = STATE_EXIT;
								break;
							}
						}
					}
					else if( ( manager->entity[entID].states & STATE_GAME_MENU ) == STATE_GAME_MENU )
					{
						switch( button->action )
						{
							case GAME_MENU_ACTION_QUIT:
							{
								*stateTransition->stateReference = STATE_MAIN_MENU;
								break;
							}
						}
					}
				}
				else if( lmbDown )
				{
					spriteTexture->resource = button->resourceClicked;
				}
				else
				{
					spriteTexture->resource = button->resourceHover;
				}
			}
			else
			{
				spriteTexture->resource = button->resourceDefault;
			}
		}
	}
};

#endif
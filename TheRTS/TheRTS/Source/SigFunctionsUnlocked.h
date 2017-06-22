#ifndef UNLOCKED_SIGNATURE_FUNCTIONS_H
#define UNLOCKED_SIGNATURE_FUNCTIONS_H

#include "Definitions.h"
#include "EntityManager.h"

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



struct SU_MainMenuButton : public SignatureFunction
{
	SU_MainMenuButton( EntityManager* manager_ )
	{
		manager						= manager_;
		signature[C_POSITION]		= true;
		signature[C_SPRITE_TEXTURE]	= true;
		signature[C_BUTTON]			= true;
		states						= STATE_MAIN_MENU;
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



struct SU_GameMenuButton : public SignatureFunction
{
	SU_GameMenuButton( EntityManager* manager_ )
	{
		manager						= manager_;
		signature[C_POSITION]		= true;
		signature[C_SPRITE_TEXTURE]	= true;
		signature[C_BUTTON]			= true;
		states						= STATE_GAME_MENU;
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
					switch( button->action )
					{
						case GAME_MENU_ACTION_QUIT:
						{
							*stateTransition->stateReference = STATE_MAIN_MENU;
							break;
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
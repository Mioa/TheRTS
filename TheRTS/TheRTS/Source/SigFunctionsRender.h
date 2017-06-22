#ifndef RENDER_SIGNATURE_FUNCTIONS_H
#define RENDER_SIGNATURE_FUNCTIONS_H

#include "Definitions.h"
#include "EntityManager.h"
#include "RenderQueue.h"
#include "SigFunctionsUnlocked.h"

//struct SignatureFunction
//{
//	EntityManager*			manager;
//	std::bitset<C_COUNT>	signature;
//	UINT					states;
//	
//	UINT					numActive;
//	UINT					currentActive[EM_MAX_ENTITIES];
//
//	virtual void Function() = 0;
//};



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
		manager						= manager_;
		signature[C_POSITION]		= true;
		signature[C_SPRITE_TEXTURE]	= true;
		states						= STATE_MAIN_MENU | STATE_GAME | STATE_GAME_MENU | STATE_PAUSE;
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

#endif
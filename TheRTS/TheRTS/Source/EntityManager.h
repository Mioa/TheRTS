#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <bitset>
#include "Components.h"
#include "Definitions.h"
#include "Input.h"
#include <vector>

struct SignatureFunction; //Forward Declare

struct PlayerKeystates
{
	bool keyDown[MAX_PLAYERS][I_KEY::COUNT];
	bool keyPressed[MAX_PLAYERS][I_KEY::COUNT];
	bool keyRelease[MAX_PLAYERS][I_KEY::COUNT];
};

class EntityManager
{
	private:
		struct Entity
		{
			bool									active;
			bool									resting;
			std::bitset<ComponentTypes::C_COUNT>	signature;
		};


	//Variables
	public:
		Entity*				entity		= nullptr;
		C_Position*			position	= nullptr;
		C_Transform*		transform	= nullptr;
		C_Mesh*				mesh		= nullptr;
		C_PlayerInput*		playerInput	= nullptr;
		C_Texture*			texture	= nullptr;

		PlayerKeystates keyStates;

	private:
		int				firstAvailableEntityIndex	= 0;
		UINT			numActiveEntities			= 0;

		std::vector<SignatureFunction*>	updateSignatures;
		std::vector<SignatureFunction*>	renderSignatures;


	//Functions
	public:
		HRESULT	Initialize();
		void	Release();

				EntityManager();
				~EntityManager();
	public:
		void	Update( float deltaTime );
		void	Render();

		int		AddEntity();
		HRESULT	AddComponent( UINT entityIndex_, CI_Position info_ );
		HRESULT AddComponent( UINT entityIndex_, CI_Transform info_ );
		HRESULT	AddComponent( UINT entityIndex_, CI_Mesh info_ );
		HRESULT AddComponent( UINT entityIndex_, CI_PlayerInput info_ );
		HRESULT AddComponent( UINT entityIndex_, CI_Texture info_ );

	private:
};
#endif
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <bitset>
#include "Components.h"
#include "Definitions.h"
#include "Input.h"
#include <vector>

struct SignatureFunction; //Forward Declare

//struct PlayerKeystates
//{
//	bool keyDown[GENERAL_MAX_PLAYERS][I_KEY::COUNT];
//	bool keyPressed[GENERAL_MAX_PLAYERS][I_KEY::COUNT];
//	bool keyRelease[GENERAL_MAX_PLAYERS][I_KEY::COUNT];
//};

class EntityManager
{
	private:
		struct Entity
		{
			bool									active;
			bool									resting;
			std::bitset<ComponentTypes::C_COUNT>	signature;
			UINT									states;
		};

		struct MouseRay
		{
			DirectX::XMFLOAT4 origin	= DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
			DirectX::XMFLOAT4 direction	= DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
		};


	//Variables
	public:
		Entity*					entity			= nullptr;

		C_Button*				button			= nullptr;
		C_Mesh*					mesh			= nullptr;
		C_PlayerInput*			playerInput		= nullptr;
		C_Position*				position		= nullptr;
		C_SpriteTexture*		spriteTexture	= nullptr;
		C_StateTransition*		stateTransition = nullptr;
		C_Transform*			transform		= nullptr;
		C_UnitMovement*			unitMovement	= nullptr;

		//PlayerKeystates keyStates;
		MouseRay mouseRay;

	private:
		int				firstAvailableEntityIndex	= 0;
		UINT			numActiveEntities			= 0;
		LONG			windowWidth					= 0;
		LONG			windowHeight				= 0;

		std::vector<SignatureFunction*>	lockstepSignatures;
		std::vector<SignatureFunction*>	renderSignatures;
		std::vector<SignatureFunction*>	updateSignatures;


	//Functions
	public:
		HRESULT	Initialize();
		void	Release();

				EntityManager();
				~EntityManager();

	public:
		void	EntityStateChange( UINT gameState_ );
		void	UpdateLockstep( UINT gameState_ );
		void	Render( UINT gameState_ );
		void	UpdateUnlocked( UINT gameState_ );
		void	UpdateWindowSize( LONG width_, LONG height );

		int		AddEntity( UINT gameState_ );

		HRESULT AddComponent( UINT entityIndex_, CI_Button info_ );
		HRESULT	AddComponent( UINT entityIndex_, CI_Mesh info_ );
		HRESULT AddComponent( UINT entityIndex_, CI_PlayerInput info_ );
		HRESULT	AddComponent( UINT entityIndex_, CI_Position info_ );
		HRESULT AddComponent( UINT entityIndex_, CI_SpriteTexture info_ );
		HRESULT AddComponent( UINT entityIndex_, CI_StateTransition info_ );
		HRESULT AddComponent( UINT entityIndex_, CI_Transform info_ );
		HRESULT	AddComponent( UINT entityIndex_, CI_UnitMovement info_ );

	private:
};
#endif
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <bitset>
#include "Components.h"
#include "Definitions.h"
#include <vector>

struct SignatureFunction; //Forward Declare

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
		Entity*			entity		= nullptr;
		C_Position*		position	= nullptr;
		C_Mesh*			mesh		= nullptr;
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
		HRESULT	AddComponent( UINT entityIndex_, CI_Mesh info_ );
	private:
};
#endif
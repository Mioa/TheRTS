#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <bitset>
#include "Components.h"

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
	private:
		Entity*			entitiy;
		C_Position*		position;
		C_Mesh*			mesh;

	//Functions
	public:
		HRESULT	Initialize();
		void	Release();

				EntityManager();
				~EntityManager();
	public:
		int		AddEntity();
		HRESULT	AddComponent( UINT entityIndex, CI_Position info );
		HRESULT	AddComponent( UINT entityIndex, CI_Mesh info );
	private:
};
#endif
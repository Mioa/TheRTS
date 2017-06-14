#include "EntityManager.h"
#include "SignatureFunctions.h"

HRESULT EntityManager::Initialize()
{
	entity		= new Entity[EM_MAX_ENTITIES];
	ZeroMemory( entity, sizeof( Entity ) * EM_MAX_ENTITIES );
	position	= new C_Position[EM_MAX_ENTITIES];
	ZeroMemory( position, sizeof( C_Position ) * EM_MAX_ENTITIES );
	mesh		= new C_Mesh[EM_MAX_ENTITIES];
	ZeroMemory( mesh, sizeof( C_Mesh ) * EM_MAX_ENTITIES );

	renderSignatures.push_back( new SR_RenderMesh( this ) );

	return S_OK;
}

void EntityManager::Release()
{
	delete[] entity;
	delete[] position;
	delete[] mesh;
}

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{

}

void EntityManager::Update( float deltaTime )
{

}

void EntityManager::Render()
{
	for( int i = 0; i < renderSignatures.size(); i++ )
		renderSignatures[i]->Function();
}

int EntityManager::AddEntity()
{
	int localRet = -1;
	if( firstAvailableEntityIndex >= 0 )
	{

		entity[firstAvailableEntityIndex].active = true;
		numActiveEntities++;
		localRet = firstAvailableEntityIndex;

		for( int i = firstAvailableEntityIndex; i < EM_MAX_ENTITIES; i++ )
		{
			if( !entity[i].active )
			{
				firstAvailableEntityIndex = i;
				break;
			}
		}
		if( firstAvailableEntityIndex == localRet )
			firstAvailableEntityIndex =  -1;
	}

	return localRet;
}

HRESULT EntityManager::AddComponent( UINT entityIndex_, CI_Position info_ )
{
	entity[entityIndex_].active		= true;
	entity[entityIndex_].resting	= false;
	position[entityIndex_].position = info_.position;

	return S_OK;
}

HRESULT EntityManager::AddComponent( UINT entityIndex_, CI_Mesh info_ )
{
	entity[entityIndex_].active		= true;
	entity[entityIndex_].resting	= false;
	mesh[entityIndex_].resource		= info_.resource;

	return S_OK;
}
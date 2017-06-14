#ifndef SIGNATURE_FUNCTIONS_H
#define SIGNATURE_FUNCTIONS_H

#include "Definitions.h"
#include "EntityManager.h"

struct SignatureFunction
{
	EntityManager*			manager;
	std::bitset<C_COUNT>	signature;
	
	UINT					numAlive;
	UINT					currentAlive[EM_MAX_ENTITIES];

	virtual void Function() = 0;
};

struct SR_RenderMesh : public SignatureFunction
{
	SR_RenderMesh( EntityManager* manager_ )
	{
		manager					= manager_;
		signature[C_POSITION]	= true;
		signature[C_MESH]		= true;
	}
	void Function()
	{

	}
};

#endif
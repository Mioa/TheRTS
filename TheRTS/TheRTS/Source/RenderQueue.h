#ifndef RENDERQUEUE_H
#define RENDERQUEUE_H

#include "Datastructures.h"
#include "Definitions.h"
#include <Windows.h>

class RenderQueue
{
	//Variables
	public:
		UINT spriteIndex		= 0;
		UINT staticMeshIndex	= 0;
		UINT dynamicMeshIndex	= 0;
		UINT pointLightIndex	= 0;
		UINT dirLightIndex		= 0;

		RI_Sprite*		sprites			= new RI_Sprite[RQ_MAX_SPRITE];
		RI_StaticMesh*	staticMeshes	= new RI_StaticMesh[RQ_MAX_STATIC_MESH];
		RI_DynamicMesh*	dynamicMeshes	= new RI_DynamicMesh[RQ_MAX_DYNAMIC_MESH];
		RI_PointLight*	pointLights		= new RI_PointLight[RQ_MAX_POINT_LIGHT];
		RI_DirLight*	dirLights		= new RI_DirLight[RQ_MAX_DIRECTIONAL_LIGHT];
	private:

	//Functions
	public:
		static RenderQueue* GetInstance()
		{
			static RenderQueue* instance = new RenderQueue();
			return instance;
		}

				RenderQueue( RenderQueue const& )	= delete;
		void	operator=( RenderQueue const& )		= delete;

	private:
		RenderQueue() {}

	public:
		void RenderSprite( UINT resourceID_, DirectX::XMFLOAT4 pos_, DirectX::XMFLOAT4 rot_, DirectX::XMFLOAT4 scale_ );
		void RenderStaticMesh( UINT resourceID_, DirectX::XMFLOAT4 pos_, DirectX::XMFLOAT4 rot_, DirectX::XMFLOAT4 scale_ );
		void RenderDynamicMesh( UINT resourceID_, DirectX::XMFLOAT4 pos_, DirectX::XMFLOAT4 rot_, DirectX::XMFLOAT4 scale_ );
		void RenderPointLight( DirectX::XMFLOAT4 pos_, DirectX::XMFLOAT4 color_, DirectX::XMFLOAT4 radius_ );
		void RenderDirLight( DirectX::XMFLOAT4 direction_, DirectX::XMFLOAT4 color_ );
		void ResetQueue();

};
#endif
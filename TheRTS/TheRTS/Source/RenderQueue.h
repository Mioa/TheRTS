#ifndef RENDERQUEUE_H
#define RENDERQUEUE_H

#include "Datastructures.h"
#include "Definitions.h"
#include <Windows.h>

class RenderQueue
{
	//Variables
	public:
		UINT* spriteCount;
		UINT* staticMeshCount;
		UINT* dynamicMeshCount;
		UINT  pointLightCount;
		UINT  dirLightCount;
		
		RI_Sprite**			sprites;
		RI_StaticMesh**		staticMeshes;
		RI_DynamicMesh**	dynamicMeshes;
		RI_PointLight*		pointLights;
		RI_DirLight*		dirLights;

	private:

	//Functions
	public:
		HRESULT	Initialize();
		void	Release();

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
		void RenderSprite( 
			UINT resourceID_, 
			DirectX::XMFLOAT4 pos_ = DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f )
			);

		void RenderStaticMesh( 
			UINT resourceID_, 
			DirectX::XMFLOAT4 pos_   = DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ), 
			DirectX::XMFLOAT4 rot_   = DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
			DirectX::XMFLOAT4 scale_ = DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f )
			);

		void RenderDynamicMesh( 
			UINT resourceID_, 
			DirectX::XMFLOAT4 pos_   = DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ), 
			DirectX::XMFLOAT4 rot_   = DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
			DirectX::XMFLOAT4 scale_ = DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f )
			);

		void RenderPointLight( 
			DirectX::XMFLOAT4 pos_    = DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ), 
			DirectX::XMFLOAT4 color_  = DirectX::XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f ), 
			DirectX::XMFLOAT4 radius_ = DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) 
			);

		void RenderDirLight( 
			DirectX::XMFLOAT4 direction_ = DirectX::XMFLOAT4( 0.0f, -1.0f, 0.0f, 0.0f ), 
			DirectX::XMFLOAT4 color_     = DirectX::XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f )
			);

		void ResetQueue();

};
#endif




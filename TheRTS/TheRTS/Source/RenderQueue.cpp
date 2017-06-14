#include "RenderQueue.h"

using namespace DirectX;

HRESULT	RenderQueue::Initialize()
{
	sprites			= new RI_Sprite[RQ_MAX_SPRITE];
	staticMeshes	= new RI_StaticMesh[RQ_MAX_STATIC_MESH];
	dynamicMeshes	= new RI_DynamicMesh[RQ_MAX_DYNAMIC_MESH];
	pointLights		= new RI_PointLight[RQ_MAX_POINT_LIGHT];
	dirLights		= new RI_DirLight[RQ_MAX_DIRECTIONAL_LIGHT];
}

void RenderQueue::Release()
{
	delete[] sprites;
	delete[] staticMeshes;
	delete[] dynamicMeshes;
	delete[] pointLights;
	delete[] dirLights;
}

void RenderQueue::RenderSprite( 
	UINT resourceID_, 
	XMFLOAT4 pos_	= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
	XMFLOAT4 rot_	= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
	XMFLOAT4 scale_ = XMFLOAT4( 1.0f, 1.0f, 1.0f, 0.0f ) 
	)
{
	XMMATRIX transformation = XMMatrixTransformation(
		XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f ),
		XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ),
		XMLoadFloat4( &scale_ ),
		XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ),
		XMLoadFloat4( &rot_ ),
		XMLoadFloat4( &pos_ )
	);

	sprites[spriteIndex].id	= resourceID_;
	XMStoreFloat4x4( &sprites[spriteIndex].transform, transformation );
}

void RenderQueue::RenderStaticMesh( 
	UINT resourceID_, 
	XMFLOAT4 pos_	= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
	XMFLOAT4 rot_	= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
	XMFLOAT4 scale_ = XMFLOAT4( 1.0f, 1.0f, 1.0f, 0.0f ) 
	)
{
	XMMATRIX transformation = XMMatrixTransformation(
		XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f ),
		XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ),
		XMLoadFloat4( &scale_ ),
		XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ),
		XMLoadFloat4( &rot_ ),
		XMLoadFloat4( &pos_ )
	);

	sprites[spriteIndex].id	= resourceID_;
	XMStoreFloat4x4( &sprites[spriteIndex].transform, transformation );
}

void RenderQueue::RenderDynamicMesh( 
	UINT resourceID_, 
	XMFLOAT4 pos_	= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
	XMFLOAT4 rot_	= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
	XMFLOAT4 scale_ = XMFLOAT4( 1.0f, 1.0f, 1.0f, 0.0f ) 
	)
{
	XMMATRIX transformation = XMMatrixTransformation(
		XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f ),
		XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ),
		XMLoadFloat4( &scale_ ),
		XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ),
		XMLoadFloat4( &rot_ ),
		XMLoadFloat4( &pos_ )
	);

	sprites[spriteIndex].id	= resourceID_;
	XMStoreFloat4x4( &sprites[spriteIndex].transform, transformation );
}

void RenderQueue::RenderPointLight( 
	XMFLOAT4 pos_		= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
	XMFLOAT4 color_		= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
	XMFLOAT4 radius_	= XMFLOAT4( 1.0f, 1.0f, 1.0f, 0.0f ) 
	)
{
	pointLights[pointLightIndex].position	= pos_;
	pointLights[pointLightIndex].color		= color_;
	pointLights[pointLightIndex].radius		= radius_;
}

void RenderQueue::RenderDirLight( 
	XMFLOAT4 direction_ = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
	XMFLOAT4 color_		= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f )
	)
{
	dirLights[dirLightIndex].direction	= direction_;
	dirLights[dirLightIndex].color		= color_;
}

void RenderQueue::ResetQueue()
{
	spriteIndex			= 0;
	staticMeshIndex		= 0;
	dynamicMeshIndex	= 0;
	pointLightIndex		= 0;
	dirLightIndex		= 0;
}
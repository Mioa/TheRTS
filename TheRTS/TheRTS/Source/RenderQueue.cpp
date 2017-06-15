#include "RenderQueue.h"

using namespace DirectX;

HRESULT	RenderQueue::Initialize()
{
	ZeroMemory( spriteCount, sizeof( UINT ) * RES_SP_COUNT );
	ZeroMemory( staticMeshCount, sizeof( UINT ) * RES_SM_COUNT );
	ZeroMemory( dynamicMeshCount, sizeof( UINT ) * RES_DM_COUNT );
	pointLightCount = 0;
	dirLightCount   = 0;

	return S_OK;
}

void RenderQueue::Release()
{

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

	XMStoreFloat4x4( &sprites[resourceID_][spriteCount[resourceID_]].transform, transformation );
	spriteCount[resourceID_]++;
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

	XMStoreFloat4x4( &staticMeshes[resourceID_][staticMeshCount[resourceID_]].transform, transformation );
	staticMeshCount[resourceID_]++;
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

	XMStoreFloat4x4( &dynamicMeshes[resourceID_][dynamicMeshCount[resourceID_]].transform, transformation );
	dynamicMeshCount[resourceID_]++;
}

void RenderQueue::RenderPointLight( 
	XMFLOAT4 pos_		= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
	XMFLOAT4 color_		= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
	XMFLOAT4 radius_	= XMFLOAT4( 1.0f, 1.0f, 1.0f, 0.0f ) 
	)
{
	pointLights[pointLightCount].position	= pos_;
	pointLights[pointLightCount].color		= color_;
	pointLights[pointLightCount].radius		= radius_;

	pointLightCount++;
}

void RenderQueue::RenderDirLight( 
	XMFLOAT4 direction_ = XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ), 
	XMFLOAT4 color_		= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f )
	)
{
	dirLights[dirLightCount].direction	= direction_;
	dirLights[dirLightCount].color		= color_;
}

void RenderQueue::ResetQueue()
{
	ZeroMemory( spriteCount, sizeof( UINT ) * RES_SP_COUNT );
	ZeroMemory( staticMeshCount, sizeof( UINT ) * RES_SM_COUNT );
	ZeroMemory( dynamicMeshCount, sizeof( UINT ) * RES_DM_COUNT );
	pointLightCount = 0;
	dirLightCount   = 0;
}
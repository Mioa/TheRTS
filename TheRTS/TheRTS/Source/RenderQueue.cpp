#include "RenderQueue.h"

using namespace DirectX;

HRESULT	RenderQueue::Initialize()
{
	spriteCount			= new UINT[RES_SP_COUNT];
	staticMeshCount		= new UINT[RES_SM_COUNT];
	dynamicMeshCount	= new UINT[RES_DM_COUNT];

	ZeroMemory( spriteCount, sizeof( UINT ) * RES_SP_COUNT );
	ZeroMemory( staticMeshCount, sizeof( UINT ) * RES_SM_COUNT );
	ZeroMemory( dynamicMeshCount, sizeof( UINT ) * RES_DM_COUNT );
	pointLightCount = 0;
	dirLightCount   = 0;

	sprites = new RI_Sprite*[RES_SP_COUNT];
	for( UINT i = 0; i < RES_SP_COUNT; i++ )
	{
		sprites[i] = new RI_Sprite[RQ_MAX_SPRITE];
		ZeroMemory( sprites[i], sizeof( RI_Sprite ) * RQ_MAX_SPRITE );
	}

	staticMeshes = new RI_StaticMesh*[RES_SM_COUNT];
	for( UINT i = 0; i < RES_SM_COUNT; i++ )
	{
		staticMeshes[i] = new RI_StaticMesh[RQ_MAX_STATIC_MESH];
		ZeroMemory( staticMeshes[i], sizeof( RI_StaticMesh ) * RQ_MAX_STATIC_MESH );
	}

	dynamicMeshes = new RI_DynamicMesh*[RES_DM_COUNT];
	for( UINT i = 0; i < RES_DM_COUNT; i++ )
	{
		dynamicMeshes[i] = new RI_DynamicMesh[RQ_MAX_DYNAMIC_MESH];
		ZeroMemory( dynamicMeshes[i], sizeof( RI_DynamicMesh ) * RQ_MAX_DYNAMIC_MESH );
	}

	pointLights = new RI_PointLight[RQ_MAX_POINT_LIGHT];
	dirLights = new RI_DirLight[RQ_MAX_DIRECTIONAL_LIGHT];

	ZeroMemory( pointLights, sizeof( RI_PointLight ) * RQ_MAX_POINT_LIGHT );
	ZeroMemory( dirLights, sizeof( RI_DirLight ) * RQ_MAX_DIRECTIONAL_LIGHT );

	return S_OK;
}

void RenderQueue::Release()
{
	delete spriteCount;
	delete staticMeshCount;
	delete dynamicMeshCount;

	for( UINT i = 0; i < RES_SP_COUNT; i++ )
		delete sprites[i];
	for( UINT i = 0; i < RES_SM_COUNT; i++ )
		delete staticMeshes[i];
	for( UINT i = 0; i < RES_DM_COUNT; i++ )
		delete dynamicMeshes[i];
	
	delete pointLights;
	delete dirLights;
}

void RenderQueue::RenderSprite( UINT resourceID_, XMFLOAT4 pos_ )
{
	sprites[resourceID_][spriteCount[resourceID_]].position = pos_;

	spriteCount[resourceID_]++;
}

void RenderQueue::RenderStaticMesh( UINT resourceID_, XMFLOAT4 pos_, XMFLOAT4 rot_, XMFLOAT4 scale_ )
{
	XMMATRIX transformation = XMMatrixTransformation(
		XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f ),
		XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ),
		XMLoadFloat4( &scale_ ),
		XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ),
		XMLoadFloat4( &rot_ ),
		XMLoadFloat4( &pos_ )
	);

	XMStoreFloat4x4( &staticMeshes[resourceID_][staticMeshCount[resourceID_]].transform, DirectX::XMMatrixTranspose( transformation ) );

	staticMeshCount[resourceID_]++;
}

void RenderQueue::RenderDynamicMesh( UINT resourceID_, XMFLOAT4 pos_, XMFLOAT4 rot_, XMFLOAT4 scale_ )
{
	XMMATRIX transformation = XMMatrixTransformation(
		XMVectorSet( 0.0f, 0.0f, 0.0f, 1.0f ),
		XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ),
		XMLoadFloat4( &scale_ ),
		XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f ),
		XMLoadFloat4( &rot_ ),
		XMLoadFloat4( &pos_ )
	);

	XMStoreFloat4x4( &dynamicMeshes[resourceID_][dynamicMeshCount[resourceID_]].transform, DirectX::XMMatrixTranspose( transformation ) );

	dynamicMeshCount[resourceID_]++;
}

void RenderQueue::RenderPointLight( XMFLOAT4 pos_, XMFLOAT4 color_, XMFLOAT4 radius_ )
{
	pointLights[pointLightCount].position	= pos_;
	pointLights[pointLightCount].color		= color_;
	pointLights[pointLightCount].radius		= radius_;

	pointLightCount++;
}

void RenderQueue::RenderDirLight( XMFLOAT4 direction_, XMFLOAT4 color_	)
{
	dirLights[dirLightCount].direction	= direction_;
	dirLights[dirLightCount].color		= color_;

	dirLightCount++;
}

void RenderQueue::ResetQueue()
{
	ZeroMemory( spriteCount, sizeof( UINT ) * RES_SP_COUNT );
	ZeroMemory( staticMeshCount, sizeof( UINT ) * RES_SM_COUNT );
	ZeroMemory( dynamicMeshCount, sizeof( UINT ) * RES_DM_COUNT );
	pointLightCount = 0;
	dirLightCount   = 0;
}
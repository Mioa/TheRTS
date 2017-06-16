#ifndef RESOURCES_H
#define RESOURCES_H

#include <Windows.h>

enum Assets
{
	ASSET_CUBE,
	ASSET_FLOOR,
	ASSET_COUNT
};

enum Res_Sprite
{
	RES_SP_DEFAULT, // To keep count higher than 0. Remove when resource is added.
	RES_SP_COUNT
};

enum Res_StaticMesh
{
	RES_SM_CUBE,
	RES_SM_FLOOR,
	RES_SM_COUNT
};

enum Res_DynamicMesh
{
	RES_DM_DEFAULT, // To keep count higher than 0. Remove when resource is added.
	RES_DM_COUNT
};



struct Resource
{

};

struct StaticMeshResource : public Resource
{
	UINT meshIndex	  = 0;
	UINT textureIndex = 0;

	StaticMeshResource( UINT meshIndex_, UINT textureIndex_ )
	{
		meshIndex    = meshIndex_;
		textureIndex = textureIndex_;
	}
};



struct RI_Sprite
{
	DirectX::XMFLOAT4X4 transform;
};

struct RI_StaticMesh
{
	DirectX::XMFLOAT4X4 transform;
};

struct RI_DynamicMesh
{
	DirectX::XMFLOAT4X4 transform;
};

struct RI_PointLight
{
	DirectX::XMFLOAT4 position	= { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 color		= { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 radius	= { 0.0f, 0.0f, 0.0f, 0.0f };
};

struct RI_DirLight
{
	DirectX::XMFLOAT4 direction = { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 color		= { 0.0f, 0.0f, 0.0f, 0.0f };
};

#endif

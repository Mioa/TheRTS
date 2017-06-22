#ifndef RESOURCES_H
#define RESOURCES_H

#include <Windows.h>

enum Assets
{
	ASSET_MESH_DEFAULT,

	ASSET_MESH_CUBE,
	ASSET_MESH_SPHERE,
	ASSET_MESH_FLOOR,

	ASSET_TEXTURE_DEFAULT_WHITE,
	ASSET_TEXTURE_DEFAULT_GRAY,
	ASSET_TEXTURE_DEFAULT_BLACK,
	ASSET_TEXTURE_DEFAULT_RED,
	ASSET_TEXTURE_DEFAULT_GREEN,
	ASSET_TEXTURE_DEFAULT_BLUE,

	ASSET_TEXTURE_SPHERE,
	ASSET_TEXTURE_GUI,

	ASSET_TEXTURE_NEWGAME_CLICKED,
	ASSET_TEXTURE_NEWGAME_HOVER,
	ASSET_TEXTURE_NEWGAME_DEFAULT,

	ASSET_TEXTURE_QUIT_CLICKED,
	ASSET_TEXTURE_QUIT_HOVER,
	ASSET_TEXTURE_QUIT_DEFAULT,

	ASSET_COUNT
};

enum ResSprite
{
	RES_SP_DEFAULT_WHITE,
	RES_SP_DEFAULT_GRAY,
	RES_SP_DEFAULT_BLACK,
	RES_SP_DEFAULT_RED,
	RES_SP_DEFAULT_GREEN,
	RES_SP_DEFAULT_BLUE,

	RES_SP_FLOWER,

	RES_SP_BUTTON_NEWGAME_CLICKED,
	RES_SP_BUTTON_NEWGAME_HOVER,
	RES_SP_BUTTON_NEWGAME_DEFAULT,

	RES_SP_BUTTON_QUIT_CLICKED,
	RES_SP_BUTTON_QUIT_HOVER,
	RES_SP_BUTTON_QUIT_DEFAULT,

	RES_SP_COUNT
};

enum ResStaticMesh
{
	RES_SM_CUBE,
	RES_SM_FLOOR,
	RES_SM_SPHERE,

	RES_SM_COUNT
};

enum ResDynamicMesh
{
	RES_DM_DEFAULT, // To keep count higher than 0. Replace when resource is added.

	RES_DM_COUNT
};

//########################################################################################
//##								 ResourceManager									##
//########################################################################################

struct StaticMeshBuffer
{
	ID3D11Buffer* buffer;
	UINT vertexCount;
};

struct Resource
{
	UINT test;
};

struct SpriteResource : public Resource
{
	UINT textureIndex = 0;

	SpriteResource( UINT textureIndex_ )
	{
		textureIndex = textureIndex_;
	}
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

//########################################################################################
//##								 RenderQueue										##
//########################################################################################

struct RI_Sprite
{
	DirectX::XMFLOAT4 position; // X, Y, Width, Height
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
	DirectX::XMFLOAT4 position	{ 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 color		{ 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 radius	{ 0.0f, 0.0f, 0.0f, 0.0f };
};

struct RI_DirLight
{
	DirectX::XMFLOAT4 direction { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 color		{ 0.0f, 0.0f, 0.0f, 0.0f };
};

#endif

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <DirectXMath.h>
#include "GraphicsDatastructures.h"
#include <Windows.h>

struct RI_Sprite
{
	UINT id	= 0;
	DirectX::XMFLOAT4X4 transform;

	RI_Sprite() {}

	RI_Sprite(UINT id, DirectX::XMFLOAT4X4 transform)
	{
		this->id		= id;
		this->transform = transform;
	}
};

struct RI_StaticMesh
{
	UINT id = 0;
	DirectX::XMFLOAT4X4 transform;

	RI_StaticMesh() {}

	RI_StaticMesh(UINT id, DirectX::XMFLOAT4X4 transform)
	{
		this->id		= id;
		this->transform = transform;
	}
};

struct RI_DynamicMesh
{
	UINT id = 0;
	DirectX::XMFLOAT4X4 transform;

	RI_DynamicMesh() {}

	RI_DynamicMesh(UINT id, DirectX::XMFLOAT4X4 transform)
	{
		this->id		= id;
		this->transform = transform;
	}
};

struct RI_DirLight
{
	DirectX::XMFLOAT4 direction = { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 color		= { 0.0f, 0.0f, 0.0f, 0.0f };

	RI_DirLight() {}

	RI_DirLight(DirectX::XMFLOAT4 direction, DirectX::XMFLOAT4 color)
	{
		this->direction = direction;
		this->color		= color;
	}
};

struct RI_PointLight
{
	DirectX::XMFLOAT4 position	= { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 color		= { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 radius	= { 0.0f, 0.0f, 0.0f, 0.0f };

	RI_PointLight() {}

	RI_PointLight(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 color, DirectX::XMFLOAT4 radius)
	{
		this->position	= position;
		this->color		= color;
		this->radius	= radius;
	}
};

#endif

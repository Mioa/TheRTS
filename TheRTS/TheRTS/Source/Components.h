#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <DirectXMath.h>
#include <Windows.h>

enum ComponentTypes
{
	C_POSITION,
	C_TRANSFORM,
	C_MESH,
	C_PLAYERINPUT,
	C_COUNT
};


struct C_Position
{
	DirectX::XMFLOAT4 position;
};

struct CI_Position
{
	DirectX::XMFLOAT4 position;
};

struct C_Transform
{
	DirectX::XMFLOAT4 position	{ 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 rotation	{ 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 scale		{ 1.0f, 1.0f, 1.0f, 1.0f };
};

struct CI_Transform
{
	DirectX::XMFLOAT4 position	{ 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMFLOAT4 rotation	{ 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMFLOAT4 scale		{ 1.0f, 1.0f, 1.0f, 1.0f };
};


struct C_Mesh
{
	UINT resource;
};

struct CI_Mesh
{
	UINT resource;
};


struct C_PlayerInput
{
	UINT playerIndex;
};

struct CI_PlayerInput
{
	UINT playerIndex;
};

#endif
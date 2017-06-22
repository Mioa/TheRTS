#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <DirectXMath.h>
#include <Windows.h>

enum ComponentTypes
{
	C_UNITMOVEMENT,
	C_POSITION,
	C_TRANSFORM,
	C_MESH,
	C_PLAYERINPUT,
	C_SPRITE_TEXTURE,
	C_BUTTON,
	C_STATE_TRANSITION,
	C_COUNT
};


struct C_UnitMovement
{
	bool moving = false;
	float speed = 0.0f;
	DirectX::XMFLOAT4 position;
};

struct CI_UnitMovement
{
	float speed;
	DirectX::XMFLOAT4 position;
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


struct C_SpriteTexture
{
	UINT resource;
	UINT depth;
};

struct CI_SpriteTexture
{
	UINT resource;
	UINT depth;
};


struct C_Button
{
	UINT action;
	UINT resourceClicked;
	UINT resourceHover;
	UINT resourceDefault;
};

struct CI_Button
{
	UINT action;
	UINT resourceClicked;
	UINT resourceHover;
	UINT resourceDefault;
};


struct C_StateTransition
{
	UINT* stateReference;
};

struct CI_StateTransition
{
	UINT* stateReference;
};

#endif
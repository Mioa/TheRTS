#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <DirectXMath.h>
#include <Windows.h>

enum ComponentTypes
{
	C_POSITION,
	C_MESH,
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


struct C_Mesh
{
	UINT resource;
};

struct CI_Mesh
{
	UINT resource;
};

#endif
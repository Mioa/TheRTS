#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <DirectXMath.h>
#include "GraphicsDatastructures.h"
#include <iostream>
#include "Resources.h"

enum GameStates
{
	STATE_LOBBY	= 1,
	STATE_GAME	= 2,
	STATE_MENU	= 4,
	STATE_PAUSE	= 8,
	STATE_EXIT	= 16
};

struct Camera
{
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

#endif

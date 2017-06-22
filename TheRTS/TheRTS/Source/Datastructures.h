#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <DirectXMath.h>
#include "GraphicsDatastructures.h"
#include <iostream>
#include "Resources.h"

#define AddXMF4( xmf1, xmf2 ) DirectX::XMFLOAT4( xmf1.x + xmf2.x, xmf1.y + xmf2.y, xmf1.z + xmf2.z, xmf1.w + xmf2.w )
#define ToXMV4( xmf ) DirectX::XMLoadFloat4( &xmf )
#define ToXMF4( xmf, xmv ) DirectX::XMStoreFloat4( &xmf, xmv )

enum GameStates
{
	STATE_MAIN_MENU	= 1,
	STATE_GAME	= 2,
	STATE_GAME_MENU	= 4,
	STATE_PAUSE	= 8,
	STATE_EXIT	= 16
};

enum MainMenuAction
{
	MAIN_MENU_ACTION_NEWGAME	= 1,
	MAIN_MENU_ACTION_QUIT		= 2
};

enum GameMenuAction
{
	GAME_MENU_ACTION_QUIT	= 1
};

enum SpriteDepths
{
	SP_DEPTH_0,
	SP_DEPTH_1,
	SP_DEPTH_2,
	SP_DEPTH_3,
	SP_DEPTH_4,
	SP_DEPTH_COUNT
};

struct Camera
{
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

#endif

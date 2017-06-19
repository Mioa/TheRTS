#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <DirectXMath.h>
#include "GraphicsDatastructures.h"
#include <iostream>
#include "Resources.h"

struct Camera
{
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

#endif

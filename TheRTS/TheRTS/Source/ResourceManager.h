#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Datastructures.h"
#include <vector>

class ResourceManager
{
	//Variables
	public:
		std::vector<ID3D11Buffer*> vertexBuffers;

	private:
		ID3D11Device* device;

	//Functions
	public:
		HRESULT	Initialize( ID3D11Device* device_ );
		void	Release();

				ResourceManager();
				~ResourceManager();

	public:
	private:
		HRESULT CreateVertexBuffer( UINT size, void* data );
};
#endif
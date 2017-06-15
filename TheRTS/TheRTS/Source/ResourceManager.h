#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Datastructures.h"
#include <vector>

class ResourceManager
{
	//Variables
	public:
		ID3D11ShaderResourceView*	textures[ASSET_COUNT];
		ID3D11Buffer*				meshes[ASSET_COUNT];

		static const UINT resourceCount = RES_SP_COUNT + RES_SM_COUNT + RES_DM_COUNT;
		Resource* resources[resourceCount];

	private:
		ID3D11Device* device;

	//Functions
	public:
		HRESULT	Initialize( ID3D11Device* device_ );
		void	Release();

				ResourceManager();
				~ResourceManager();

	public:
		void LoadStaticMesh(  UINT staticMeshIndex_, char* filePath_ );
		void LoadTexture(  UINT textureIndex_, char* filePath_ );

		void CreateStaticMesh( UINT resourceIndex_, UINT meshIndex_, UINT textureIndex_ );

	private:
		ID3D11Buffer* CreateVertexBuffer( UINT size, void* data );
};
#endif
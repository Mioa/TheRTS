#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "Datastructures.h"
#include <vector>

class ResourceManager
{
	//Variables
	public:
		ID3D11ShaderResourceView*	textures[ASSET_COUNT];
		StaticMeshBuffer			meshes[ASSET_COUNT];

		StaticMeshResource* smResources[RES_SM_COUNT];
		SpriteResource* spResources[RES_SP_COUNT];

	private:
		ID3D11Device* device;

	//Functions
	public:
		HRESULT	Initialize( ID3D11Device* device_ );
		void	Release();

				ResourceManager();
				~ResourceManager();

	public:
		void LoadStaticMesh(  UINT staticMeshIndex_, std::string filePath_ );
		void LoadTexture(  UINT textureIndex_, std::string filePath_ );

		void CreateSprite( UINT resourceIndex_, UINT textureIndex_ );
		void CreateStaticMesh( UINT resourceIndex_, UINT meshIndex_, UINT textureIndex_ );

	private:
		void InitDefaultData();
		ID3D11Buffer* CreateVertexBuffer( UINT size, void* data );
};
#endif
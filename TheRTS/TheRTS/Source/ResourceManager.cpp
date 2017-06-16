#include "ResourceManager.h"

HRESULT	ResourceManager::Initialize( ID3D11Device* device_ )
{
	ZeroMemory( meshes, sizeof( ID3D11Buffer* ) * ASSET_COUNT );
	ZeroMemory( textures, sizeof( ID3D11ShaderResourceView* ) * ASSET_COUNT );
	ZeroMemory( resources, sizeof( Resource* ) * ( RES_SP_COUNT + RES_SM_COUNT + RES_DM_COUNT ) );

	device = device_;

	return S_OK;
}

void ResourceManager::Release()
{
	for( UINT i = 0; i < ASSET_COUNT; i++ )
		if( meshes[i] ) meshes[i]->Release();

	for( UINT i = 0; i < ASSET_COUNT; i++ )
		if( textures[i] ) textures[i]->Release();

	for( UINT i = 0; i < RES_SP_COUNT + RES_SM_COUNT + RES_DM_COUNT; i++ )
		if( resources[i] != nullptr ) delete resources[i];
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::LoadStaticMesh( UINT staticMeshIndex_, char* filePath_ )
{
	// Temporary
	Vertex_POS3 vertices[3] = {
		DirectX::XMFLOAT3( 0.5f, -0.5f, 0.0f ),
		DirectX::XMFLOAT3( 0.0f, 0.5f, 0.0f ),
		DirectX::XMFLOAT3( -0.5f, -0.5f, 0.0f )
		};

	meshes[staticMeshIndex_] = CreateVertexBuffer( sizeof( Vertex_POS3 ) * 3, vertices );
	//
}

void ResourceManager::LoadTexture( UINT textureIndex_, char* filePath_ )
{
	textures[textureIndex_] = nullptr;
}

void ResourceManager::CreateStaticMesh( UINT resourceIndex_, UINT meshIndex_, UINT textureIndex_ )
{
	resources[resourceIndex_] = new StaticMeshResource( meshIndex_, textureIndex_ );
}

ID3D11Buffer* ResourceManager::CreateVertexBuffer( UINT size, void* data )
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory( &vertexDesc, sizeof(D3D11_BUFFER_DESC) );
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.Usage	 = D3D11_USAGE_DEFAULT;
	vertexDesc.ByteWidth = size;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = data;

	device->CreateBuffer( &vertexDesc, &vertexData, &buffer );

	return buffer;
}
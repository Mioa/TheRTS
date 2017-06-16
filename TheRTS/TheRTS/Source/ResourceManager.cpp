#include <fstream>
#include "ResourceManager.h"
#include <string>
//#include "../Libraries/DDSTextureLoader/DDSTextureLoader.h"

HRESULT	ResourceManager::Initialize( ID3D11Device* device_ )
{
	ZeroMemory( meshes, sizeof( StaticMeshBuffer ) * ASSET_COUNT );
	ZeroMemory( textures, sizeof( ID3D11ShaderResourceView* ) * ASSET_COUNT );
	ZeroMemory( resources, sizeof( Resource* ) * ( RES_SP_COUNT + RES_SM_COUNT + RES_DM_COUNT ) );

	device = device_;

	return S_OK;
}

void ResourceManager::Release()
{
	for( UINT i = 0; i < ASSET_COUNT; i++ )
		if( meshes[i].buffer ) meshes[i].buffer->Release();

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
	std::string path = std::string( "Assets/" ) + std::string( filePath_ );
	std::ifstream file( path.c_str(), std::ios::binary );
	
	int vertexCount;
	file.read( (char*)&vertexCount, sizeof( int ) );

	Vertex_POS3* vertices = new Vertex_POS3[vertexCount];
	file.read( (char*)vertices, sizeof( Vertex_POS3 ) * vertexCount );

	file.close();

	meshes[staticMeshIndex_].buffer			= CreateVertexBuffer( sizeof( Vertex_POS3 ) * vertexCount, vertices );
	meshes[staticMeshIndex_].vertexCount	= vertexCount;

	delete vertices;
}

void ResourceManager::LoadTexture( UINT textureIndex_, wchar_t* filePath_ )
{
	//std::wstring path = std::wstring( L"Assets/" ) + std::wstring( filePath_ );
	//
	//HRESULT hr = DirectX::CreateDDSTextureFromFile(
	//	device,
	//	path.c_str(),
	//	nullptr,
	//	&textures[textureIndex_]
	//	);
	//
	//	if (!SUCCEEDED(hr))
	//	{
	//		OutputDebugString(L"Could not load texture!");
	//		exit(-1);
	//	}
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
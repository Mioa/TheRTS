#include <fstream>
#include "ResourceManager.h"
#include <string>
#include "../Libraries/DDSTextureLoader/DDSTextureLoader.cpp"

HRESULT	ResourceManager::Initialize( ID3D11Device* device_ )
{
	ZeroMemory( meshes, sizeof( StaticMeshBuffer ) * ASSET_COUNT );
	ZeroMemory( textures, sizeof( ID3D11ShaderResourceView* ) * ASSET_COUNT );
	ZeroMemory( spResources, sizeof( SpriteResource* ) * RES_SP_COUNT );
	ZeroMemory( smResources, sizeof( StaticMeshResource* ) * RES_SM_COUNT );

	device = device_;

	InitDefaultData();

	return S_OK;
}

void ResourceManager::Release()
{
	for( UINT i = 0; i < ASSET_COUNT; i++ )
		if( meshes[i].buffer ) meshes[i].buffer->Release();

	for( UINT i = 0; i < ASSET_COUNT; i++ )
		if( textures[i] ) textures[i]->Release();

	for( UINT i = 0; i < RES_SP_COUNT; i++ )
		if( spResources[i] != nullptr ) delete spResources[i];

	for( UINT i = 0; i < RES_SM_COUNT; i++ )
		if( smResources[i] != nullptr ) delete smResources[i];
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

void ResourceManager::LoadStaticMesh( UINT staticMeshIndex_, std::string filePath_ )
{
	std::string path = std::string( "Assets/" ) + std::string( filePath_ );
	std::ifstream file( path.c_str(), std::ios::binary );
	
	int vertexCount;
	file.read( (char*)&vertexCount, sizeof( int ) );

	Vertex_POS3_NOR3_UV2* vertices = new Vertex_POS3_NOR3_UV2[vertexCount];
	file.read( (char*)vertices, sizeof( Vertex_POS3_NOR3_UV2 ) * vertexCount );

	file.close();

	meshes[staticMeshIndex_].buffer			= CreateVertexBuffer( sizeof( Vertex_POS3_NOR3_UV2 ) * vertexCount, vertices );
	meshes[staticMeshIndex_].vertexCount	= vertexCount;

	delete vertices;
}

void ResourceManager::LoadTexture( UINT textureIndex_, std::string filePath_ )
{
	std::string path = std::string( "Assets/" ) + std::string( filePath_ );
	
	HRESULT hr = DirectX::CreateDDSTextureFromFile(
		device,
		std::wstring( path.begin(), path.end() ).c_str(),
		nullptr,
		&textures[textureIndex_]
		);
	
		if (!SUCCEEDED(hr))
		{
			OutputDebugString( L"Could not load texture!" );
			exit( -1 );
		}
}

void ResourceManager::CreateSprite( UINT resourceIndex_, UINT textureIndex_ )
{
	spResources[resourceIndex_] = new SpriteResource( textureIndex_ );
}

void ResourceManager::CreateStaticMesh( UINT resourceIndex_, UINT meshIndex_, UINT textureIndex_ )
{
	smResources[resourceIndex_] = new StaticMeshResource( meshIndex_, textureIndex_ );
}

void ResourceManager::InitDefaultData()
{
	// Texture
	UINT color = 0xFFFFFFFF;
	ID3D11Texture2D* texture;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory( &texDesc, sizeof( D3D11_TEXTURE2D_DESC ) );
	texDesc.Width				= 1;
	texDesc.Height				= 1;
	texDesc.ArraySize			= 1;
	texDesc.MipLevels			= 1;
	texDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.Usage				= D3D11_USAGE_DEFAULT;
	texDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	texDesc.SampleDesc.Count	= 1;

	D3D11_SUBRESOURCE_DATA subDesc;
	ZeroMemory( &subDesc, sizeof( D3D11_SUBRESOURCE_DATA ) );
	subDesc.pSysMem = &color;
	subDesc.SysMemPitch = sizeof( UINT );

	HRESULT hr = device->CreateTexture2D(&texDesc, &subDesc, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory( &srvDesc, sizeof( D3D11_SHADER_RESOURCE_VIEW_DESC ) );
	srvDesc.Format						= texDesc.Format;
	srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels			= 1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &textures[ASSET_TEXTURE_DEFAULT]);

	texture->Release();

	// Static mesh
	Vertex_POS3_NOR3_UV2 vertices[36] = {
		DirectX::XMFLOAT3( -0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3(  0.0f,  0.0f,  1.0f ), DirectX::XMFLOAT2( 0.375f,  0.0f ),
		DirectX::XMFLOAT3(  0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3(  0.0f,  0.0f,  1.0f ), DirectX::XMFLOAT2( 0.625f,  0.0f ),
		DirectX::XMFLOAT3( -0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3(  0.0f,  0.0f,  1.0f ), DirectX::XMFLOAT2( 0.375f,  0.25f ),
		DirectX::XMFLOAT3( -0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3(  0.0f,  0.0f,  1.0f ), DirectX::XMFLOAT2( 0.375f,  0.25f ),
		DirectX::XMFLOAT3(  0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3(  0.0f,  0.0f,  1.0f ), DirectX::XMFLOAT2( 0.625f,  0.0f ),
		DirectX::XMFLOAT3(  0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3(  0.0f,  0.0f,  1.0f ), DirectX::XMFLOAT2( 0.625f,  0.25f ),
		DirectX::XMFLOAT3( -0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3(  0.0f,  1.0f,  0.0f ), DirectX::XMFLOAT2( 0.375f,  0.25f ),
		DirectX::XMFLOAT3(  0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3(  0.0f,  1.0f,  0.0f ), DirectX::XMFLOAT2( 0.625f,  0.25f ),
		DirectX::XMFLOAT3( -0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3(  0.0f,  1.0f,  0.0f ), DirectX::XMFLOAT2( 0.375f,  0.5f ),
		DirectX::XMFLOAT3( -0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3(  0.0f,  1.0f,  0.0f ), DirectX::XMFLOAT2( 0.375f,  0.5f ),
		DirectX::XMFLOAT3(  0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3(  0.0f,  1.0f,  0.0f ), DirectX::XMFLOAT2( 0.625f,  0.25f ),
		DirectX::XMFLOAT3(  0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3(  0.0f,  1.0f,  0.0f ), DirectX::XMFLOAT2( 0.625f,  0.5f ),
		DirectX::XMFLOAT3( -0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3(  0.0f,  0.0f, -1.0f ), DirectX::XMFLOAT2( 0.375f,  0.5f ),
		DirectX::XMFLOAT3(  0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3(  0.0f,  0.0f, -1.0f ), DirectX::XMFLOAT2( 0.625f,  0.5f ),
		DirectX::XMFLOAT3( -0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3(  0.0f,  0.0f, -1.0f ), DirectX::XMFLOAT2( 0.375f,  0.75f ),
		DirectX::XMFLOAT3( -0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3(  0.0f,  0.0f, -1.0f ), DirectX::XMFLOAT2( 0.375f,  0.75f ),
		DirectX::XMFLOAT3(  0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3(  0.0f,  0.0f, -1.0f ), DirectX::XMFLOAT2( 0.625f,  0.5f ),
		DirectX::XMFLOAT3(  0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3(  0.0f,  0.0f, -1.0f ), DirectX::XMFLOAT2( 0.625f,  0.75f ),
		DirectX::XMFLOAT3( -0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3(  0.0f, -1.0f,  0.0f ), DirectX::XMFLOAT2( 0.375f,  0.75f ),
		DirectX::XMFLOAT3(  0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3(  0.0f, -1.0f,  0.0f ), DirectX::XMFLOAT2( 0.625f,  0.75f ),
		DirectX::XMFLOAT3( -0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3(  0.0f, -1.0f,  0.0f ), DirectX::XMFLOAT2( 0.375f,  1.0f ),
		DirectX::XMFLOAT3( -0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3(  0.0f, -1.0f,  0.0f ), DirectX::XMFLOAT2( 0.375f,  1.0f ),
		DirectX::XMFLOAT3(  0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3(  0.0f, -1.0f,  0.0f ), DirectX::XMFLOAT2( 0.625f,  0.75f ),
		DirectX::XMFLOAT3(  0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3(  0.0f, -1.0f,  0.0f ), DirectX::XMFLOAT2( 0.625f,  1.0f ),
		DirectX::XMFLOAT3(  0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3(  1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.625f,  0.0f ),
		DirectX::XMFLOAT3(  0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3(  1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.875f,  0.0f ),
		DirectX::XMFLOAT3(  0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3(  1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.625f,  0.25f ),
		DirectX::XMFLOAT3(  0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3(  1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.625f,  0.25f ),
		DirectX::XMFLOAT3(  0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3(  1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.875f,  0.0f ),
		DirectX::XMFLOAT3(  0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3(  1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.875f,  0.25f ),
		DirectX::XMFLOAT3( -0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT3( -1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.125f,  0.0f ),
		DirectX::XMFLOAT3( -0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3( -1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.375f,  0.0f ),
		DirectX::XMFLOAT3( -0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3( -1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.125f,  0.25f ),
		DirectX::XMFLOAT3( -0.5f,  0.5f, -0.5f ), DirectX::XMFLOAT3( -1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.125f,  0.25f ),
		DirectX::XMFLOAT3( -0.5f, -0.5f,  0.5f ), DirectX::XMFLOAT3( -1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.375f,  0.0f ),
		DirectX::XMFLOAT3( -0.5f,  0.5f,  0.5f ), DirectX::XMFLOAT3( -1.0f,  0.0f,  0.0f ), DirectX::XMFLOAT2( 0.375f,  0.25f ),
	};

	meshes[ASSET_MESH_DEFAULT].buffer = CreateVertexBuffer( sizeof( Vertex_POS3_NOR3_UV2 ) * 36, vertices );
	meshes[ASSET_MESH_DEFAULT].vertexCount = 36;
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
#include "ResourceManager.h"

HRESULT	ResourceManager::Initialize( ID3D11Device* device_ )
{
	device = device_;

	// Temporary
	Vertex_POS3 vertices[3] = {
		DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f),
		DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f)
		};

	CreateVertexBuffer( sizeof( Vertex_POS3 ) * 3, vertices );

	return S_OK;
}

void ResourceManager::Release()
{
	for( UINT i = 0; i < vertexBuffers.size(); i++ )
		if( vertexBuffers[i] ) vertexBuffers[i]->Release();
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

HRESULT ResourceManager::CreateVertexBuffer( UINT size, void* data )
{
	ID3D11Buffer* buffer;

	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory( &vertexDesc, sizeof(D3D11_BUFFER_DESC) );
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.Usage	 = D3D11_USAGE_DEFAULT;
	vertexDesc.ByteWidth = size;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = data;

	device->CreateBuffer(&vertexDesc, &vertexData, &buffer);

	vertexBuffers.push_back(buffer);

	return S_OK;
}
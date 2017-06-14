#ifndef GRAPHICSDATASTRUCTURES_H
#define GRAPHICSDATASTRUCTURES_H

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

enum InputLayouts
{
	IL_POS3,
	IL_POS3_NOR3_UV2,
	IL_COUNT
};

enum ShaderFlags
{
	SHADERFLAG_V	= 1,
	SHADERFLAG_G	= 2,
	SHADERFLAG_VG	= 3,
	SHADERFLAG_P	= 4,
	SHADERFLAG_VP	= 5,
	SHADERFLAG_GP	= 6,
	SHADERFLAG_VGP	= 7,
	SHADERFLAG_COUNT
};

struct Vertex_POS3
{
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
};

struct ShaderGroup
{
	ID3D11InputLayout*		inputLayout		= nullptr;
	ID3D11VertexShader*		vertexShader	= nullptr;
	ID3D11GeometryShader*	geometryShader	= nullptr;
	ID3D11PixelShader*		pixelShader		= nullptr;

	HRESULT hr = S_OK;

	void ShaderProgram() {}

	HRESULT Initialize( ID3D11Device* device_, wchar_t* filePath_, InputLayouts inputLayout_, ShaderFlags shaderFlag_ )
	{
		ID3DBlob* pBlob = nullptr;

		if( shaderFlag_ & SHADERFLAG_V )
		{
			D3DCompileFromFile(
				filePath_,
				nullptr,
				nullptr,
				"VS_main",
				"vs_5_0",
				0,
				0,
				&pBlob,
				0
				);

			hr = device_->CreateVertexShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &vertexShader );

			if(!SUCCEEDED(hr))
				OutputDebugString(L"VS compile failed!");

			switch ( inputLayout_ )
			{
				case IL_POS3:
				{
					D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					};
					hr = device_->CreateInputLayout( inputDesc, ARRAYSIZE(inputDesc), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &inputLayout );
					break;
				}
				case IL_POS3_NOR3_UV2:
				{
					D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
					};
					hr = device_->CreateInputLayout( inputDesc, ARRAYSIZE(inputDesc), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &inputLayout );
					break;
				}
				default:
					break;
			}

			if(!SUCCEEDED(hr))
				OutputDebugString(L"InputLayout creation failed!");

			pBlob->Release();
		}

		if( shaderFlag_ & SHADERFLAG_G )
		{
			D3DCompileFromFile(
				filePath_,
				nullptr,
				nullptr,
				"GS_main",
				"gs_5_0",
				0,
				0,
				&pBlob,
				nullptr	
				);

			hr = device_->CreateGeometryShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &geometryShader );

			if(!SUCCEEDED(hr))
				OutputDebugString(L"GS compile failed!");

			pBlob->Release();
		}

		if( shaderFlag_ & SHADERFLAG_P )
		{
			D3DCompileFromFile(
				filePath_,
				nullptr,
				nullptr,
				"PS_main",
				"ps_5_0",
				0,
				0,
				&pBlob,
				nullptr	
				);

			hr = device_->CreatePixelShader( pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pixelShader );

			if(!SUCCEEDED(hr))
				OutputDebugString(L"PS compile failed!");

			pBlob->Release();
		}

		return hr;
	}

	void Release()
	{
		if( inputLayout )		inputLayout->Release();
		if( vertexShader )		vertexShader->Release();
		if( geometryShader )	geometryShader->Release();
		if( pixelShader )		pixelShader->Release();
	}
};

#endif
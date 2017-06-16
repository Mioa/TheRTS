#include "Graphics.h"

HRESULT Graphics::Initialize( HWND windowHandle_, LONG windowWidth_, LONG windowHeight_ )
{
	windowHandle = windowHandle_;
	windowWidth  = windowWidth_;
	windowHeight = windowHeight_;

	InitSwapChain();
	InitShaders();
	InitDepthBuffers();
	InitRasterStates();
	InitConstantBuffers();
	InitCamera();

	resourceManager = new ResourceManager();
	resourceManager->Initialize( device );

	RenderQueue::GetInstance()->Initialize();

	// Temporary
	SetViewport( (float)windowWidth, (float)windowHeight );
	deviceContext->RSSetState(defaultRS);
	//

	return S_OK;
}

void Graphics::Release()
{
	if( swapChain )		swapChain->Release();
	if( device )		device->Release();
	if( deviceContext )	deviceContext->Release();

	if( defaultRS )		defaultRS->Release();

	if( defaultRTV )	defaultRTV->Release();
	if( defaultDSV )	defaultDSV->Release();

	if( frameCB )		frameCB->Release();
	if( objectCB )		objectCB->Release();

	defaultShaders.Release();

	resourceManager->Release();
	delete resourceManager;
}

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

HRESULT Graphics::InitSwapChain()
{
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory( &scd, sizeof( DXGI_SWAP_CHAIN_DESC ) );

	scd.BufferCount		  = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width  = windowWidth;
	scd.BufferDesc.Height = windowHeight;
	scd.BufferUsage		  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow	  = windowHandle;
	scd.SampleDesc.Count  = 1;
	scd.Windowed		  = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapChain,
		&device,
		NULL,
		&deviceContext
		);

	if ( SUCCEEDED( hr ) )
	{
		ID3D11Texture2D* pBackBuffer = nullptr;
		swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer );

		hr = device->CreateRenderTargetView( pBackBuffer, NULL, &defaultRTV );
		pBackBuffer->Release();
	}

	return hr;
}

void Graphics::BeginScene()
{
	// Load resources based on queue
}

void Graphics::EndScene()
{
	deviceContext->ClearRenderTargetView( defaultRTV, clearColor );
	deviceContext->ClearDepthStencilView( defaultDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );

	deviceContext->OMSetRenderTargets( 1, &defaultRTV, defaultDSV );

	deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	deviceContext->IASetInputLayout( defaultShaders.inputLayout );

	deviceContext->VSSetShader( defaultShaders.vertexShader, nullptr, 0 );
	deviceContext->PSSetShader( defaultShaders.pixelShader, nullptr, 0 );

	deviceContext->VSSetConstantBuffers( 0, 1, &frameCB );

	for( UINT type = 0; type < RES_SM_COUNT; type++ )
	{
		UINT count = RenderQueue::GetInstance()->staticMeshCount[type];
		for( UINT index = 0; index < count; index++ )
		{
			deviceContext->IASetVertexBuffers( 0, 1, &resourceManager->meshes[type].buffer, &vSize_POS3, &offset );

			deviceContext->UpdateSubresource( objectCB, 0, nullptr, &RenderQueue::GetInstance()->staticMeshes[type][index].transform, sizeof ( DirectX::XMFLOAT4X4 ), 0 ); 
			deviceContext->VSSetConstantBuffers( 1, 1, &objectCB );

			deviceContext->Draw( resourceManager->meshes[type].vertexCount, 0 );
		}
	}

	swapChain->Present( 0, 0 );
	RenderQueue::GetInstance()->ResetQueue();
}

HRESULT Graphics::InitShaders()
{
	hr = defaultShaders.Initialize( device, L"Shaders/defaultShaders.hlsl", IL_POS3, SHADERFLAG_VP );

	return hr;
}

HRESULT Graphics::InitDepthBuffers()
{
	ID3D11Texture2D* texture;

	D3D11_TEXTURE2D_DESC depthMapDesc;
	ZeroMemory( &depthMapDesc, sizeof( D3D11_TEXTURE2D_DESC ) );
	depthMapDesc.Width				= windowWidth;
	depthMapDesc.Height				= windowHeight;
	depthMapDesc.MipLevels			= 1;
	depthMapDesc.ArraySize			= 1;
	depthMapDesc.Format				= DXGI_FORMAT_R32_TYPELESS;;
	depthMapDesc.SampleDesc.Count	= 1;
	depthMapDesc.Usage				= D3D11_USAGE_DEFAULT;
	depthMapDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;

	hr = device->CreateTexture2D( &depthMapDesc, nullptr, &texture );

	if( !SUCCEEDED( hr ) )
	{
		OutputDebugString( L"Depth texture creation failed!" );
		return hr;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof( D3D11_DEPTH_STENCIL_VIEW_DESC ) );
	descDSV.Format				= DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice	= 0;
	descDSV.Flags				= 0;

	hr = device->CreateDepthStencilView( texture, &descDSV, &defaultDSV );
	
	if( !SUCCEEDED( hr ) )
	{
		OutputDebugString( L"DSV creation failed!" );
		return hr;
	}

	texture->Release();

	return hr;
}

HRESULT Graphics::InitConstantBuffers()
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory( &desc, sizeof( D3D11_BUFFER_DESC ) );
	desc.ByteWidth		= sizeof( DirectX::XMFLOAT4X4 );
	desc.Usage			= D3D11_USAGE_DEFAULT;
	desc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;

	hr = device->CreateBuffer( &desc, nullptr, &objectCB );

	desc.ByteWidth		= sizeof( Camera );
	hr = device->CreateBuffer( &desc, nullptr, &frameCB );

	return hr;
}

HRESULT Graphics::InitRasterStates()
{
	D3D11_RASTERIZER_DESC rasterizerState;
	ZeroMemory( &rasterizerState, sizeof( D3D11_RASTERIZER_DESC ) );
	rasterizerState.FillMode		= D3D11_FILL_SOLID;
	rasterizerState.CullMode		= D3D11_CULL_NONE;
	rasterizerState.DepthClipEnable = true;

	if( defaultRS ) defaultRS->Release();

	hr = device->CreateRasterizerState( &rasterizerState, &defaultRS );

	return hr;
}

void Graphics::InitCamera()
{
	cameraPos	= DirectX::XMFLOAT4( 0.0f, 10.0f, -2.0f, 1.0f );
	cameraFocus = DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );

	// View
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat4( &cameraPos ),
		DirectX::XMLoadFloat4( &cameraFocus ),
		DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f )
		);

	DirectX::XMStoreFloat4x4( &camera.view, DirectX::XMMatrixTranspose( view ) );

	// Projection
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(
		float(DirectX::XM_PI / 180.0f) * 45.0f, 
		(float)windowWidth / (float)windowHeight, 
		0.1f, 
		2000.0f);

	DirectX::XMStoreFloat4x4( &camera.projection, DirectX::XMMatrixTranspose( projection ) );
}

void Graphics::SetViewport( float windowWidth_, float windowHeight_ )
{
	D3D11_VIEWPORT vp;
	vp.Width	= windowWidth_;
	vp.Height	= windowHeight_;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	deviceContext->RSSetViewports( 1, &vp );
}

ResourceManager* Graphics::GetResourceManager()
{
	return resourceManager;
}

void Graphics::UpdateCamera( DirectX::XMFLOAT4 position_ )
{
	DirectX::XMStoreFloat4(&cameraPos, DirectX::XMVectorAdd( DirectX::XMLoadFloat4( &cameraPos ), DirectX::XMLoadFloat4( &position_ ) ));
	DirectX::XMStoreFloat4(&cameraFocus, DirectX::XMVectorAdd( DirectX::XMLoadFloat4( &cameraFocus ), DirectX::XMLoadFloat4( &position_ ) ));

	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat4( &cameraPos ),
		DirectX::XMLoadFloat4( &cameraFocus ),
		DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f )
		);

	DirectX::XMStoreFloat4x4( &camera.view, DirectX::XMMatrixTranspose( view ) );

	deviceContext->UpdateSubresource( frameCB, 0, nullptr, &camera, sizeof ( Camera ), 0 ); 
}
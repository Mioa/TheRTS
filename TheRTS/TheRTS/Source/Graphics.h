#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "RenderQueue.h"
#include "ResourceManager.h"

class Graphics
{
	//Variables
	public:
	private:
		// D3D
		IDXGISwapChain*			swapChain		= nullptr;
		ID3D11Device*			device			= nullptr;
		ID3D11DeviceContext*	deviceContext	= nullptr;

		ID3D11RasterizerState*	defaultRS		= nullptr;

		ID3D11RenderTargetView* defaultRTV		= nullptr;
		ID3D11DepthStencilView* defaultDSV		= nullptr;

		ID3D11Buffer* frameCB		= nullptr;
		ID3D11Buffer* objectCB		= nullptr;
		ID3D11Buffer* spriteCB		= nullptr;

		ID3D11SamplerState*	linearSamp = nullptr;

		// Shaders
		ShaderGroup defaultShaders;
		ShaderGroup spriteShaders;

		// Misc
		HWND windowHandle;
		LONG windowWidth;
		LONG windowHeight;

		HRESULT hr;

		ResourceManager* resourceManager;

		Camera camera;
		DirectX::XMFLOAT4 cameraPos;
		DirectX::XMFLOAT4 cameraFocus;

		float  clearColor[4]		= { 0.2f, 0.2f, 0.2f, 1.0f };
		UINT32 vSize_POS3			= sizeof( Vertex_POS3 );
		UINT32 vSize_POS3_NOR3_UV2	= sizeof( Vertex_POS3_NOR3_UV2 );
		UINT32 offset				= 0;

	//Functions
	public:
		HRESULT	Initialize( HWND windowHandle_, LONG windowWidth_, LONG windowHeight_ );
		void	Release();

		Graphics() {}
		~Graphics() {}
	public:
		void BeginScene();
		void EndScene();

		ResourceManager* GetResourceManager();
		void UpdateCamera( DirectX::XMFLOAT4 position_ );

	private:
		HRESULT InitSwapChain();
		HRESULT InitShaders();
		HRESULT InitDepthBuffers();
		HRESULT InitConstantBuffers();
		HRESULT InitRasterStates();
		HRESULT InitSamplers();
		void	InitCamera();

		void SetPipeline( UINT pipeline_ );

		void DrawSprites();
		void DrawStaticMeshes();

		void SetViewport( float windowWidth_, float windowHeight_ );
};
#endif
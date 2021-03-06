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

		ID3D11Buffer* frameCB	= nullptr;
		ID3D11Buffer* objectCB	= nullptr;

		// Shaders
		ShaderGroup defaultShaders;

		// Misc
		HWND windowHandle;
		LONG windowWidth;
		LONG windowHeight;

		HRESULT hr;

		ResourceManager* resourceManager;

		float  clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		UINT32 vSize_POS3	 = sizeof( Vertex_POS3 );
		UINT32 offset		 = 0;

	//Functions
	public:
		HRESULT	Initialize( HWND windowHandle_, LONG windowWidth_, LONG windowHeight_ );
		void	Release();

				Graphics();
				~Graphics();
	public:
		void BeginScene();
		void EndScene();

		ResourceManager* GetResourceManager();

	private:
		HRESULT InitSwapChain();
		HRESULT InitShaders();
		HRESULT InitRenderTargets();
		HRESULT InitDepthBuffers();
		HRESULT InitConstantBuffers();
		HRESULT InitRasterStates();

		void SetViewport( float windowWidth_, float windowHeight_ ); 
		void UpdateConstantBuffer( UINT size_, void* data_ );
};
#endif
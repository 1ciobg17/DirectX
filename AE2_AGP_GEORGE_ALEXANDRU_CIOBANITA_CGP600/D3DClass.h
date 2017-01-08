//////////////////////////////////
// Filename: D3DClass.h
/////////////////////////////////
#pragma once
#pragma comment(lib, "dxgi.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>

class D3DClass
{
private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11DepthStencilView* m_ZBuffer;
	ID3D11RasterizerState* m_rasterState;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
	ID3D11RasterizerState* m_rasterStateNoCulling;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(D3DMATRIX&);
	void GetWorldMatrix(D3DMATRIX&);
	void GetOrthoMatrix(D3DMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	void TurnOnCulling();
	void TurnOffCulling();

	void TurnOnZBuffer();
	void TurnOffZBuffer();

	void SetBackStates();
};
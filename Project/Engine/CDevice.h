#pragma once

// DirectX 관련 기능을 담당
// GPU 제어
class CConstantBuffer;

#include "CTexture.h"

class CDevice
	: public CSingleton<CDevice>
{
	SINGLE(CDevice);
public:
	int Initialize(HWND _hWnd, UINT _Width, UINT _Height);
	void Present();

public:
	void SetViewport(float _Width, float _Height);

public:
	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetContext() { return m_Context.Get(); }

	ID3D11RasterizerState* GetRasterizerState(RASTERIZER_STATE_TYPE _Type) { return m_RSState[(UINT)_Type].Get(); }
	ID3D11DepthStencilState* GetDepthStencilState(DEPTH_STENCIL_STATE_TYPE _Type) { return m_DSState[(UINT)_Type].Get(); }
	ID3D11BlendState* GetBlendState(BLEND_STATE_TYPE _Type) { return m_BSState[(UINT)_Type].Get(); }
	CConstantBuffer* GetConstantBuffer(CONSTANT_BUFFER_TYPE _Type) { return m_arrCB[(UINT)_Type]; }

public:
	Vec2 GetResolution() { return m_vResolution; }

private:
	int CreateSwapChain();
	int CreateView();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSamplerState();
	int CreateConstantBuffer();

private:
	HWND							m_hWnd;
	Vec2							m_vResolution;

	ComPtr<ID3D11Device>			m_Device;	// Dx11 객체 생성, GPU 메모리 할당
	ComPtr<ID3D11DeviceContext>		m_Context;	// Rendering, 

	ComPtr<IDXGISwapChain>			m_SwapChain;

	Ptr<CTexture>					m_RTTex;
	Ptr<CTexture>					m_DSTex;

	ComPtr<ID3D11RasterizerState>	m_RSState[(UINT)RASTERIZER_STATE_TYPE::END];
	ComPtr<ID3D11SamplerState>		m_Sampler[3];

	ComPtr<ID3D11DepthStencilState>	m_DSState[(UINT)DEPTH_STENCIL_STATE_TYPE::END];
	ComPtr<ID3D11BlendState>		m_BSState[(UINT)BLEND_STATE_TYPE::END];

	CConstantBuffer*				m_arrCB[(UINT)CONSTANT_BUFFER_TYPE::END];

};

#include "pch.h"
#include "CDevice.h"

#include "CConstantBuffer.h"
#include "CAssetMgr.h"

CDevice::CDevice()
    : m_hWnd(nullptr)
    , m_arrCB{}
    , m_Sampler{}
{
}

CDevice::~CDevice()
{
    for (UINT i = 0; i < (UINT)CONSTANT_BUFFER_TYPE::END; ++i)
    {
        if (nullptr != m_arrCB[i])
            delete m_arrCB[i];
    }
}

int CDevice::Initialize(HWND _hWnd, UINT _Width, UINT _Height)
{
    // 윈도우 핸들 및 해상도 값 저장
    m_hWnd = _hWnd;

    m_vResolution.x = (float)_Width;
    m_vResolution.y = (float)_Height;

    // Device, Context 생성
    UINT Flag = 0;

#ifdef _DEBUG
    Flag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE
        , nullptr, Flag
        , nullptr, 0
        , D3D11_SDK_VERSION
        , m_Device.GetAddressOf(), nullptr, m_Context.GetAddressOf())))
    {
        MessageBox(nullptr, L"Device & Context creation failed.", L"장치 초기화 실패", MB_OK);
        return E_FAIL;
    }

    // Swap Chain
    if (FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"Swap Chain creation failed.", L"장치 초기화 실패", MB_OK);
        return E_FAIL;
    }

    // RenderTargetView, DepthStencilView
    if (FAILED(CreateView()))
    {
        MessageBox(nullptr, L"View creation failed.", L"장치 초기화 실패", MB_OK);
        return E_FAIL;
    }

    // Rasterizer State
    if (FAILED(CreateRasterizerState()))
    {
        MessageBox(nullptr, L"Rasterizer State creation failed.", L"장치 초기화 실패", MB_OK);
        return E_FAIL;
    }

    // Depth Stencil State
    if (FAILED(CreateDepthStencilState()))
    {
        MessageBox(nullptr, L"Depth Stencil State creation failed.", L"장치 초기화 실패", MB_OK);
        return E_FAIL;
    }

    // Blend State
    if (FAILED(CreateBlendState()))
    {
        MessageBox(nullptr, L"Blend State creation failed.", L"장치 초기화 실패", MB_OK);
        return E_FAIL;
    }

    // Sampler State
    if (FAILED(CreateSamplerState()))
    {
        MessageBox(nullptr, L"Sampler State creation failed.", L"장치 초기화 실패", MB_OK);
        return E_FAIL;
    }

    // Constant Buffer
    if (FAILED(CreateConstantBuffer()))
    {
        MessageBox(nullptr, L"Constant Buffer creation failed.", L"장치 초기화 실패", MB_OK);
        return E_FAIL;
    }

    SetViewport(m_vResolution.x, m_vResolution.y);

    return S_OK;
}

void CDevice::Present()
{
    m_SwapChain->Present(0, 0);
}

void CDevice::SetViewport(float _Width, float _Height)
{
    // ViewPort 설정
    // 출력시킬 화면 윈도우 영역을 설정
    D3D11_VIEWPORT viewport = {};

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = _Width;
    viewport.Height = _Height;

    viewport.MinDepth = 0;
    viewport.MaxDepth = 1.f;

    CONTEXT->RSSetViewports(1, &viewport);
}

int CDevice::CreateSwapChain()
{
    // SwapChain : 화면에 그림을 그리기위한 버퍼를 관리하고,
    //             버퍼에 그려진 이미지를 화면 출력을 담당하는 객체
    DXGI_SWAP_CHAIN_DESC Desc = {};

    Desc.BufferCount = 1; // 백버퍼 개수
    Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    Desc.BufferDesc.Width = (UINT)m_vResolution.x;          // 백버퍼 해상도 
    Desc.BufferDesc.Height = (UINT)m_vResolution.y;         // 백버퍼 해상도
    Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // 픽셀 포맷
    Desc.BufferDesc.RefreshRate.Denominator = 1;
    Desc.BufferDesc.RefreshRate.Numerator = 120;
    Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    Desc.Windowed = true;   // 창모드 설정
    Desc.OutputWindow = m_hWnd; // SwapChain 버퍼의 이미지를 출력시킬 윈도우 핸들
    Desc.Flags = 0;
    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;
    Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    ComPtr<IDXGIDevice>     Device = nullptr;
    ComPtr<IDXGIAdapter>    Adapter = nullptr;
    ComPtr<IDXGIFactory>    Factory = nullptr;

    if (FAILED(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)Device.GetAddressOf())))
        return E_FAIL;

    if (FAILED(Device->GetParent(__uuidof(IDXGIAdapter), (void**)Adapter.GetAddressOf())))
        return E_FAIL;

    if (FAILED(Adapter->GetParent(__uuidof(IDXGIFactory), (void**)Factory.GetAddressOf())))
        return E_FAIL;

    if (FAILED(Factory->CreateSwapChain(m_Device.Get(), &Desc, m_SwapChain.GetAddressOf())))
    {
        return E_FAIL;
    }

    return S_OK;
}

int CDevice::CreateView()
{
    // =======================================================
    // RenderTarget Texture, DepthStencil Texture 를 생성시킨다
    // =======================================================
    // 스왚체인의 백버퍼의 주소를 받아온다.
    ComPtr<ID3D11Texture2D> RenderTargetTex;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)RenderTargetTex.GetAddressOf());
    m_RTTex = CAssetMgr::GetInstance()->CreateTexture(L"RenderTargetTex", RenderTargetTex);

    // DepthStencil 텍스쳐 생성
    m_DSTex = CAssetMgr::GetInstance()->CreateTexture(L"DepthStencilTex"
                                                      , (UINT)m_vResolution.x, (UINT)m_vResolution.y
                                                      , DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

    // Render Target 및 DepthStencilTexture Target 지정
    CONTEXT->OMSetRenderTargets(1, m_RTTex->GetRTV().GetAddressOf(), m_DSTex->GetDSV().Get());

    return S_OK;
}

int CDevice::CreateRasterizerState()
{
    D3D11_RASTERIZER_DESC Desc = {};

    // Cull Back
    m_RSState[(UINT)RASTERIZER_STATE_TYPE::CULL_BACK] = nullptr;

    // Cull Front 
    Desc.CullMode = D3D11_CULL_FRONT;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RASTERIZER_STATE_TYPE::CULL_FRONT].GetAddressOf());

    // Cull None
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RASTERIZER_STATE_TYPE::CULL_NONE].GetAddressOf());

    // Wire Frame
    Desc.CullMode = D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_WIREFRAME;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RASTERIZER_STATE_TYPE::WIRE_FRAME].GetAddressOf());

    return S_OK;
}

int CDevice::CreateDepthStencilState()
{
    D3D11_DEPTH_STENCIL_DESC Desc = {};

    // Less : 더 작은 깊이가 통과
    m_DSState[(UINT)DEPTH_STENCIL_STATE_TYPE::LESS] = nullptr;

    // Less Equal
    Desc.DepthEnable = true;                            // 깊이판정을 진행
    Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;       // 깊이 판정 방식
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // 깊이판정을 성공시 깊이 기록여부
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DEPTH_STENCIL_STATE_TYPE::LESS_EQUAL].GetAddressOf())))
    {
        return E_FAIL;
    }

    // Greater
    Desc.DepthEnable = true;                            // 깊이판정을 진행
    Desc.DepthFunc = D3D11_COMPARISON_GREATER;          // 깊이 판정 방식
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // 깊이판정을 성공시 깊이 기록여부
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DEPTH_STENCIL_STATE_TYPE::GREATER].GetAddressOf())))
    {
        return E_FAIL;
    }

    // NO_TEST
    Desc.DepthEnable = true;                            // 깊이판정을 진행
    Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;           // 깊이 판정 방식
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // 깊이판정을 성공시 깊이 기록여부
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DEPTH_STENCIL_STATE_TYPE::NO_TEST].GetAddressOf())))
    {
        return E_FAIL;
    }

    // NO_WRITE
    Desc.DepthEnable = true;                            // 깊이판정을 진행
    Desc.DepthFunc = D3D11_COMPARISON_LESS;             // 깊이 판정 방식
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // 깊이판정을 성공시 깊이 기록여부
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DEPTH_STENCIL_STATE_TYPE::NO_WRITE].GetAddressOf())))
    {
        return E_FAIL;
    }

    // NO_TEST_NO_WRITE
    Desc.DepthEnable = true;                            // 깊이판정을 진행
    Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;           // 깊이 판정 방식
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // 깊이판정을 성공시 깊이 기록여부
    Desc.StencilEnable = false;

    if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DEPTH_STENCIL_STATE_TYPE::NO_TEST_NO_WRITE].GetAddressOf())))
    {
        return E_FAIL;
    }

    //// INNER - VolumeMesh 뒷변보다 앞쪽에 있는 부분 체크
    //Desc.DepthEnable = true;    // 깊이판정을 진행
    //Desc.DepthFunc = D3D11_COMPARISON_GREATER;           // 깊이 판정 방식
    //Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;   // 깊이판정을 성공시 깊이 기록여부
    //Desc.StencilEnable = true;

    //Desc.BackFace.StencilFunc           = D3D11_COMPARISON_ALWAYS;  // 항상 통과    
    //Desc.BackFace.StencilDepthFailOp    = D3D11_STENCIL_OP_KEEP;    // 깊이가 실패(뒷면보다 더 멀다)
    //Desc.BackFace.StencilFailOp         = D3D11_STENCIL_OP_KEEP;    // 스텐실 테스트는 항상 통과기 때문에 발생할 일이 없다.
    //Desc.BackFace.StencilPassOp         = D3D11_STENCIL_OP_INCR;    // 스텐실 값을 증가시킨다.


    //Desc.FrontFace.StencilFunc;
    //Desc.FrontFace.StencilPassOp;
    //Desc.FrontFace.StencilDepthFailOp;
    //Desc.FrontFace.StencilDepthFailOp;

    //if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf())))
    //{
    //    return E_FAIL;
    //}


    //// OUTER - VolumeMesh 앞면보다 뒤에 있는 부분 체크
    //Desc.DepthEnable = true;    // 깊이판정을 진행
    //Desc.DepthFunc = D3D11_COMPARISON_LESS;          // 깊이 판정 방식
    //Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;   // 깊이판정을 성공시 깊이 기록여부
    //Desc.StencilEnable = true;

    //Desc.FrontFace.StencilFunc          = D3D11_COMPARISON_EQUAL; // 특정값(1) 이랑 동일할 경우 Stencil 통과
    //Desc.FrontFace.StencilFailOp        = D3D11_STENCIL_OP_KEEP;  // 스텐실 실패 == Inner 체크 성공을 못한 부분들, 아무것도 할 필요가 없다.
    //Desc.FrontFace.StencilDepthFailOp   = D3D11_STENCIL_OP_DECR;  // 스텐실 성공, 깊이 실패, 뒷면보다는 안쪽에 있었지만 앞면보다도 더 앞쪽에 있었다    
    //Desc.FrontFace.StencilPassOp        = D3D11_STENCIL_OP_KEEP;  // 스텐실 성공, 깊이 성공, 뒷면보다 안쪽에, 앞면보다 뒷쪽에, 볼륨메쉬 내부영역

    //Desc.BackFace.StencilFunc;
    //Desc.BackFace.StencilPassOp;
    //Desc.BackFace.StencilDepthFailOp;
    //Desc.BackFace.StencilDepthFailOp;  

    //if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf())))
    //{
    //    return E_FAIL;
    //}


    //// VOLUME_CHECK
    //Desc.DepthEnable    = true;    // 깊이판정을 진행
    //Desc.DepthFunc      = D3D11_COMPARISON_GREATER;           // 깊이 판정 방식
    //Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;   // 깊이판정을 성공시 깊이 기록여부
    //Desc.StencilEnable  = true;

    //Desc.BackFace.StencilFunc           = D3D11_COMPARISON_ALWAYS;  // 항상 통과    
    //Desc.BackFace.StencilFailOp         = D3D11_STENCIL_OP_KEEP;    // 스텐실 테스트는 항상 통과기 때문에 발생할 일이 없다.
    //Desc.BackFace.StencilDepthFailOp    = D3D11_STENCIL_OP_DECR;    // 깊이가 실패(뒷면보다 더 멀다)
    //Desc.BackFace.StencilPassOp         = D3D11_STENCIL_OP_INCR;    // 스텐실 값을 증가시킨다.

    //Desc.FrontFace.StencilFunc          = D3D11_COMPARISON_ALWAYS; // 특정값(1) 이랑 동일할 경우 Stencil 통과
    //Desc.FrontFace.StencilFailOp        = D3D11_STENCIL_OP_KEEP;  // 스텐실 실패 == Inner 체크 성공을 못한 부분들, 아무것도 할 필요가 없다.
    //Desc.FrontFace.StencilDepthFailOp   = D3D11_STENCIL_OP_INCR;  // 스텐실 성공, 깊이 실패, 뒷면보다는 안쪽에 있었지만 앞면보다도 더 앞쪽에 있었다    
    //Desc.FrontFace.StencilPassOp        = D3D11_STENCIL_OP_DECR;  // 스텐실 성공, 깊이 성공, 뒷면보다 안쪽에, 앞면보다 뒷쪽에, 볼륨메쉬 내부영역

    //if (FAILED(DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf())))
    //{
    //    return E_FAIL;
    //}

    return S_OK;
}

int CDevice::CreateBlendState()
{
    D3D11_BLEND_DESC Desc = {};

    // Default
    m_BSState[(UINT)BLEND_STATE_TYPE::DEFAULT] = nullptr;

    // Alpha Blend - Coverage
    Desc.AlphaToCoverageEnable = true;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Src(Pixel RGB) * A     +      Dest(RenderTarget RGB) * (1 - A)
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // 계수
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // 계수

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

    if (FAILED(DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BLEND_STATE_TYPE::ALPHABLEND_COVERAGE].GetAddressOf())))
    {
        return E_FAIL;
    }

    // Alpha Blend
    Desc.AlphaToCoverageEnable = false;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Src(Pixel RGB) * A     +      Dest(RenderTarget RGB) * (1 - A)
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;      // 계수
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // 계수

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

    if (FAILED(DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BLEND_STATE_TYPE::ALPHABLEND].GetAddressOf())))
    {
        return E_FAIL;
    }

    // One - One Blend
    Desc.AlphaToCoverageEnable = false;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    // Src(Pixel RGB) * 1  +  Dest(RenderTarget RGB) * 1
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;    // 계수
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;   // 계수

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

    if (FAILED(DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BLEND_STATE_TYPE::ONE_ONE].GetAddressOf())))
    {
        return E_FAIL;
    }

    return S_OK;
}

int CDevice::CreateSamplerState()
{
    D3D11_SAMPLER_DESC Desc = {};

    //Anisotropic Filter
    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_ANISOTROPIC;

    if (FAILED(DEVICE->CreateSamplerState(&Desc, m_Sampler[0].GetAddressOf())))
    {
        return E_FAIL;
    }

    // Point Filter
    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

    if (FAILED(DEVICE->CreateSamplerState(&Desc, m_Sampler[1].GetAddressOf())))
    {
        return E_FAIL;
    }

    // Linear Filter
    Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    Desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    if (FAILED(DEVICE->CreateSamplerState(&Desc, m_Sampler[2].GetAddressOf())))
    {
        return E_FAIL;
    }

    CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->PSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->CSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

    CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->CSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());

    CONTEXT->VSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
    CONTEXT->HSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
    CONTEXT->DSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
    CONTEXT->GSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
    CONTEXT->PSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());
    CONTEXT->CSSetSamplers(2, 1, m_Sampler[2].GetAddressOf());

    return S_OK;
}

int CDevice::CreateConstantBuffer()
{
    CConstantBuffer* pCB = nullptr;

    // World, View, Projection Matrix Transmission
    pCB = new CConstantBuffer;
    if (FAILED(pCB->Create(CONSTANT_BUFFER_TYPE::TRANSFORM, sizeof(tTransform))))
    {
        MessageBox(nullptr, L"Transform constant buffer creation failed.", L"초기화 실패", MB_OK);
        return E_FAIL;
    }
    m_arrCB[(UINT)CONSTANT_BUFFER_TYPE::TRANSFORM] = pCB;

    pCB = new CConstantBuffer;
    if (FAILED(pCB->Create(CONSTANT_BUFFER_TYPE::MATERIAL, sizeof(tMtrlConst))))
    {
        MessageBox(nullptr, L"Material constant buffer creation failed.", L"초기화 실패", MB_OK);
        return E_FAIL;
    }
    m_arrCB[(UINT)CONSTANT_BUFFER_TYPE::MATERIAL] = pCB;

    pCB = new CConstantBuffer;
    if (FAILED(pCB->Create(CONSTANT_BUFFER_TYPE::SPRITE, sizeof(tSpriteInfo))))
    {
        MessageBox(nullptr, L"Sprite constant buffer creation failed.", L"초기화 실패", MB_OK);
        return E_FAIL;
    }
    m_arrCB[(UINT)CONSTANT_BUFFER_TYPE::SPRITE] = pCB;

    pCB = new CConstantBuffer;
    if (FAILED(pCB->Create(CONSTANT_BUFFER_TYPE::GLOBAL, sizeof(tGlobalData))))
    {
        MessageBox(nullptr, L"Global Data constant buffer creation failed.", L"초기화 실패", MB_OK);
        return E_FAIL;
    }
    m_arrCB[(UINT)CONSTANT_BUFFER_TYPE::GLOBAL] = pCB;

    return S_OK;
}

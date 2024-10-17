#include "pch.h"
#include "CRenderMgr.h"

// Default Headers
#include "components.h"
#include "assets.h"

// Manager Headers
#include "CAssetMgr.h"
#include "CGameObject.h"

// Module Headers
#include "CMRT.h"

void CRenderMgr::Initialize()
{
	// AssetMgr �� �ʱ�ȭ�ɶ� ������ ��ó���� �ؽ��ĸ� �����Ѵ�.
	m_PostProcessTex = CAssetMgr::GetInstance()->FindAsset<CTexture>(L"PostProcessTex");

	// ����� �������� ���� ������Ʈ
	m_DebugObject = new CGameObject;
	m_DebugObject->AddComponent(new CTransform);
	m_DebugObject->AddComponent(new CMeshRender);
	m_DebugObject->MeshRender()->SetMaterial(CAssetMgr::GetInstance()->FindAsset<CMaterial>(L"DebugShapeMtrl"));

	// MRT ����
	CreateMRT();
}

void CRenderMgr::CreateMRT()
{
	CMRT* pMRT = nullptr;

	// =============
	// SwapChain MRT
	// =============
	{
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInstance()->FindAsset<CTexture>(L"RenderTargetTex"), };
		Ptr<CTexture> pDSTex = CAssetMgr::GetInstance()->FindAsset<CTexture>(L"DepthStencilTex");
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		pMRT = new CMRT;
		pMRT->SetName(L"SwapChain");
		pMRT->Create(1, arrRT, pDSTex);
		pMRT->SetClearColor(arrClearColor, false);

		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN] = pMRT;
	}

	// =============
	// Effect MRT
	// =============
	{
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInstance()->FindAsset<CTexture>(L"EffectTargetTex"), };
		Ptr<CTexture> pDSTex = CAssetMgr::GetInstance()->FindAsset<CTexture>(L"EffectDepthStencilTex");
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		pMRT = new CMRT;
		pMRT->SetName(L"Effect");
		pMRT->Create(1, arrRT, pDSTex);
		pMRT->SetClearColor(arrClearColor, false);

		m_arrMRT[(UINT)MRT_TYPE::EFFECT] = pMRT;
	}

	// ===============
	// EffectBlur MRT
	// ===============
	{
		Ptr<CTexture> arrRT[8] = { CAssetMgr::GetInstance()->FindAsset<CTexture>(L"EffectBlurTargetTex"), };
		Ptr<CTexture> pDSTex = nullptr;
		Vec4		  arrClearColor[8] = { Vec4(0.f, 0.f, 0.f, 0.f), };

		pMRT = new CMRT;
		pMRT->SetName(L"EffectBlur");
		pMRT->Create(1, arrRT, nullptr);
		pMRT->SetClearColor(arrClearColor, false);

		m_arrMRT[(UINT)MRT_TYPE::EFFECT_BLUR] = pMRT;
	}
}

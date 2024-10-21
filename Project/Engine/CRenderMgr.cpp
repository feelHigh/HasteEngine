#include "pch.h"
#include "CRenderMgr.h"

// Device Headers
#include "CDevice.h"
#include "CConstantBuffer.h"
#include "CStructuredBuffer.h"

// Manager Headers
#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CLevelMgr.h"

// Level Headers
#include "CLevel.h"

// Component Headers
#include "CTransform.h"
#include "CCamera.h"
#include "CLight2D.h"
#include "CLight3D.h"
#include "CMeshRender.h"

// GameObject Headers
#include "CGameObject.h"

// Module Headers
#include "CMRT.h"

CRenderMgr::CRenderMgr()
	: m_EditorCamera(nullptr)
	, m_Light2DBuffer(nullptr)
	, m_DebugObject(nullptr)
	, m_arrMRT{}
{
	m_Light2DBuffer = new CStructuredBuffer;
	m_Light3DBuffer = new CStructuredBuffer;
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_DebugObject)
		delete m_DebugObject;

	if (nullptr != m_Light2DBuffer)
		delete m_Light2DBuffer;

	if (nullptr != m_Light3DBuffer)
		delete m_Light3DBuffer;

	Delete_Array(m_arrMRT);
}

void CRenderMgr::Tick()
{
	CLevel* pCurLevel = CLevelMgr::GetInstance()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return;

	RenderStart();

	// Level �� Player ������ ���, Level ���� �ִ� ī�޶� �������� �������ϱ�
	if (PLAY == pCurLevel->GetState())
	{
		if (nullptr != m_vecCam[0])
			Render(m_vecCam[0]);

		for (size_t i = 1; i < m_vecCam.size(); ++i)
		{
			if (nullptr == m_vecCam[i])
				continue;

			Render_Sub(m_vecCam[1]);
		}
	}

	// Level �� Stop �̳� Pause �� ���, Editor �� ī�޶� �������� ������ �ϱ�
	else
	{
		if (nullptr != m_EditorCamera)
		{
			Render(m_EditorCamera);
		}
	}

	// Debug Render
	RenderDebugShape();

	// Time ���� ���
	CTimeMgr::GetInstance()->Render();

	// Clear
	Clear();
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _CamPriority)
{
	// ī�޶� �켱������ ���� ������ ������ ���õǾ�� �Ѵ�.
	if (m_vecCam.size() <= _CamPriority + 1)
		m_vecCam.resize(_CamPriority + 1);

	// ī�޶� �켱������ �´� ��ġ�� �ִ´�
	m_vecCam[_CamPriority] = _Cam;
}

void CRenderMgr::PostProcessCopy()
{
	// RenderTarget -> PostProcessTex
	Ptr<CTexture> pRTTex = CAssetMgr::GetInstance()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

CCamera* CRenderMgr::GetPOVCam()
{
	CLevel* pCurLevel = CLevelMgr::GetInstance()->GetCurrentLevel();

	if (nullptr == pCurLevel)
		return nullptr;

	if (LEVEL_STATE::PLAY == pCurLevel->GetState())
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}

	else
	{
		return m_EditorCamera;
	}
}

void CRenderMgr::RenderStart()
{
	// ����Ÿ�� Ŭ���� �� ����
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->ClearRenderTargetView();
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->ClearDepthStencilView();
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();

	// GlobalData ����
	g_GlobalData.g_Resolution = CDevice::GetInstance()->GetResolution();
	g_GlobalData.g_Light2DCount = (int)m_vecLight2D.size();
	g_GlobalData.g_Light3DCount = (int)m_vecLight3D.size();

	// Light2D ���� ������Ʈ �� ���ε�
	vector<tLightInfo> vecLight2DInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}

	if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	{
		m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	if (!vecLight2DInfo.empty())
	{
		m_Light2DBuffer->SetData(vecLight2DInfo.data());
		m_Light2DBuffer->Binding(11);
	}

	// Light3D ���� ������Ʈ �� ���ε�
	vector<tLightInfo> vecLight3DInfo;
	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		vecLight3DInfo.push_back(m_vecLight3D[i]->GetLightInfo());
	}

	if (m_Light3DBuffer->GetElementCount() < vecLight3DInfo.size())
	{
		m_Light3DBuffer->Create(sizeof(tLightInfo), (UINT)vecLight3DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	if (!vecLight3DInfo.empty())
	{
		m_Light3DBuffer->SetData(vecLight3DInfo.data());
		m_Light3DBuffer->Binding(12);
	}

	// ���� ȭ���� �������ϴ� ī�޶��� ������� Global �����Ϳ� ����
	CLevel* pCurLevel = CLevelMgr::GetInstance()->GetCurrentLevel();
	CCamera* pCam = nullptr;
	if (PLAY == pCurLevel->GetState())
		pCam = m_vecCam[0];
	else
		pCam = m_EditorCamera;

	if (pCam == nullptr)
		g_GlobalData.g_CamWorldPos = Vec3(0.f, 0.f, 0.f);
	else
		g_GlobalData.g_CamWorldPos = pCam->Transform()->GetWorldPosition();


	// GlobalData ���ε�
	static CConstantBuffer* pGlobalCB = CDevice::GetInstance()->GetConstantBuffer(CONSTANT_BUFFER_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_GlobalData);
	pGlobalCB->Binding();
}

void CRenderMgr::Render(CCamera* _Cam)
{
	// ������Ʈ �з�
	_Cam->SortGameObject();

	// ī�޶� ��ȯ��� ����
	// ��ü�� �������� �� ����� View, Projection ���
	g_Trans.matView = _Cam->GetViewMatrix();
	g_Trans.matProj = _Cam->GetProjectionMatrix();

	// MRT ��� Ŭ����
	ClearMRT();

	// ==================
	// DEFERRED RENDERING
	// ==================
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();
	_Cam->Render_Deferred();

	// ===============
	// LIGHT RENDERING
	// ===============
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->OMSet();

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		m_vecLight3D[i]->Render();
	}

	// ===================================
	// MERGE ALBEDO + LIGHTS ==> SwapChain
	// ===================================
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();
	m_MergeMtrl->Binding();
	m_RectMesh->Render();

	// =================
	// FORWARD RENDERING
	// =================
	// �з��� ��ü�� ������
	_Cam->Render_Opaque();
	_Cam->Render_Masked();
	_Cam->Render_Effect();
	_Cam->Render_Transparent();
	_Cam->Render_Particle();
	_Cam->Render_Postprocess();
	_Cam->Render_UI();

	// ����
	_Cam->Clear();
}

void CRenderMgr::Render_Sub(CCamera* _Cam)
{
}

void CRenderMgr::Clear()
{
	m_vecLight2D.clear();
	m_vecLight3D.clear();
}

void CRenderMgr::RenderDebugShape()
{
	list<tDebugShapeInfo>::iterator iter = m_DebugShapeList.begin();

	for (; iter != m_DebugShapeList.end(); )
	{
		// ����� ��û ��翡 �´� �޽ø� ������
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInstance()->FindAsset<CMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInstance()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::LINE:

			break;
		case DEBUG_SHAPE::CUBE:

			break;
		case DEBUG_SHAPE::SPHERE:

			break;
		}

		// ��ġ ����
		m_DebugObject->Transform()->SetWorldMatrix((*iter).matWorld);

		// ���� ����
		m_DebugObject->MeshRender()->GetMaterial()->SetScalarParameter(VEC4_0, (*iter).vColor);

		// �������� ���ο� ����, ���̴��� �������� ����� �����Ѵ�.
		if ((*iter).DepthTest)
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDepthStencilStateType(DEPTH_STENCIL_STATE_TYPE::LESS);
		else
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDepthStencilStateType(DEPTH_STENCIL_STATE_TYPE::NO_TEST_NO_WRITE);

		// ������
		m_DebugObject->MeshRender()->Render();

		// ������ ���� ����� ������ ����
		(*iter).Age += EngineDT;
		if ((*iter).LifeTime < (*iter).Age)
		{
			iter = m_DebugShapeList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRenderMgr::ClearMRT()
{
	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Clear();
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->ClearRenderTargetView();
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->ClearRenderTargetView();
}
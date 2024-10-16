#include "pch.h"
#include "CInspector.h"

// Default Headers
#include "components.h"

// Manager Headers
#include "CLevelMgr.h"

// Level Headers
#include "CLevel.h"
#include "CLayer.h"

// GameObject Headers
#include "CGameObject.h"

// Asset UI Headers
#include "CMeshUI.h"
#include "CMeshDataUI.h"
#include "CTextureUI.h"
#include "CPrefabUI.h"
#include "CTextureUI.h"
#include "CMaterialUI.h"
#include "CSpriteUI.h"
#include "CFlipbookUI.h"
#include "CSoundUI.h"
#include "CGraphicShaderUI.h"
#include "CComputeShaderUI.h"

// Component UI Header
#include "CTransformUI.h"
#include "CCollider2DUI.h"
#include "CLight2DUI.h"
#include "CCameraUI.h"
#include "CMeshRenderUI.h"
#include "CFlipbookComponentUI.h"
#include "CTileMapUI.h"
#include "CParticleSystemUI.h"
#include "CScriptUI.h"

void CInspector::Initialize()
{
	CreateComponentUI();

	CreateAssetUI();

	if (nullptr == m_TargetObject)
	{
		SetTargetObject(CLevelMgr::GetInstance()->FindObjectByName(L"Player"));
		//SetTargetObject(CLevelMgr::GetInst()->FindObjectByName(L"MainCamera"));
		//SetTargetObject(CLevelMgr::GetInst()->FindObjectByName(L"PointLight 1"));
		return;
	}
}

void CInspector::CreateComponentUI()
{
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new CTransformUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetName("TransformUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->SetChildSize(ImVec2(0.f, 130.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new CCollider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetName("Collider2DUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->SetChildSize(ImVec2(0.f, 100.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new CLight2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetName("Light2DUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->SetChildSize(ImVec2(0.f, 200.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA] = new CCameraUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetName("CameraUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]->SetChildSize(ImVec2(0.f, 200.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::CAMERA]);

	m_arrComUI[(UINT)COMPONENT_TYPE::FLIPBOOKCOMPONENT] = new CFlipbookComponentUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::FLIPBOOKCOMPONENT]->SetName("FlipbookComponentUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::FLIPBOOKCOMPONENT]->SetChildSize(ImVec2(0.f, 100.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::FLIPBOOKCOMPONENT]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new CMeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetName("MeshRenderUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->SetChildSize(ImVec2(0.f, 100.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP] = new CTileMapUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetName("TileMapUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]->SetChildSize(ImVec2(0.f, 100.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::TILEMAP]);

	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = new CParticleSystemUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]->SetName("ParticleSystemUI");
	m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]->SetChildSize(ImVec2(0.f, 100.f));
	AddChild(m_arrComUI[(UINT)COMPONENT_TYPE::PARTICLESYSTEM]);
}

void CInspector::CreateScriptUI(UINT _Count)
{
	for (UINT i = 0; i < _Count; ++i)
	{
		CScriptUI* pScriptUI = new CScriptUI;

		char szScriptUIName[255] = {};
		sprintf_s(szScriptUIName, 255, "ScriptUI##%d", (int)m_vecScriptUI.size());
		pScriptUI->SetName(szScriptUIName);

		AddChild(pScriptUI);
		m_vecScriptUI.push_back(pScriptUI);
	}
}

void CInspector::CreateAssetUI()
{
	CAssetUI* UI = nullptr;

	UI = new CMeshUI;
	UI->SetName("CMeshUI");
	AddChild(UI);
	m_arrCAssetUI[(UINT)ASSET_TYPE::MESH] = UI;

	UI = new CMeshDataUI;
	UI->SetName("CMeshDataUI");
	AddChild(UI);
	m_arrCAssetUI[(UINT)ASSET_TYPE::MESH_DATA] = UI;

	UI = new CMaterialUI;
	UI->SetName("CMaterialUI");
	AddChild(UI);
	m_arrCAssetUI[(UINT)ASSET_TYPE::MATERIAL] = UI;

	UI = new CPrefabUI;
	UI->SetName("CPrefabUI");
	AddChild(UI);
	m_arrCAssetUI[(UINT)ASSET_TYPE::PREFAB] = UI;

	UI = new CTextureUI;
	UI->SetName("CTextureUI");
	AddChild(UI);
	m_arrCAssetUI[(UINT)ASSET_TYPE::TEXTURE] = UI;

	UI = new CSpriteUI;
	UI->SetName("CSpriteUI");
	AddChild(UI);
	m_arrCAssetUI[(UINT)ASSET_TYPE::SPRITE] = UI;

	UI = new CFlipbookUI;
	UI->SetName("CFlipbookUI");
	AddChild(UI);
	m_arrCAssetUI[(UINT)ASSET_TYPE::FLIPBOOK] = UI;

	UI = new CSoundUI;
	UI->SetName("CSoundUI");
	AddChild(UI);
	m_arrCAssetUI[(UINT)ASSET_TYPE::SOUND] = UI;

	UI = new CGraphicShaderUI;
	UI->SetName("CGraphicShaderUI");
	AddChild(UI);
	m_arrCAssetUI[(UINT)ASSET_TYPE::GRAPHIC_SHADER] = UI;

	UI = new CComputeShaderUI;
	UI->SetName("CComputeShaderUI");
	AddChild(UI);
	m_arrCAssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER] = UI;
}
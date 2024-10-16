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

#include "CComponentUI.h"
#include "CAssetUI.h"
#include "CScriptUI.h"

CInspector::CInspector()
	: m_TargetObject(nullptr)
	, m_arrComUI{}
	, m_arrAssetUI{}
{
}

CInspector::~CInspector()
{
}

void CInspector::SetTargetObject(CGameObject* _Object)
{
	m_TargetObject = _Object;

	// Object �� �����ϰ� �ִ� ������Ʈ�� �����ϴ� ������ƮUI �� Ȱ��ȭ ��
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTargetObject(_Object);
	}

	// Object �� �����ϰ� �ִ� Script ���� ScriptUI �� ������
	if (nullptr == m_TargetObject)
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetTargetScript(nullptr);
		}
	}
	else
	{
		const vector<CScript*>& vecScripts = m_TargetObject->GetScripts();

		// ��ũ��ƮUI ������ �����ϸ� �߰� ����
		if (m_vecScriptUI.size() < vecScripts.size())
		{
			CreateScriptUI((UINT)(vecScripts.size() - m_vecScriptUI.size()));
		}

		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			if (i < vecScripts.size())
				m_vecScriptUI[i]->SetTargetScript(vecScripts[i]);
			else
				m_vecScriptUI[i]->SetTargetScript(nullptr);
		}
	}

	// ���� UI ��Ȱ��ȭ
	m_TargetAsset = nullptr;
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->SetAsset(nullptr);
	}
}

void CInspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	if (nullptr == _Asset)
		return;

	SetTargetObject(nullptr);

	m_TargetAsset = _Asset;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->SetAsset(m_TargetAsset);
	}
}

void CInspector::Update()
{
	if (nullptr == m_TargetObject)
		return;

	SetTargetObject(m_TargetObject);

	// ===========
	// Object Name
	// ===========
	string strObjectName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	ImGui::Text("Object Name");
	ImGui::SameLine(108);
	ImGui::InputText("##ObjectName", (char*)strObjectName.c_str(), strObjectName.length(), ImGuiInputTextFlags_ReadOnly);

	// ======
	// Layer
	// ======
	int LayerIdx = m_TargetObject->GetLayerIdx();
	CLevel* pCurLevel = CLevelMgr::GetInstance()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(LayerIdx);
	string LayerName = string(pLayer->GetName().begin(), pLayer->GetName().end());

	char buffer[50] = {};

	if (LayerName.empty())
		sprintf_s(buffer, 50, "%d : %s", LayerIdx, "None");
	else
		sprintf_s(buffer, 50, "%d : %s", LayerIdx, LayerName.c_str());

	ImGui::Text("Layer");
	ImGui::SameLine(108);
	ImGui::InputText("##LayerName", buffer, strlen(buffer), ImGuiInputTextFlags_ReadOnly);
}

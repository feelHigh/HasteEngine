#include "pch.h"
#include "CScriptUI.h"

// Component Headers
#include "CScript.h"

// Editor Headers
#include "CParameterUI.h"
#include "CListUI.h"

// Script Headers
#include <Scripts/CScriptMgr.h>

CScriptUI::CScriptUI()
	: CComponentUI(COMPONENT_TYPE::SCRIPT)
	, m_Script(nullptr)
	, m_UIHeight(0)
{
}

CScriptUI::~CScriptUI()
{
}

void CScriptUI::Update()
{
	m_UIHeight = 0;

	ImGui::PushID((int)GetComponentUIType());
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.8f));

	wstring strScriptName = CScriptMgr::GetScriptName(m_Script);
	ImGui::Button(string(strScriptName.begin(), strScriptName.end()).c_str());

	ImGui::PopStyleColor(3);
	ImGui::PopID();

	m_UIHeight += (int)ImGui::GetItemRectSize().y;


	// Script 에서 노출시킬 데이터를 보여준다.
	const vector<tScriptParam>& vecParam = m_Script->GetScriptParam();

	for (size_t i = 0; i < vecParam.size(); ++i)
	{
		switch (vecParam[i].Type)
		{
		case SCRIPT_PARAM::INT:
			CParameterUI::InputInt((int*)vecParam[i].pData, vecParam[i].Desc);
			break;
		case SCRIPT_PARAM::FLOAT:
			CParameterUI::InputFloat((float*)vecParam[i].pData, vecParam[i].Desc);
			break;
		case SCRIPT_PARAM::VEC2:
			CParameterUI::InputFloat((float*)vecParam[i].pData, vecParam[i].Desc);
			break;
		case SCRIPT_PARAM::VEC3:

			break;
		case SCRIPT_PARAM::VEC4:
			CParameterUI::InputVec4((Vec4*)vecParam[i].pData, vecParam[i].Desc);
			break;
		case SCRIPT_PARAM::TEXTURE:
		{
			Ptr<CTexture>& pTex = *((Ptr<CTexture>*)vecParam[i].pData);
			CParameterUI::InputTexture(pTex, vecParam[i].Desc);
			m_UIHeight += 8;
			break;
		}
		case SCRIPT_PARAM::PREFAB:
		{
			Ptr<CPrefab>& pPrefab = *((Ptr<CPrefab>*)vecParam[i].pData);

			if (CParameterUI::InputPrefab(pPrefab, vecParam[i].Desc, this, (DELEGATE_1)&CScriptUI::SelectPrefab))
			{
				m_SelectedPrefab = &pPrefab;
			}

			m_UIHeight += 8;
			break;
		}
		break;
		}

		m_UIHeight += (int)ImGui::GetItemRectSize().y;
	}

	SetChildSize(ImVec2(0.f, (float)m_UIHeight));
}

void CScriptUI::SetTargetScript(CScript* _Script)
{
	m_Script = _Script;

	if (nullptr != m_Script)
		SetActive(true);
	else
		SetActive(false);
}

void CScriptUI::SelectPrefab(DWORD_PTR _ListUI)
{
	CListUI* pListUI = (CListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if ("None" == strName)
	{
		*m_SelectedPrefab = nullptr;
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());
	Ptr<CPrefab> pPrefab = CAssetMgr::GetInstance()->FindAsset<CPrefab>(strAssetName);

	assert(pPrefab.Get());

	*m_SelectedPrefab = pPrefab;
}

#include "pch.h"
#include "CParameterUI.h"

// Manager Headers
#include "CAssetMgr.h"
#include "CEditorMgr.h"

// Editor Headers
#include "CListUI.h"

// ImGui Headers
#include "ImGui/imgui.h"

UINT CParameterUI::g_ID = 0;

bool CParameterUI::InputInt(int* _Data, const string& _Desc)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::InputInt(szID, _Data, 0))
	{
		return true;
	}

	return false;
}

bool CParameterUI::DragInt(int* _Data, float _Step, const string& _Desc)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::DragInt(szID, _Data, _Step))
	{
		return true;
	}

	return false;
}

bool CParameterUI::InputFloat(float* _Data, const string& _Desc)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::InputFloat(szID, _Data, 0))
	{
		return true;
	}

	return false;
}

bool CParameterUI::DragFloat(float* _Data, float _Step, const string& _Desc)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::InputFloat(szID, _Data, _Step))
	{
		return true;
	}

	return false;
}

bool CParameterUI::InputVec2(Vec2* _Data, const string& _Desc)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::InputFloat2(szID, *_Data, 0))
	{
		return true;
	}

	return false;
}

bool CParameterUI::DragVec2(Vec2* _Data, float _Step, const string& _Desc)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::DragFloat2(szID, *_Data, _Step))
	{
		return true;
	}

	return false;
}

bool CParameterUI::InputVec4(Vec4* _Data, const string& _Desc)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::InputFloat4(szID, *_Data, 0))
	{
		return true;
	}

	return false;
}

bool CParameterUI::DragVec4(Vec4* _Data, float _Step, const string& _Desc)
{
	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	char szID[255] = {};
	sprintf_s(szID, 255, "##Input%d", g_ID++);

	if (ImGui::DragFloat4(szID, *_Data, _Step))
	{
		return true;
	}

	return false;
}

#include "CTreeUI.h"
bool CParameterUI::InputTexture(Ptr<CTexture>& _CurTex, const string& _Desc
								, CEditor* _Inst, DELEGATE_1 _MemFunc)
{
	Ptr<CTexture> CurTex = _CurTex;

	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	// �̹���
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImTextureID TexID = nullptr;
	if (nullptr != CurTex)
		TexID = CurTex->GetSRV().Get();

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	ImGui::Image(TexID, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
			{
				_CurTex = ((CTexture*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	// DragDrop ���� ���� �ؽ��İ� �ٲ���
	if (CurTex != _CurTex)
		return true;


	// List Button
	if (nullptr == _Inst && nullptr == _MemFunc)
	{
		return false;
	}

	char szID[255] = {};
	sprintf_s(szID, 255, "##InputBtn%d", g_ID++);

	ImGui::SameLine();
	if (ImGui::Button(szID, ImVec2(18.f, 18.f)))
	{
		CListUI* pListUI = (CListUI*)CEditorMgr::GetInstance()->FindEditor("List");
		pListUI->SetName("Texture");
		vector<string> vecTexNames;
		CAssetMgr::GetInstance()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
		pListUI->AddList(vecTexNames);
		pListUI->AddDelegate(_Inst, (DELEGATE_1)_MemFunc);
		pListUI->SetActive(true);

		return true;
	}

	return false;
}

bool CParameterUI::InputPrefab(Ptr<CPrefab>& _CurPrefab, const string& _Desc, CEditor* _Inst, DELEGATE_1 _MemFunc)
{
	Ptr<CPrefab> CurPrefab = _CurPrefab;

	ImGui::Text(_Desc.c_str());
	ImGui::SameLine(120);

	string PrefabName;

	if (CurPrefab.Get())
		PrefabName = string(CurPrefab->GetKey().begin(), CurPrefab->GetKey().end());

	char szID[255] = {};
	sprintf_s(szID, 255, "##PrefabKey%d", g_ID++);

	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText(szID, (char*)PrefabName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::PREFAB == pAsset->GetAssetType())
			{
				_CurPrefab = ((CPrefab*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	// DragDrop ���� ���� �ؽ��İ� �ٲ���
	if (CurPrefab != _CurPrefab)
		return true;


	// List Button
	if (nullptr == _Inst && nullptr == _MemFunc)
	{
		return false;
	}

	sprintf_s(szID, 255, "##InputBtn%d", g_ID++);

	ImGui::SameLine();
	if (ImGui::Button(szID, ImVec2(18.f, 18.f)))
	{
		CListUI* pListUI = (CListUI*)CEditorMgr::GetInstance()->FindEditor("List");
		pListUI->SetName("Prefab");
		vector<string> vecPrefabName;
		CAssetMgr::GetInstance()->GetAssetNames(ASSET_TYPE::PREFAB, vecPrefabName);
		pListUI->AddList(vecPrefabName);
		pListUI->AddDelegate(_Inst, (DELEGATE_1)_MemFunc);
		pListUI->SetActive(true);

		return true;
	}

	return false;
}

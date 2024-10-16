#include "pch.h"
#include "CMeshRenderUI.h"

// Manager Headers
#include "CAssetMgr.h"
#include "CEditorMgr.h"

// Asset Headers
#include "CMesh.h"

// Component Headers
#include "CMaterial.h"
#include "CMeshRender.h"

// Editor Headers
#include "CListUI.h"
#include "CTreeUI.h"

CMeshRenderUI::CMeshRenderUI()
	: CComponentUI(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRenderUI::~CMeshRenderUI()
{
}

void CMeshRenderUI::Update()
{
	Title();

	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();

	// 메쉬 정보
	Ptr<CMesh> pMesh = pMeshRender->GetMesh();

	string MeshName;

	if (pMesh.Get())
		MeshName = string(pMesh->GetKey().begin(), pMesh->GetKey().end());

	ImGui::Text("Mesh");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##MeshKey", (char*)MeshName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::MESH == pAsset->GetAssetType())
			{
				pMeshRender->SetMesh((CMesh*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", ImVec2(18.f, 18.f)))
	{
		CListUI* pListUI = (CListUI*)CEditorMgr::GetInstance()->FindEditorUI("List");
		pListUI->SetName("Mesh");
		vector<string> vecMeshNames;
		CAssetMgr::GetInstance()->GetAssetNames(ASSET_TYPE::MESH, vecMeshNames);
		pListUI->AddList(vecMeshNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&CMeshRenderUI::SelectMesh);
		pListUI->SetActive(true);
	}


	// 머티리얼 정보
	Ptr<CMaterial> pMtrl = pMeshRender->GetMaterial();

	string MtrlName;
	if (pMtrl.Get())
		MtrlName = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());

	ImGui::Text("Material");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##MaterialKey", (char*)MtrlName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);


	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::MATERIAL == pAsset->GetAssetType())
			{
				pMeshRender->SetMaterial((CMaterial*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}



	ImGui::SameLine();
	if (ImGui::Button("##MtrlBtn", ImVec2(18.f, 18.f)))
	{
		CListUI* pListUI = (CListUI*)CEditorMgr::GetInstance()->FindEditorUI("List");
		pListUI->SetName("Material");
		vector<string> vecMtrlNames;
		CAssetMgr::GetInstance()->GetAssetNames(ASSET_TYPE::MATERIAL, vecMtrlNames);
		pListUI->AddList(vecMtrlNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&CMeshRenderUI::SelectMaterial);
		pListUI->SetActive(true);
	}
}

void CMeshRenderUI::SelectMesh(DWORD_PTR _ListUI)
{
	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();

	CListUI* pListUI = (CListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		pMeshRender->SetMesh(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CMesh> pMesh = CAssetMgr::GetInstance()->FindAsset<CMesh>(strAssetName);

	assert(pMesh.Get());

	pMeshRender->SetMesh(pMesh);
}

void CMeshRenderUI::SelectMaterial(DWORD_PTR _ListUI)
{
	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();

	CListUI* pListUI = (CListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if ("None" == strName)
	{
		pMeshRender->SetMaterial(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CMaterial> pMtrl = CAssetMgr::GetInstance()->FindAsset<CMaterial>(strAssetName);

	assert(pMtrl.Get());

	pMeshRender->SetMaterial(pMtrl);
}

#include "pch.h"
#include "COutliner.h"

// Manager Headers
#include "CLevelMgr.h"
#include "CEditorMgr.h"

// Level Headers
#include "CLevel.h"
#include "CLayer.h"

// GameObject Headers
#include "CGameObject.h"

// Editor Headers
#include "CTreeUI.h"
#include "CInspector.h"

COutliner::COutliner()
{
	m_Tree = new CTreeUI;
	m_Tree->SetName("COutlinerTree");
	AddChild(m_Tree);

	// Ʈ�� �ɼ� ����
	// ��Ʈ ������ �ʱ�
	m_Tree->ShowRoot(false);

	// Drag, Drop On
	m_Tree->UseDrag(true);
	m_Tree->UseDrop(true);

	// Clicked Delegate ���
	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&COutliner::GameObjectClicked);

	// Self DragDrop Delegate ���
	m_Tree->AddDragDropDelegate(this, (DELEGATE_2)&COutliner::GameObjectAddChild);

	// �ܺ� ��� Delegate ���
	m_Tree->AddDropDelegate(this, (DELEGATE_2)&COutliner::DroppedFromOuter);
	m_Tree->SetDropPayLoadName("ContentTree");


	// Asset ���¸� Content �� TreeUI �� �ݿ�
	RenewLevel();
}

COutliner::~COutliner()
{
}

void COutliner::Update()
{
	if (CLevelMgr::GetInstance()->IsLevelChanged())
		RenewLevel();
}

void COutliner::RenewLevel()
{
	// ��� ���� ����
	m_Tree->Clear();

	// ��Ʈ��� ����
	TreeNode* pRootNode = m_Tree->AddNode(nullptr, "Root", 0);


	CLevel* pLevel = CLevelMgr::GetInstance()->GetCurrentLevel();
	if (nullptr == pLevel)
		return;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pLevel->GetLayer(i);
		const vector<CGameObject*>& vecObjects = pLayer->GetParentObjects();

		for (size_t i = 0; i < vecObjects.size(); ++i)
		{
			AddGameObject(pRootNode, vecObjects[i]);
		}
	}
}

void COutliner::AddGameObject(TreeNode* pNode, CGameObject* _Object)
{
	string ObjectName = string(_Object->GetName().begin(), _Object->GetName().end());

	TreeNode* pObjectNode = m_Tree->AddNode(pNode, ObjectName.c_str(), (DWORD_PTR)_Object);

	const vector<CGameObject*>& vecChild = _Object->GetChildren();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(pObjectNode, vecChild[i]);
	}
}

void COutliner::GameObjectAddChild(DWORD_PTR _Param1, DWORD_PTR _Param2)
{
	TreeNode* pDragNode = (TreeNode*)_Param1;
	TreeNode* pDropNode = (TreeNode*)_Param2;

	CGameObject* pDragObject = (CGameObject*)pDragNode->GetData();
	CGameObject* pDropObject = nullptr;

	// Drag ������Ʈ�� Drop ������Ʈ�� �ڽ����� �־��ش�.
	if (pDropNode)
	{
		pDropObject = (CGameObject*)pDropNode->GetData();

		// �ڽ����� �� ������Ʈ�� �θ�(����) �� �ϳ����ٸ� �����Ѵ�.
		if (pDropObject->IsAncestor(pDragObject))
			return;

		pDropObject->AddChild(pDragObject);
	}

	// Drop �������� ���� ������, Drag ������Ʈ�� �ֻ��� �θ�� ������ش�.
	else
	{
		if (!pDragObject->GetParent())
			return;

		// �θ������Ʈ�� ������ ����
		pDragObject->DeregisterChild();

		// ���� �Ҽ� ���̾ �ֻ��� �θ�μ� ���� �Ѵ�.
		CLevelMgr::GetInstance()->GetCurrentLevel()->RegisterAsParent(pDragObject->GetLayerIdx(), pDragObject);
	}
}

void COutliner::DroppedFromOuter(DWORD_PTR _OuterData, DWORD_PTR _DropNode)
{
	TreeNode* ContentNode = *((TreeNode**)_OuterData);
	TreeNode* DropNode = (TreeNode*)_DropNode;

}

void COutliner::GameObjectClicked(DWORD_PTR _Param)
{
	TreeNode* pNode = (TreeNode*)_Param;

	CGameObject* pObject = (CGameObject*)pNode->GetData();

	CInspector* pInspector = (CInspector*)CEditorMgr::GetInstance()->FindEditor("Inspector");
	pInspector->SetTargetObject(pObject);
	//ImGui::SetWindowFocus(nullptr);
}

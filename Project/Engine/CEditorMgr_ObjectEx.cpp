#include "pch.h"
#include "CEditorMgr.h"

// Default Headers
#include "components.h"

// Manager Headers
#include "CRenderMgr.h"

// GameObject Headers
#include "CGameObjectEx.h"

// Scripts
#include <Scripts/CEditorCameraScript.h>

void CEditorMgr::CreateEditorObject()
{
	CGameObject* pEditorCamera = new CGameObjectEx;
	pEditorCamera->SetName(L"EditorCamera");
	pEditorCamera->AddComponent(new CTransform);
	pEditorCamera->AddComponent(new CCamera);
	pEditorCamera->AddComponent(new CEditorCameraScript);

	pEditorCamera->Transform()->SetRelativePosition(0.f, 0.f, -1000.f);

	pEditorCamera->Camera()->SetLayerAll();
	pEditorCamera->Camera()->SetLayer(31, false);
	pEditorCamera->Camera()->SetFar(100000.f);
	pEditorCamera->Camera()->SetProjectionType(PROJECTION_TYPE::PERSPECTIVE);

	m_vecEditorObject.push_back(pEditorCamera);

	CRenderMgr::GetInstance()->RegisterEditorCamera(pEditorCamera->Camera());
}

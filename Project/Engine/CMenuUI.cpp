#include "pch.h"
#include "CMenuUI.h"

// Default Headers
#include "assets.h"

// Manager Headers
#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CEditorMgr.h"

// Level Headers
#include "CLevel.h"

// GameObject Headers
#include "CGameObject.h"

// Component Headers
#include "CScript.h"

// Editor Headers

// Module Headers
#include "CLevelSaveLoad.h"

// Scripts
#include <Scripts/CScriptMgr.h>

CMenuUI::CMenuUI()
{
}

CMenuUI::~CMenuUI()
{
}

void CMenuUI::Tick()
{
	if (!IsActive())
		return;

	if (ImGui::BeginMainMenuBar())
	{

		Update();

		ImGui::EndMainMenuBar();
	}
}

void CMenuUI::Update()
{
	File();

	Level();

	GameObject();

	Assets();
}

void CMenuUI::File()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Level Save"))
		{

		}

		if (ImGui::MenuItem("Level Load"))
		{

		}

		ImGui::EndMenu();
	}
}

void CMenuUI::Level()
{
	CLevel* pCurLevel = CLevelMgr::GetInstance()->GetCurrentLevel();

	ImGui::BeginDisabled(!pCurLevel);

	if (ImGui::BeginMenu("Level"))
	{
		LEVEL_STATE State = LEVEL_STATE::STOP;
		if (nullptr != pCurLevel)
			State = pCurLevel->GetState();

		ImGui::BeginDisabled(LEVEL_STATE::PLAY == State);
		if (ImGui::MenuItem("Play"))
		{
			if (LEVEL_STATE::STOP == State)
			{
				wstring strLevelPath = CPathMgr::GetInstance()->GetContentPath();
				strLevelPath += L"level\\Temp.lv";
				CLevelSaveLoad::SaveLevel(strLevelPath, pCurLevel);
			}

			ChangeLevelState(LEVEL_STATE::PLAY);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::PLAY != State);
		if (ImGui::MenuItem("Pause"))
		{
			ChangeLevelState(LEVEL_STATE::PAUSE);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::STOP == State);
		if (ImGui::MenuItem("Stop"))
		{
			wstring StrLevelLoadPath = CPathMgr::GetInstance()->GetContentPath();
			StrLevelLoadPath += L"level\\Temp.lv";
			CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(StrLevelLoadPath);
			ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

			// Inspector Clear �ϱ� (���� ������Ʈ ������ �����ְ� ���� ���� �ֱ� ������)				
			/*CInspector* pInspector = (CInspector*)CEditorMgr::GetInstance()->FindEditor("Inspector");
			pInspector->SetTargetObject(nullptr);
			pInspector->SetTargetAsset(nullptr);*/
		}
		ImGui::EndDisabled();

		ImGui::EndMenu();
	}

	ImGui::EndDisabled();
}

void CMenuUI::GameObject()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::MenuItem("Create Empty Object"))
		{

		}

		if (ImGui::BeginMenu("Add Component"))
		{
			ImGui::MenuItem("MeshRender");
			ImGui::MenuItem("Collider2D");
			ImGui::MenuItem("Camera");

			ImGui::EndMenu();
		}

		AddScript();


		ImGui::EndMenu();
	}
}

void CMenuUI::AddScript()
{
	if (ImGui::BeginMenu("Add Script"))
	{
		vector<wstring> vecScriptsName;
		CScriptMgr::GetScriptInfo(vecScriptsName);

		for (size_t i = 0; i < vecScriptsName.size(); ++i)
		{
			if (ImGui::MenuItem(string(vecScriptsName[i].begin(), vecScriptsName[i].end()).c_str()))
			{
				//CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);

				// �ν�����
				//CInspector* pInspector = (CInspector*)CEditorMgr::GetInstance()->FindEditor("Inspector");

				//// Ÿ�� ������Ʈ �˾Ƴ�
				//CGameObject* pObject = pInspector->GetTargetObject();
				//if (nullptr != pObject)
				//{
				//	// ������Ʈ��, ������ ��ũ��Ʈ�� �߰�����
				//	CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);
				//	pObject->AddComponent(pScript);
				//}
			}
		}

		ImGui::EndMenu();
	}
}

void CMenuUI::Assets()
{
	if (ImGui::BeginMenu("Assets"))
	{
		if (ImGui::MenuItem("Create Empty Material"))
		{
			Ptr<CMaterial> pMtrl = new CMaterial;
			wstring Key = GetAssetKey(ASSET_TYPE::MATERIAL, L"Default Material");
			CAssetMgr::GetInstance()->AddAsset<CMaterial>(Key, pMtrl);
			pMtrl->Save(Key);
		}



		/*CEditor* pSpriteEditor = CEditorMgr::GetInstance()->FindEditor("SpriteEditor");
		bool IsActive = pSpriteEditor->IsActive();

		if (ImGui::MenuItem("Sprite Editor", nullptr, &IsActive))
		{
			CEditorMgr::GetInstance()->FindEditor("SpriteEditor")->SetActive(IsActive);
		}*/

		ImGui::EndMenu();
	}
}

wstring CMenuUI::GetAssetKey(ASSET_TYPE _Type, const wstring& _KeyFormat)
{
	wstring Key;

	switch (_Type)
	{
	case ASSET_TYPE::MATERIAL:
	{
		Key = wstring(L"Material\\") + _KeyFormat + L" %d.mtrl";
	}
	break;
	case ASSET_TYPE::PREFAB:
	{
		Key = wstring(L"Prefab\\") + _KeyFormat + L" %d.pref";
	}
	break;
	case ASSET_TYPE::SPRITE:
	{
		Key = wstring(L"Sprite\\") + _KeyFormat + L" %d.sprite";
	}
	break;
	case ASSET_TYPE::FLIPBOOK:
	{
		Key = wstring(L"Flipbook\\") + _KeyFormat + L" %d.flip";
	}
	break;
	default:
		assert(nullptr);
		break;
	}

	wchar_t szKey[255] = {};
	wstring FilePath = CPathMgr::GetInstance()->GetContentPath();

	for (UINT i = 0; i < 0xffffffff; ++i)
	{
		swprintf_s(szKey, 255, Key.c_str(), i);

		if (false == std::filesystem::exists(FilePath + szKey))
		{
			break;
		}
	}

	return szKey;
}

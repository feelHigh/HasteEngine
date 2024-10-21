#include "pch.h"
#include "CContentDrawer.h"

// Default Headers
#include "assets.h"

// Manager Headers
#include "CAssetMgr.h"
#include "CTaskMgr.h"
#include "CEditorMgr.h"

// Editor Headers
#include "CTreeUI.h"

CContentDrawer::CContentDrawer()
{
	m_Tree = new CTreeUI;
	m_Tree->SetName("ContentDrawerTree");
	AddChild(m_Tree);

	// Ʈ�� �ɼ� ����
	m_Tree->ShowRoot(false);	// ��Ʈ ������ �ʱ�
	m_Tree->UseDrag(true);		// Tree Drag ��� ON
	m_Tree->ShowNameOnly(true); // ����� �̸����� ��� �� Ȯ���ڴ� �����ϰ� �����̸��� ���̱�
	m_Tree->AddPopupDelegate(this, (DELEGATE_1)&CContentDrawer::PopupMenu);
	// Asset ���¸� CContentDrawer �� TreeUI �� �ݿ�
	RenewContentDrawer();
}

CContentDrawer::~CContentDrawer()
{
}

void CContentDrawer::Initialize()
{
	Reload();
}

void CContentDrawer::Update()
{
	// AssetMgr �� �������� �����, Ʈ�� ������ �籸���Ѵ�.
	if (CAssetMgr::GetInstance()->IsChanged())
	{
		RenewContentDrawer();
	}
}

void CContentDrawer::RenewContentDrawer()
{
	// Ʈ���� ������ ���� ����
	m_Tree->Clear();

	// �θ��带 �������� ���� == ��Ʈ��� �Է�
	TreeNode* pRoot = m_Tree->AddNode(nullptr, "Root");

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		TreeNode* pNode = m_Tree->AddNode(pRoot, ToString((ASSET_TYPE)i));
		pNode->SetFrame(true);

		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInstance()->GetAssets((ASSET_TYPE)i);

		for (const auto& pair : mapAsset)
		{
			m_Tree->AddNode(pNode, string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get());
		}
	}
}

void CContentDrawer::Reload()
{
	// CContentDrawer ������ �ִ� �������ϵ��� ��θ� ���� �˾Ƴ���.
	wstring CContentDrawerPath = CPathMgr::GetInstance()->GetContentPath();
	FindAssetName(CContentDrawerPath, L"*.*");

	// �˾Ƴ� ���� ���ϵ��� ��θ� ���ؼ� Asset ���� AssetMgr �� �ε��Ѵ�.
	for (size_t i = 0; i < m_vecAssetPath.size(); ++i)
	{
		LoadAsset(m_vecAssetPath[i]);
	}

	// ���� �Ŵ������� �ε��Ǿ�������, content �������� ���� ������ AssetMgr ���� �����ϱ�
	// �ε��� ���¿� �ش��ϴ� ���� ������ CContentDrawer ������ �ִ��� Exist üũ
	wstring strCContentDrawerPath = CPathMgr::GetInstance()->GetContentPath();

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInstance()->GetAssets((ASSET_TYPE)i);
		for (const auto pair : mapAsset)
		{
			// �������� ������ ������ ���� ���������� ���⶧���� �Ѿ��.
			if (pair.second->IsEngineAsset())
				continue;

			wstring strRelativePath = pair.second->GetRelativePath();

			if (false == std::filesystem::exists(strCContentDrawerPath + strRelativePath))
			{
				if (pair.second->GetRefCount() <= 1)
				{
					// ���� ������û
					CTaskMgr::GetInstance()->AddTask(tTask{ TASK_TYPE::DEL_ASSET, (DWORD_PTR)pair.second.Get(), });
				}
				else
				{
					int ret = MessageBox(nullptr, L"�ٸ� ������ �����ǰ� ���� �� �ֽ��ϴ�.\n������ �����Ͻðڽ��ϱ�?", L"���� ���� ����", MB_YESNO);
					if (ret == IDYES)
					{
						// ���� ������û
						CTaskMgr::GetInstance()->AddTask(tTask{ TASK_TYPE::DEL_ASSET, (DWORD_PTR)pair.second.Get(), });
					}
				}
			}
		}
	}
}

void CContentDrawer::FindAssetName(const wstring& _FolderPath, const wstring& _Filter)
{
	WIN32_FIND_DATA tFindData = {};

	// ��ο� �´� ���� �� ������ Ž���� �� �ִ� Ŀ�ο�����Ʈ ����
	wstring strFindPath = _FolderPath + _Filter;
	HANDLE hFinder = FindFirstFile(strFindPath.c_str(), &tFindData);
	assert(hFinder != INVALID_HANDLE_VALUE);

	// Ž�� Ŀ�ο�����Ʈ�� �̿��ؼ� ���� ������ �ݺ��ؼ� ã�Ƴ���
	while (FindNextFile(hFinder, &tFindData))
	{
		wstring strFindName = tFindData.cFileName;

		if (tFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strFindName == L"..")
				continue;

			FindAssetName(_FolderPath + strFindName + L"\\", _Filter);
		}
		else
		{
			wstring RelativePath = CPathMgr::GetInstance()->GetRelativePath(_FolderPath + strFindName);
			m_vecAssetPath.push_back(RelativePath);
		}
	}

	FindClose(hFinder);
}

void CContentDrawer::LoadAsset(const path& _Path)
{
	path ext = _Path.extension();

	if (ext == L".mesh")
		CAssetMgr::GetInstance()->Load<CMesh>(_Path, _Path);
	//else if (ext == L".mdat")
		//CAssetMgr::GetInst()->Load<CMeshData>(_Path, _Path);
	else if (ext == L".mtrl")
		CAssetMgr::GetInstance()->Load<CMaterial>(_Path, _Path);
	else if (ext == L".pref")
		CAssetMgr::GetInstance()->Load<CPrefab>(_Path, _Path);
	else if (ext == L".png" || ext == L".jpg" || ext == L".jpeg" || ext == L".bmp" || ext == L".dds" || ext == L".tga"
		|| ext == L".PNG" || ext == L".JPG" || ext == L".JPEG" || ext == L".BMP" || ext == L".DDS" || ext == L".TGA")
		CAssetMgr::GetInstance()->Load<CTexture>(_Path, _Path);
	else if (ext == L".mp3" || ext == L".mp4" || ext == L".ogg" || ext == L".wav"
		|| ext == L".MP3" || ext == L".MP4" || ext == L".OGG" || ext == L".WAV")
		CAssetMgr::GetInstance()->Load<CSound>(_Path, _Path);
	else if (ext == L".sprite")
		CAssetMgr::GetInstance()->Load<CSprite>(_Path, _Path);
	else if (ext == L".flip")
		CAssetMgr::GetInstance()->Load<CFlipbook>(_Path, _Path);
}

void CContentDrawer::PopupMenu(DWORD_PTR _Param)
{
	TreeNode* pTargetNode = (TreeNode*)_Param;

	Ptr<CAsset> pAsset = (CAsset*)pTargetNode->GetData();

	if (pAsset->GetAssetType() == ASSET_TYPE::PREFAB)
	{
		if (ImGui::MenuItem("Instantiate"))
		{
			Ptr<CPrefab> pPrefab = (CPrefab*)pAsset.Get();

			CGameObject* CloneObj = pPrefab->Instantiate();

			CreateObject(CloneObj, 0);

			ImGui::CloseCurrentPopup();
		}
	}

	ImGui::EndPopup();
}

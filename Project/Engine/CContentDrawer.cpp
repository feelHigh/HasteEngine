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

	// 트리 옵션 세팅
	m_Tree->ShowRoot(false);	// 루트 보이지 않기
	m_Tree->UseDrag(true);		// Tree Drag 기능 ON
	m_Tree->ShowNameOnly(true); // 노드의 이름에서 경로 및 확장자는 제외하고 파일이름만 보이기
	m_Tree->AddPopupDelegate(this, (DELEGATE_1)&CContentDrawer::PopupMenu);
	// Asset 상태를 CContentDrawer 의 TreeUI 에 반영
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
	// AssetMgr 에 변경점이 생기면, 트리 내용을 재구성한다.
	if (CAssetMgr::GetInstance()->IsChanged())
	{
		RenewContentDrawer();
	}
}

void CContentDrawer::RenewContentDrawer()
{
	// 트리의 내용을 전부 제거
	m_Tree->Clear();

	// 부모노드를 지정하지 않음 == 루트노드 입력
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
	// CContentDrawer 폴더에 있는 에셋파일들의 경로를 전부 알아낸다.
	wstring CContentDrawerPath = CPathMgr::GetInstance()->GetContentPath();
	FindAssetName(CContentDrawerPath, L"*.*");

	// 알아낸 에셋 파일들의 경로를 통해서 Asset 들을 AssetMgr 에 로딩한다.
	for (size_t i = 0; i < m_vecAssetPath.size(); ++i)
	{
		LoadAsset(m_vecAssetPath[i]);
	}

	// 에셋 매니저에는 로딩되어있지만, content 폴더에는 없는 에셋은 AssetMgr 에서 삭제하기
	// 로딩된 에셋에 해당하는 원본 파일이 CContentDrawer 폴더에 있는지 Exist 체크
	wstring strCContentDrawerPath = CPathMgr::GetInstance()->GetContentPath();

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInstance()->GetAssets((ASSET_TYPE)i);
		for (const auto pair : mapAsset)
		{
			// 엔진에서 제작한 에셋은 원래 원본파일이 없기때문에 넘어간다.
			if (pair.second->IsEngineAsset())
				continue;

			wstring strRelativePath = pair.second->GetRelativePath();

			if (false == std::filesystem::exists(strCContentDrawerPath + strRelativePath))
			{
				if (pair.second->GetRefCount() <= 1)
				{
					// 에셋 삭제요청
					CTaskMgr::GetInstance()->AddTask(tTask{ TASK_TYPE::DEL_ASSET, (DWORD_PTR)pair.second.Get(), });
				}
				else
				{
					int ret = MessageBox(nullptr, L"다른 곳에서 참조되고 있을 수 있습니다.\n에셋을 삭제하시겠습니까?", L"에셋 삭제 에러", MB_YESNO);
					if (ret == IDYES)
					{
						// 에셋 삭제요청
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

	// 경로에 맞는 파일 및 폴더를 탐색할 수 있는 커널오브젝트 생성
	wstring strFindPath = _FolderPath + _Filter;
	HANDLE hFinder = FindFirstFile(strFindPath.c_str(), &tFindData);
	assert(hFinder != INVALID_HANDLE_VALUE);

	// 탐색 커널오브젝트를 이용해서 다음 파일을 반복해서 찾아나감
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

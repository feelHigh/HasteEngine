#pragma once
#include "CEditor.h"

class CContentDrawer :
    public CEditor
{
public:
    CContentDrawer();
    ~CContentDrawer();

public:
    virtual void Initialize() override;
    virtual void Update() override;

public:
    // Content 폴더에 있는 리소스(에셋) 들을 확인 및 로딩
    void Reload();

private:
    void RenewContentDrawer();
    void AssetClicked(DWORD_PTR _Param);
    void PopupMenu(DWORD_PTR _Parma);

    void FindAssetName(const wstring& _FolderPath, const wstring& _Filter);
    void LoadAsset(const path& _Path);
    
private:
    class CTreeUI* m_Tree;
    vector<wstring> m_vecAssetPath; // Content 폴더에 있는 에셋 경로

};

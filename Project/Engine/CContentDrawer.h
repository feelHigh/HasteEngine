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
    // Content ������ �ִ� ���ҽ�(����) ���� Ȯ�� �� �ε�
    void Reload();

private:
    void RenewContentDrawer();
    void AssetClicked(DWORD_PTR _Param);
    void PopupMenu(DWORD_PTR _Parma);

    void FindAssetName(const wstring& _FolderPath, const wstring& _Filter);
    void LoadAsset(const path& _Path);
    
private:
    class CTreeUI* m_Tree;
    vector<wstring> m_vecAssetPath; // Content ������ �ִ� ���� ���

};

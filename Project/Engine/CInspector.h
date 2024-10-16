#pragma once
#include "CEditor.h"

#include "assets.h"

class CInspector :
    public CEditor
{
public:
    CInspector();
    ~CInspector();

public:
    virtual void Initialize() override;
    virtual void Update() override;

public:
    void SetTargetObject(CGameObject* _Object);
    void SetTargetAsset(Ptr<CAsset> _Asset);

    CGameObject* GetTargetObject() { return m_TargetObject; }
    Ptr<CAsset> GetTargetAsset() { return m_TargetAsset; }

private:
    void CreateComponentUI();
    void CreateScriptUI(UINT _Count);
    void CreateAssetUI();

private:
    class CGameObject*          m_TargetObject;
    class CComponentUI*         m_arrComUI[(UINT)COMPONENT_TYPE::END];
    vector<class CScriptUI*>    m_vecScriptUI;

    Ptr<CAsset>                 m_TargetAsset;
    class CAssetUI*             m_arrAssetUI[(UINT)ASSET_TYPE::END];

};

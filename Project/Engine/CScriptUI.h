#pragma once
#include "CComponentUI.h"

class CScript;

class CScriptUI :
    public CComponentUI
{
public:
    CScriptUI();
    ~CScriptUI();

public:
    virtual void Update() override;

public:
    void SetTargetScript(CScript* _Script);
    CScript* GetTargetScript() { return m_Script; }

private:
    void SelectPrefab(DWORD_PTR _ListUI);

private:
    CScript*        m_Script;

    UINT            m_UIHeight;

    Ptr<CPrefab>*   m_SelectedPrefab;

};

#pragma once
#include "CEditor.h"

class CListUI :
    public CEditor
{
public:
    CListUI();
    ~CListUI();

public:
    virtual void Update() override;

public:
    void AddItem(const string& _item);
    void AddList(const vector<string>& _list);
    void AddDelegate(CEditor* _UI, DELEGATE_1 _Func)
    {
        m_UI = _UI;
        m_MemFunc = _Func;
    }

    string GetSelectName() { return m_SelectedName; }

public:
    virtual void Activate() override;
    virtual void Deactivate() override;

private:
    vector<string>  m_vecList;
    int             m_SelectedIdx;
    string          m_SelectedName;

    CEditor*        m_UI;
    DELEGATE_1      m_MemFunc;

};

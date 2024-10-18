#pragma once
#include "singleton.h"

class CGameObject;
class CEditor;

class CEditorMgr :
    public CSingleton<CEditorMgr>
{
    SINGLE(CEditorMgr);
public:
    void Initialize();
    void Tick();
    void Render();

public:
    CEditor* FindEditor(const string& Name);

private:
    void ObserveContent();
    void ImGuiProgress();
    void ImGuiTick();

private:
    vector<CGameObject*>    m_vecEditorObject;
    map<string, CEditor*>   m_mapUI;

    HANDLE                  m_hNotifyHandle;

};

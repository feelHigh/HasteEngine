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

public:
    CEditor* FindEditor(const string& Name);

private:
    void ShortCut();

    void CreateEditorObject();
    void EditorObjectProgress();

    void ImGui_Initialize();
    void ObserveContent();
    void ImGuiProgress();
    void ImGuiTick();
    void CreateEditorUI();

private:
    vector<CGameObject*>    m_vecEditorObject;
    map<string, CEditor*>   m_mapUI;

    HANDLE                  m_hNotifyHandle;

};

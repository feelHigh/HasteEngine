#include "pch.h"
#include "CEditorMgr.h"

// Default Headers
#include "components.h"

// Engine Headers
#include "CEngine.h"

// Device Headers
#include "CDevice.h"

// Manager Headers
#include "CKeyMgr.h"
#include "CRenderMgr.h"
#include "CPathMgr.h"

// GameObject Headers
#include "CGameObjectEx.h"

// Editor Headers
#include "CEditor.h"

// ImGui Headers
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

// Scripts
#include <Scripts/CEditorCameraScript.h>

CEditorMgr::CEditorMgr()
    : m_hNotifyHandle(nullptr)
{
}

CEditorMgr::~CEditorMgr()
{
    Delete_Vec(m_vecEditorObject);
    Delete_Map(m_mapUI);

    // ImGui Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void CEditorMgr::Initialize()
{
    CreateEditorObject();

    ImGui_Initialize();

    // Content 폴더를 감시하는 커널 오브젝트 생성
    wstring ContentPath = CPathMgr::GetInstance()->GetContentPath();
    m_hNotifyHandle = FindFirstChangeNotification(ContentPath.c_str(), true
                                                , FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME
                                                | FILE_ACTION_ADDED | FILE_ACTION_REMOVED);
}

void CEditorMgr::Tick()
{
    ShortCut();

    EditorObjectProgress();

    ImGuiProgress();

    ObserveContent();
}

void CEditorMgr::ShortCut()
{
    if (KEY_TAP(KEY::I))
    {
        CEditor* pUI = FindEditor("Inspector");

        if (pUI->IsActive())
            pUI->SetActive(false);
        else
            pUI->SetActive(true);
    }
}

void CEditorMgr::EditorObjectProgress()
{
    for (size_t i = 0; i < m_vecEditorObject.size(); ++i)
    {
        m_vecEditorObject[i]->Tick();
    }

    for (size_t i = 0; i < m_vecEditorObject.size(); ++i)
    {
        m_vecEditorObject[i]->FinalTick();
    }
}

void CEditorMgr::ImGuiTick()
{
    ImGui::ShowDemoWindow();

    for (const auto& pair : m_mapUI)
    {
        pair.second->Tick();
    }
}

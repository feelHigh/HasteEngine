#include "pch.h"
#include "CLevelMgr.h"

// Manager Headers
#include "CTaskMgr.h"

// Level Headers
#include "CLevel.h"
#include "CLayer.h"

// Game Object Headers
#include "CGameObject.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_CurLevel)
		delete m_CurLevel;
}

void CLevelMgr::Initialize()
{
}

void CLevelMgr::Progress()
{
	m_LevelChanged = false;
	if (nullptr == m_CurLevel)
		return;

	if (m_CurLevel->GetState() == LEVEL_STATE::PLAY)
	{
		m_CurLevel->Tick();
	}

	m_CurLevel->ClearObject();
	m_CurLevel->FinalTick();
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _Name)
{
	if (m_CurLevel)
	{
		return m_CurLevel->FindObjectByName(_Name);
	}

	return nullptr;
}

void CLevelMgr::LevelChanged()
{
	CTaskMgr::GetInstance()->AddTask(tTask{ TASK_TYPE::LEVEL_CHANGED });
}

bool CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	if (m_CurLevel == _NextLevel)
		return false;

	if (nullptr != m_CurLevel)
		delete m_CurLevel;

	m_CurLevel = _NextLevel;

	// 레벨이 변경된 이벤트 ON
	LevelChanged();

	return true;
}

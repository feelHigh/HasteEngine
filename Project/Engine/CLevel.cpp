#include "pch.h"
#include "CLevel.h"

// Manager Headers
#include "CLevelMgr.h"

// Level Headers
#include "CLayer.h"

// GameObject Headers
#include "CGameObject.h"

CLevel::CLevel()
	: m_Layer{}
	, m_State(LEVEL_STATE::STOP)
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i] = new CLayer(i);
	}
}

CLevel::CLevel(const CLevel& _Origin)
	: CEntity(_Origin)
	, m_Layer{}
	, m_State(LEVEL_STATE::STOP)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i] = _Origin.m_Layer[i]->Clone();
	}
}

CLevel::~CLevel()
{
	Delete_Array(m_Layer);
}

void CLevel::Begin()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->Begin();
	}
}

void CLevel::Tick()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->Tick();
	}
}

void CLevel::FinalTick()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->FinalTick();
	}
}

void CLevel::ClearObject()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		m_Layer[i]->ClearObject();
	}
}

void CLevel::AddObject(int LayerIdx, CGameObject* _Object, bool _bMoveChild)
{
	m_Layer[LayerIdx]->AddObject(_Object, _bMoveChild);

	CLevelMgr::GetInstance()->LevelChanged();
}

void CLevel::RegisterAsParent(int LayerIdx, CGameObject* _Object)
{
	m_Layer[LayerIdx]->RegisterAsParent(_Object);
}

CGameObject* CLevel::FindObjectByName(const wstring& _Name)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecParent = m_Layer[i]->GetParentObjects();

		static list<CGameObject*> list;
		for (size_t i = 0; i < vecParent.size(); ++i)
		{
			list.clear();
			list.push_back(vecParent[i]);

			while (!list.empty())
			{
				CGameObject* pObject = list.front();
				list.pop_front();

				const vector<CGameObject*>& vecChild = pObject->GetChildren();
				for (size_t i = 0; i < vecChild.size(); ++i)
				{
					list.push_back(vecChild[i]);
				}

				if (_Name == pObject->GetName())
				{
					return pObject;
				}
			}
		}
	}

	return nullptr;
}

void CLevel::ChangeState(LEVEL_STATE _NextState)
{
	if (m_State == _NextState)
		return;

	// Stop -> Pause (X)
	if (STOP == m_State && PAUSE == _NextState)
		return;

	// Stop -> Play (���� ������ ������ ���۵Ǹ�, ������ �ִ� ��ü���� Begin �� ȣ��Ǿ�� �Ѵ�.)
	if (STOP == m_State && PLAY == _NextState)
	{
		m_State = _NextState;
		Begin();
	}
	else
	{
		m_State = _NextState;
	}

	// Play -> Stop (���� ������ ���۵Ǵ� �������� ������ �����ؾ� �Ѵ�.)
}
#include "pch.h"
#include "CEditor.h"

UINT CEditor::m_GlobalID = 0;

CEditor::CEditor()
	: m_Active(true)
	, m_Parent(nullptr)
	, m_ID(m_GlobalID++)
	, m_Modal(false)
	, m_ChildBorder(false)
	, m_UseMenuBar(false)
	, m_Move(true)
{
}

CEditor::~CEditor()
{
	Delete_Vec(m_vecChildUI);
}

void CEditor::Tick()
{
	if (!m_Active)
		return;

	bool bActive = m_Active;

	UINT flag = ImGuiWindowFlags_HorizontalScrollbar;
	if (m_UseMenuBar)
		flag = ImGuiWindowFlags_MenuBar;
	if (!m_Move)
		flag |= ImGuiWindowFlags_NoMove;

	// �ֻ��� �θ� UI �� ���
	if (nullptr == m_Parent)
	{
		// Modaless
		if (false == m_Modal)
		{
			ImGui::Begin(m_FullName.c_str(), &bActive, flag);

			if (m_Active != bActive)
			{
				SetActive(bActive);
			}

			Update();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				m_vecChildUI[i]->Tick();

				if (m_vecChildUI[i]->m_ChildBorder && i == m_vecChildUI.size() - 1)
					ImGui::Separator();
			}

			ImGui::End();
		}

		// Modal
		else
		{
			ImGui::OpenPopup(m_FullName.c_str());

			if (ImGui::BeginPopupModal(m_FullName.c_str(), &bActive))
			{
				Update();

				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					m_vecChildUI[i]->Tick();

					if (m_vecChildUI[i]->m_ChildBorder && i == m_vecChildUI.size() - 1)
						ImGui::Separator();
				}

				ImGui::EndPopup();
			}
			else
			{
				if (m_Active != bActive)
				{
					SetActive(bActive);
				}
			}
		}
	}

	// �ڽ� Ÿ�� UI �� ���
	else
	{
		if (m_ChildBorder)
		{
			ImGui::Separator();
		}

		ImGui::BeginChild(m_FullName.c_str(), m_ChildSize);

		Update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			if (m_vecChildUI[i]->m_ChildBorder)
				ImGui::Separator();

			m_vecChildUI[i]->Tick();

			if (m_vecChildUI[i]->m_ChildBorder && i == m_vecChildUI.size() - 1)
				ImGui::Separator();
		}

		ImGui::EndChild();
	}
}

void CEditor::AddChild(CEditor* _UI)
{
	_UI->m_Parent = this;
	m_vecChildUI.push_back(_UI);
}

void CEditor::SetName(const string& _Name)
{
	m_Name = _Name;

	// ID ���ڿ� ����
	m_FullName = m_Name;
	char szNum[50] = {};
	_itoa_s(m_ID, szNum, 10);
	m_FullName = m_FullName + "##" + szNum;
}

void CEditor::SetActive(bool _Active)
{
	if (m_Active == _Active)
		return;

	m_Active = _Active;

	if (m_Active)
		Activate();
	else
		Deactivate();
}

void CEditor::SetFocus()
{
	ImGui::SetWindowFocus(m_FullName.c_str());
}
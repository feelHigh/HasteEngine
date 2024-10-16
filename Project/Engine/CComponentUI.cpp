#include "pch.h"
#include "CComponentUI.h"

// GameObject Headers
#include "CGameObject.h"

// Component Headers
#include "CComponent.h"

CComponentUI::CComponentUI(COMPONENT_TYPE _Type)
	: m_TargetObject(nullptr)
	, m_Type(_Type)
{
	SetChildBorder(true);
}

CComponentUI::~CComponentUI()
{
}

void CComponentUI::SetTargetObject(CGameObject* _Object)
{
	m_TargetObject = _Object;

	if (nullptr != m_TargetObject &&
		nullptr != m_TargetObject->GetComponent(m_Type))
	{
		SetActive(true);
	}

	else
	{
		SetActive(false);
	}
}

void CComponentUI::Title()
{
	ImGui::PushID((int)m_Type);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.8f));

	ImGui::Button(ToString(m_Type));

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

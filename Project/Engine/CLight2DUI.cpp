#include "pch.h"
#include "CLight2DUI.h"

// GameObject Headers
#include "CGameObject.h"

// Component Headers
#include "CLight2D.h"

CLight2DUI::CLight2DUI()
	: CComponentUI(COMPONENT_TYPE::LIGHT2D)
{
}

CLight2DUI::~CLight2DUI()
{
}

void CLight2DUI::Update()
{
	Title();

	CLight2D* pLight = GetTargetObject()->Light2D();

	// ���� ����
	LIGHT_TYPE	Type = pLight->GetLightType();

	const char* items[] = { "DIRECTIONAL", "POINT", "SPOT" };
	const char* combo_preview_value = items[(UINT)Type];

	ImGui::Text("Light Type");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(180);

	if (ImGui::BeginCombo("##LightTypeCombo", combo_preview_value))
	{
		for (int i = 0; i < 3; i++)
		{
			const bool is_selected = ((UINT)Type == i);

			if (ImGui::Selectable(items[i], is_selected))
			{
				Type = (LIGHT_TYPE)i;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	pLight->SetLightType(Type);


	// ���� ��������	
	const tLightInfo& info = pLight->GetLightInfo();

	ImGui::Text("Light Color");
	ImGui::SameLine(100);
	ImGui::ColorEdit3("##LightColor", info.LightEssence.Color);

	ImGui::Text("Light Ambient");
	ImGui::SameLine(100);
	ImGui::ColorEdit3("##LightAmbient", info.LightEssence.Ambient);

	// ������ �ݰ� ( Point, Spot )
	ImGui::BeginDisabled(Type == LIGHT_TYPE::DIRECTIONAL);

	ImGui::Text("Light Radius");
	ImGui::SameLine(100);
	ImGui::DragFloat("##DragRadius", (float*)&info.Radius, 0.1f);

	ImGui::EndDisabled();


	// ������ ���� ����
	ImGui::BeginDisabled(Type != LIGHT_TYPE::SPOT);

	float Angle = info.Angle;
	Angle = (Angle / XM_PI) * 180.f;

	ImGui::Text("Light Angle");
	ImGui::SameLine(100);
	ImGui::DragFloat("##DragAngle", &Angle, 0.1f);

	Angle = (Angle / 180.f) * XM_PI;
	pLight->SetAngle(Angle);

	ImGui::EndDisabled();
}

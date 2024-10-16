#include "pch.h"
#include "CLight3DUI.h"

// GameObject Headers
#include "CGameObject.h"

// Component Headers
#include "CLight3D.h"

CLight3DUI::CLight3DUI()
	: CComponentUI(COMPONENT_TYPE::LIGHT3D)
{
}

CLight3DUI::~CLight3DUI()
{
}

void CLight3DUI::Update()
{
	Title();

	CLight3D* pLight = GetTargetObject()->Light3D();

	// 광원 종류
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


	// 광원 색상정보	
	const tLightInfo& info = pLight->GetLightInfo();

	ImGui::Text("Light Color");
	ImGui::SameLine(100);
	ImGui::ColorEdit3("##LightColor", info.LightEssence.Color);

	ImGui::Text("Light Ambient");
	ImGui::SameLine(100);
	ImGui::ColorEdit3("##LightAmbient", info.LightEssence.Ambient);

	// 광원의 반경 ( Point, Spot )
	ImGui::BeginDisabled(Type == LIGHT_TYPE::DIRECTIONAL);

	ImGui::Text("Light Radius");
	ImGui::SameLine(100);
	ImGui::DragFloat("##DragRadius", (float*)&info.Radius, 0.1f);

	ImGui::EndDisabled();


	// 광원의 범위 각도
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

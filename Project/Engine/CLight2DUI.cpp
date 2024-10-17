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

void CLight2DUI::SelectLightType(LIGHT_TYPE SelectLight)
{
	if (SelectLight == LIGHT_TYPE::DIRECTIONAL)
	{
		LIGHT_TYPE eType = GetTargetObject()->Light2D()->GetLightType();
		ImGui::Text("Type   "); ImGui::SameLine(); ImGui::Combo("##LightType", (int*)&eType, "Directional\0Point\0Spot\0");
		GetTargetObject()->Light2D()->SetLightType(eType);

		Vec3 vAmbient = GetTargetObject()->Light2D()->GetLightAmbient();
		ImGui::Text("Ambient "); ImGui::SameLine(); ImGui::ColorEdit3("##LightAmbient", vAmbient);
		GetTargetObject()->Light2D()->SetLightAmbient(vAmbient);
	}
	else if (SelectLight == LIGHT_TYPE::POINT)
	{
		LIGHT_TYPE eType = GetTargetObject()->Light2D()->GetLightType();
		ImGui::Text("Type   "); ImGui::SameLine(); ImGui::Combo("##LightType", (int*)&eType, "Directional\0Point\0Spot\0");
		GetTargetObject()->Light2D()->SetLightType(eType);

		Vec3 vColor = GetTargetObject()->Light2D()->GetLightColor();
		ImGui::Text("Color   "); ImGui::SameLine(); ImGui::ColorEdit3("##LightColor", vColor);
		GetTargetObject()->Light2D()->SetLightColor(vColor);

		float fRadius = GetTargetObject()->Light2D()->GetRadius();
		ImGui::Text("Radius "); ImGui::SameLine(); ImGui::DragFloat("##LightRadius", &fRadius);
		GetTargetObject()->Light2D()->SetRadius(fRadius);
	}
	else if (SelectLight == LIGHT_TYPE::SPOT)
	{
		LIGHT_TYPE eType = GetTargetObject()->Light2D()->GetLightType();
		ImGui::Text("Type   "); ImGui::SameLine(); ImGui::Combo("##LightType", (int*)&eType, "Directional\0Point\0Spot\0");
		GetTargetObject()->Light2D()->SetLightType(eType);

		Vec3 vColor = GetTargetObject()->Light2D()->GetLightColor();
		ImGui::Text("Color   "); ImGui::SameLine(); ImGui::ColorEdit3("##LightColor", vColor);
		GetTargetObject()->Light2D()->SetLightColor(vColor);

		float fRadius = GetTargetObject()->Light2D()->GetRadius();
		ImGui::Text("Radius "); ImGui::SameLine(); ImGui::DragFloat("##LightRadius", &fRadius);
		GetTargetObject()->Light2D()->SetRadius(fRadius);

		float fAngle = GetTargetObject()->Light2D()->GetAngle();
		ImGui::Text("Angle  "); ImGui::SameLine(); ImGui::DragFloat("##LightAngle", &fAngle);
		GetTargetObject()->Light2D()->SetAngle(fAngle);
	}
}

#include "pch.h"
#include "CTransformUI.h"

// GameObject Headers
#include "CGameObject.h"

// Component Headers
#include "CTransform.h"

CTransformUI::CTransformUI()
	: CComponentUI(COMPONENT_TYPE::TRANSFORM)
{
}

CTransformUI::~CTransformUI()
{
}

void CTransformUI::Update()
{
	Title();

	CTransform* pTrans = GetTargetObject()->Transform();

	Vec3 vPos = pTrans->GetRelativePosition();
	Vec3 vScale = pTrans->GetRelativeScale();
	Vec3 vRot = pTrans->GetRelativeRoatation();
	vRot = (vRot / XM_PI) * 180.f;


	ImGui::Text("Position");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##Pos", vPos);

	ImGui::Text("Scale");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##Scale", vScale);

	ImGui::Text("Rotation");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##Rot", vRot, 0.1f);

	pTrans->SetRelativePosition(vPos);
	pTrans->SetRelativeScale(vScale);

	vRot = (vRot / 180.f) * XM_PI;
	pTrans->SetRelativeRotation(vRot);

	// Independent Scale
	bool IS = pTrans->IsIndependentScale();

	ImGui::Text("Ignore Parent");
	ImGui::SameLine(100);
	if (ImGui::Checkbox("##TransIS", &IS))
	{
		pTrans->SetIndependentScale(IS);
	}

}

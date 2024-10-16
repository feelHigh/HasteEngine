#include "pch.h"
#include "CLight3D.h"

// Manager Headers
#include "CRenderMgr.h"

// Component Headers
#include "CTransform.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
{
}

CLight3D::~CLight3D()
{
}

void CLight3D::FinalTick()
{
	m_Info.WorldPos = Transform()->GetWorldPosition();
	m_Info.WorldDir = Transform()->GetWorldDirection(DIRECTION_TYPE::FRONT);

	// 자신을 RenderMgr 에 등록시킴
	CRenderMgr::GetInstance()->RegisterLight3D(this);
}

void CLight3D::SetLightType(LIGHT_TYPE _Type)
{
	m_Info.Type = _Type;
}

void CLight3D::SaveToFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight3D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}

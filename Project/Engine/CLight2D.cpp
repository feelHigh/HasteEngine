#include "pch.h"
#include "CLight2D.h"

// Manager Headers
#include "CRenderMgr.h"

// Component Headers
#include "CTransform.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
	, m_Info{}
{
}

CLight2D::~CLight2D()
{
}

void CLight2D::FinalTick()
{
	m_Info.WorldPos = Transform()->GetWorldPosition();
	m_Info.WorldDir = Transform()->GetWorldDirection(DIRECTION_TYPE::RIGHT);

	// 자신을 RenderMgr 에 등록시킴
	CRenderMgr::GetInstance()->RegisterLight2D(this);
}

void CLight2D::SetLightType(LIGHT_TYPE _Type)
{
	m_Info.Type = _Type;
}

void CLight2D::SaveToFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight2D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}

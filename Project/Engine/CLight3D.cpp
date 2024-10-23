#include "pch.h"
#include "CLight3D.h"

// Manager Headers
#include "CAssetMgr.h"
#include "CRenderMgr.h"

// Component Headers
#include "CTransform.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_Info{}
	, m_LightIdx(-1)
{
	SetLightType(LIGHT_TYPE::DIRECTIONAL);
}

CLight3D::CLight3D(const CLight3D& _Origin)
	: CComponent(_Origin)
	, m_Info(_Origin.m_Info)
	, m_LightIdx(-1)
{
	SetLightType(LIGHT_TYPE::DIRECTIONAL);
}

CLight3D::~CLight3D()
{
}

void CLight3D::FinalTick()
{
	m_Info.WorldPos = Transform()->GetWorldPosition();
	m_Info.WorldDir = Transform()->GetWorldDirection(DIRECTION_TYPE::FRONT);

	// PointLight, SphereMesh, r = 0.5f
	Transform()->SetRelativeScale(m_Info.Radius * 2.f, m_Info.Radius * 2.f, m_Info.Radius * 2.f);

	// 자신을 RenderMgr 에 등록시킴
	m_LightIdx = CRenderMgr::GetInstance()->RegisterLight3D(this);
}

void CLight3D::Render()
{
	Transform()->Binding();

	m_LightMtrl->SetScalarParameter(INT_0, m_LightIdx);
	m_LightMtrl->Binding();

	m_VolumeMesh->Render();
}

void CLight3D::SetLightType(LIGHT_TYPE _Type)
{
	m_Info.Type = _Type;

	// Directional Light
	if (m_Info.Type == LIGHT_TYPE::DIRECTIONAL)
	{
		m_VolumeMesh = CAssetMgr::GetInstance()->FindAsset<CMesh>(L"RectMesh");
		m_LightMtrl = CAssetMgr::GetInstance()->FindAsset<CMaterial>(L"DirLightMtrl");
	}
	// Point Light
	else if (m_Info.Type == LIGHT_TYPE::POINT)
	{
		m_VolumeMesh = CAssetMgr::GetInstance()->FindAsset<CMesh>(L"SphereMesh");
		m_LightMtrl = CAssetMgr::GetInstance()->FindAsset<CMaterial>(L"PointLightMtrl");
	}
	// Spot Light
	else if (m_Info.Type == LIGHT_TYPE::SPOT)
	{
		m_VolumeMesh = CAssetMgr::GetInstance()->FindAsset<CMesh>(L"ConeMesh");
		m_LightMtrl = CAssetMgr::GetInstance()->FindAsset<CMaterial>(L"SpotLightMtrl");
	}
}

void CLight3D::SetRadius(float _Radius)
{
	m_Info.Radius = _Radius;
}

void CLight3D::SaveToFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight3D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}

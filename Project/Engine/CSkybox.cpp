#include "pch.h"
#include "CSkybox.h"

// Manager Headers
#include "CAssetMgr.h"

// Component Headers
#include "CTransform.h"

CSkybox::CSkybox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_Type(SPHERE)
{
	SetSkyboxType(m_Type);
}

CSkybox::~CSkybox()
{
}

void CSkybox::SetSkyboxType(SKYBOX_TYPE _Type)
{
	m_Type = _Type;

	if (SKYBOX_TYPE::SPHERE == m_Type)
		SetMesh(CAssetMgr::GetInstance()->FindAsset<CMesh>(L"SphereMesh"));
	else if (SKYBOX_TYPE::CUBE == m_Type)
		SetMesh(CAssetMgr::GetInstance()->FindAsset<CMesh>(L"CubeMesh"));

	// Mesh 가 변경되면 재질을 다시 설정해야 한다.
	SetMaterial(CAssetMgr::GetInstance()->FindAsset<CMaterial>(L"SkyBoxMtrl"));
}

void CSkybox::FinalTick()
{
	// 1. 시점 카메라 의 최대 시야거리로, Transform 의 Scale 을 설정한다.
	//  - Play -> Main Cam
	//  - Pause, Stop -> Editor Cam

	// 2. SkyBox 위치를 시점 카메라 위치로 옮긴다.

}

void CSkybox::Render()
{
	Transform()->Binding();

	GetMaterial()->SetScalarParametereter(INT_0, (int)m_Type);

	if (m_Type == SPHERE)
	{
		if (!m_SkyboxTex->IsCubeMap())
			GetMaterial()->SetTextureParameter(TEX_0, m_SkyboxTex);
		else
			GetMaterial()->SetTextureParameter(TEX_0, nullptr);
	}

	else if (m_Type == CUBE)
	{
		if (m_SkyboxTex->IsCubeMap())
			GetMaterial()->SetTextureParameter(TEXCUBE_0, m_SkyboxTex);
		else
			GetMaterial()->SetTextureParameter(TEXCUBE_0, nullptr);
	}

	GetMaterial()->Binding();

	GetMesh()->Render();
}

void CSkybox::SaveToFile(FILE* _File)
{
}

void CSkybox::LoadFromFile(FILE* _File)
{
}
